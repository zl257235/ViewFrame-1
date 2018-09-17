#include "fileutils.h"
#include "qt_windows.h"
#include <QDir>

namespace FileUtils {

XmlFileOpt* XmlFileOpt::singleton = NULL;

XmlFileOpt *XmlFileOpt::getOne()
{
    if(NULL == singleton)
    {
        singleton = new XmlFileOpt();
    }
    return singleton;
}

FileSaverBase::FileSaverBase():m_hasError(false)
{

}

FileSaverBase::~FileSaverBase()
{
    delete m_file;
}

bool FileSaverBase::finalize()
{
    m_file->close();
    m_hasError = m_file->error() == QFile::NoError;
    delete m_file;
    m_file = NULL;
    return m_hasError;
}

bool FileSaverBase::write(const char *data, int len)
{
    if (m_hasError)
    {
        return false;
    }
    return setResult(m_file->write(data, len) == len);
}

bool FileSaverBase::write(const QByteArray &bytes)
{
    if (m_hasError)
    {
        return false;
    }
    return setResult(m_file->write(bytes) == bytes.count());
}

bool FileSaverBase::setResult(bool ok)
{
    if (!ok && !m_hasError)
    {
        m_errorString = tr("Cannot write file %1. Disk full?").arg(QDir::toNativeSeparators(m_fileName));
        m_hasError = true;
    }
    return ok;
}

FileSaver::FileSaver(const QString &filename, QIODevice::OpenMode mode)
{
    m_fileName = filename;

    m_file = new QFile(filename);

    if (!m_file->open(QIODevice::WriteOnly | mode))
    {
        QString err = QFile::exists(filename) ? tr("Cannot overwrite file %1: %2") : tr("Cannot create file %1: %2");
        m_errorString = err.arg(QDir::toNativeSeparators(filename), m_file->errorString());
        m_hasError = true;
    }
}

bool FileSaver::finalize()
{
    if(!m_hasError)
    {
        if(!m_file->flush())
        {
            m_file->remove();
            return false;
        }

        m_file->close();
        if (m_file->error() != QFile::NoError) {
            m_file->remove();
            return false;
        }

        return true;
    }
    return false;
}


void XmlFileOpt::writeFileHead()
{

    //   mdoc = new QDomDocument();

    //   //添加处理指令
    //   QDomProcessingInstruction instruction;
    //   instruction = mdoc->createProcessingInstruction("xml","version = \"1.0\" encoding = \"UTF-8\"");
    //   mdoc->appendChild(instruction);


}


void XmlFileOpt::setParseMethod(ParseXMLMethod *method)
{
    if(NULL == method)
        return;
    delete mParseMethod;
    mParseMethod = method;
}

bool XmlFileOpt::readXmLsFile(QString filePath)
{
    if(filePath.isEmpty())
        return false;

    QFile file(filePath);
    if(!file.exists())
    {
        qDebug()<<QString("file %1 doesn't exist.").arg(filePath);
        return false;
    }

    if (!file.open(QIODevice::ReadOnly|QFile::Text))
    {
        qDebug()<<" open "<<filePath<<" error";
        return false;
    }

    QString error = "";
    int row = 0, column = 0;
    QDomDocument doc;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return false;
    }

    file.close();


    QDomElement root = doc.documentElement();

    if(!root.isNull())
    {
        mParseMethod->startParse(dynamic_cast<QDomNode&>(root));
    }
    return true;
}

XmlFileOpt::XmlFileOpt()
{
    mParseMethod = new ParseXMLMethod();
}
bool ParseXMLMethod::startParse(QDomNode &node)
{
    if(NULL != mParent)
    {
        return mParent->startParse(node);
    }
    return mParent->concreteParse(node);
}



XlsGenerateMethod::XlsGenerateMethod(bool isVisible)
    :QThread(0)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);


    pExcel = new QAxObject("Excel.Application");            //链接控件

    pExcel->dynamicCall("SetVisible(bool)", isVisible);     //显示或者不显示控件

    pExcel->setProperty("DisplayAlerts", false);           //不显示任何警告信息。

    pWorkBook = NULL;
    pWorkbooks = NULL;
    pSheets = NULL;
    pWorksheet = NULL;
    mxlsPath.clear();

    bIsANewFile = false;
    bIsSaveAlready = false;
    bhasSetFileName = false;

}

XlsGenerateMethod::~XlsGenerateMethod()
{
    //原版本中有先判断文件是否已经打开，调用close函数后再OleUninitialize
    OleUninitialize();
}

void XlsGenerateMethod::setXlsFie(QString fileName)
{
    if(!bhasSetFileName)
    {
        if(fileName.isEmpty())
            return;

        mxlsPath = QDir::currentPath()+"/"+fileName;

        pWorkbooks = pExcel->querySubObject("WorkBooks");     //获取工作簿
        QFile f(mxlsPath);

        if (!f.exists())
        {
            bIsANewFile  = true;
            pWorkbooks->dynamicCall("Add");                       //添加一个新的工作薄
            pWorkBook  = pExcel->querySubObject("ActiveWorkBook"); //新建一个xls
        }
        else
        {
            bIsANewFile = false;

            //打开xls对应的工作簿
            pWorkBook = pWorkbooks->querySubObject("Open(const QString&)",mxlsPath);
        }

        pSheets = pWorkBook->querySubObject("WorkSheets");

        if(!openWorkSheet(1))
        {
            qDebug()<<QString("open file %1 's first workSheet failed.").arg(mxlsPath);
            return;
        }
    }

}

int XlsGenerateMethod::getSheetsCount()
{
    if(NULL == pSheets)return -1;
    return pSheets->property("Count").toInt();
}

void XlsGenerateMethod::addWorkSheet(QString sheetName)
{
    if(NULL == pSheets)return;
    int cnt = getSheetsCount();
    QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
    pSheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
    pWorksheet = pSheets->querySubObject("Item(int)", cnt);
    pLastSheet->dynamicCall("Move(QVariant)", pWorksheet->asVariant());
    pWorksheet->setProperty("Name", sheetName);

}

void XlsGenerateMethod::deleteSheet(const QString &sheetName)
{
    if(NULL == pSheets)return;
    QAxObject * a = pSheets->querySubObject("Item(const QString&)", sheetName);
    a->dynamicCall("delete");
}

void XlsGenerateMethod::deleteSheet(int sheetIndex)
{
    if(NULL == pSheets)return;
    QAxObject * a = pSheets->querySubObject("Item(int)", sheetIndex);
    a->dynamicCall("delete");

}

bool XlsGenerateMethod::setCellData(UINT row, UINT column, QVariant data)
{
    if(NULL == pWorksheet)return false;
    bool op = false;

    QAxObject *cell = pWorksheet->querySubObject("Cells(int,int)",row,column);//获取单元格对象
    if ( cell )
    {
        QString strData = data.toString(); //excel 居然只能插入字符串和整型，浮点型无法插入
        cell->dynamicCall("SetValue(const QVariant&)",strData); //修改单元格的数据
        op = true;
    }
    else
    {
        op = false;
    }

    return op;
}

/**
  *@brief 获取指定单元格的数据
  *@param row : 单元格的行号
  *@param column : 单元格的列号
  *@return [row,column]单元格对应的数据
  */
QVariant XlsGenerateMethod::getCellData(UINT row, UINT column)
{
    if(NULL == pWorksheet)return -1;
    QVariant data;

    QAxObject *cell = pWorksheet->querySubObject("Cells(int,int)",row,column);//获取单元格对象
    if ( cell )
    {
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

void XlsGenerateMethod::mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn)
{
    if(NULL == pWorksheet)return;
    QString cell;
    cell.append(QChar(topLeftColumn - 1 + 'A'));
    cell.append(QString::number(topLeftRow));
    cell.append(":");
    cell.append(QChar(bottomRightColumn - 1 + 'A'));
    cell.append(QString::number(bottomRightRow));

    QAxObject *range = pWorksheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);

}

void XlsGenerateMethod::clearCelll(int row, int column)
{
    if(NULL == pWorksheet)return;
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = pWorksheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");


}

void XlsGenerateMethod::setRowHeight(int row, int height)
{
    if(NULL == pWorksheet)return;
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * r = pWorksheet->querySubObject("Rows(const QString &)", rowsName);
    r->setProperty("RowHeight", height);

}

void XlsGenerateMethod::setCellTextCenter(int row, int column)
{
    if(NULL == pWorksheet)return;
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = pWorksheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void XlsGenerateMethod::setCellTextWrap(int row, int column, bool isWrap)
{
    //    QString cell;
    //    cell.append(QChar(column - 1 + 'A'));
    //    cell.append(QString::number(row));

    //    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    //    range->setProperty("WrapText", isWrap);

}

void XlsGenerateMethod::setAutoFitRow(int row)
{
    if(NULL == pWorksheet)return;
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * rows = pWorksheet->querySubObject("Rows(const QString &)", rowsName);
    rows->dynamicCall("AutoFit()");

}

void XlsGenerateMethod::setColumnWidth(int column, int width)
{
    if(NULL == pWorksheet)return;
    QString columnName;
    columnName.append(QChar(column - 1 + 'A'));
    columnName.append(":");
    columnName.append(QChar(column - 1 + 'A'));

    QAxObject * col = pWorksheet->querySubObject("Columns(const QString&)", columnName);
    col->setProperty("ColumnWidth", width);
}

/*!
 * \brief XlsGenerateMethod::setCellFontSize
 * \param cell  这里的stringt类型代表的是范围，使用时参考其他函数
 * \param size
 */
void XlsGenerateMethod::setCellFontSize(const QString &cell, int size)
{
    if(NULL == pWorksheet)return;
    QAxObject *range = pWorksheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);

}

void XlsGenerateMethod::setCellFontBold(const QString &cell, bool isBold)
{
    if(NULL == pWorksheet)return;
    QAxObject *range = pWorksheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);

}

void XlsGenerateMethod::setTitle(QString title)
{
    if(NULL == pExcel)return;
    pExcel->setProperty("Caption",title);
}

int XlsGenerateMethod::getRowCount()
{
    if(NULL == pWorksheet)return -1;
    QAxObject *usedrange = pWorksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject *rows = usedrange->querySubObject("Rows");


    return rows->property("Count").toInt();       //获取行数
}

int XlsGenerateMethod::getColCount()
{
    if(NULL == pWorksheet)return -1;
    QAxObject *usedrange = pWorksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject *columns = usedrange->querySubObject("Columns");

    return columns->property("Count").toInt();    //获取列数
}


void XlsGenerateMethod::save()
{
    if(NULL == pWorkBook)return;
    if (bIsSaveAlready)
    {
        return ;
    }

    if (!bIsANewFile)
    {
        pWorkBook->dynamicCall("Save()");
    }
    else /*如果该文档是新建出来的，则使用另存为COM接口*/
    {
        pWorkBook->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                               QDir::toNativeSeparators(mxlsPath),56,QString(""),QString(""),false,false);
    }

    bIsSaveAlready = true;

}
void XlsGenerateMethod::close()
{
    //关闭前先保存数据
    save();

    if ( pExcel && pWorkBook )
    {
        pWorkBook->dynamicCall("Close(bool)", true);
        pExcel->dynamicCall("Quit()");

        delete pExcel;
        mxlsPath.clear();
        bIsANewFile = false;
        bIsSaveAlready = true;
    }
}

XlsGenerateMethod::XlsGenerateMethod()
{
    pExcel = NULL;
    pWorkBook = NULL;
    pWorkbooks = NULL;
    pSheets = NULL;
    pWorksheet = NULL;
    mxlsPath.clear();

    bIsANewFile = false;
    bIsSaveAlready = false;
    bhasSetFileName = false;
}

bool XlsGenerateMethod::openWorkSheet(int index)
{
    if(NULL == pWorkBook)return false;
    int cnt = getSheetsCount();
    if(index > cnt)
        return  false;
    pWorksheet = pWorkBook->querySubObject("WorkSheets(int)", index);
    return true;
}

/*!
 * \brief XlsGenerateMethod::openWorkSheet
 * \param sheetName
 * \return
 * \warning 此函数暂时不可用  不知道查不到会怎么样  、
 *          此外 openWorkSheet(QString sheetName) 使用的工作表集查找
 *              openWorkSheet(int index)使用的工作簿查找
 */
bool XlsGenerateMethod::openWorkSheet(QString sheetName)
{
    return false;
    pWorksheet = pSheets->querySubObject("Item(const QString&)", sheetName);
}


/*!
 * \brief XlsFileProxery::XlsFileProxery  调用父类的轻量级的构造函数，因为代理并不是一个实际操作类
 * \param isVisible
 */
XlsFileProxery::XlsFileProxery(bool isVisible)
    :XlsGenerateMethod(),mifVisible(isVisible)
{
    mHasSetFileName = false;
    mtable = NULL;
    mModel = NULL;
}

XlsFileProxery::~XlsFileProxery()
{

}



bool XlsFileProxery::AddData(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget &&NULL == mModel )
    {
        return false;
    }

    mtable = tableWidget;
    this->start();
    return true;
}

bool XlsFileProxery::AddData(QAbstractItemModel *model)
{
    if ( NULL == model && NULL == mtable)
    {
        return false;
    }
    mModel = model;
    this->start();
    return true;
}

/*!
 * \brief XlsFileProxery::setXlsFie
 * \param fileName
 */
void XlsFileProxery::setXlsFie(QString fileName)
{
    if(this->mxlsPath.isEmpty())
    {
        qDebug()<<"XlsFileProxery:fileName is unvalid";
        return;
    }
    this->mxlsPath = fileName;
}

bool XlsFileProxery::ifSetData()
{
    return (NULL != mModel && NULL != mtable);
}




void XlsFileProxery::WriteData()
{
    int tableR,tableC;
    if(NULL != mtable)
    {
        tableR = mtable->rowCount();
        tableC = mtable->columnCount();
    }
    else if(NULL != mModel)
    {
        tableR = mModel->rowCount();
        tableC = mModel->columnCount();
    }

    //获取表头写做第一行
    for (int i=0; i<tableC; i++)
    {
        if(NULL != mtable)
        {
            if ( mtable->horizontalHeaderItem(i) != NULL )
            {
                this->setCellData(1,i+1,mtable->horizontalHeaderItem(i)->text());
            }
        }
        else if(NULL != mModel)
        {
            this->setCellData(1,i+1,mModel->headerData(i,Qt::Horizontal));
        }
    }

    //写数据
    for (int i=0; i<tableR; i++)
    {
        for (int j=0; j<tableC; j++)
        {
            if(NULL != mtable)
            {
                if ( mtable->item(i,j) != NULL )
                {
                    this->setCellData(i+2,j+1,mtable->item(i,j)->text());
                }
            }
            else if(NULL != mModel)
            {
                this->setCellData(i+2,j+1,mModel->data(mModel->index(i,j)));
            }
        }
    }

    //保存
    save();
}

/*!
 * \brief XlsFileProxery::run
 * \attention 首次运行时会调用构造函数，并打开响应的文件，这么写，避免QASxobject响应时间太长
 */
void XlsFileProxery::run()
{
    if(this->mxlsPath.isEmpty())
    {
        qDebug()<<"XlsFileProxery:please set fileName first!";
        return;
    }
    if(!mHasSetFileName)
    {
        XlsGenerateMethod::XlsGenerateMethod(mifVisible);
        XlsGenerateMethod::setXlsFie(this->mxlsPath);
        mHasSetFileName = true;
    }

    if(XlsFileProxery::ifSetData())
    {
        if( NULL != mtable||NULL != mModel)
        {
            WriteData();
            return;
        }
    }
}

}
