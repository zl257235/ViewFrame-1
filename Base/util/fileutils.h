/*!
 *  @brief     文本帮助工具类
 *  @details   提供统一的文本创建、保存等操作
 *  @file
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.XX
 *  @warning
 *  @copyright NanJing RenGu.
 */

/*!
 * @details  内容包括：
 *                  xml 文件解析操作
 *                  xls表格文件的打印  暂时还不完善--测试使用中
 *
 */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QIODevice>
#include <QCoreApplication>

#include <QtXml/qdom.h>
#include <Qfile>
#include <QtXml/QDomProcessingInstruction>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>
#include <QDebug>

#include <QObject>
#include <QFile>
#include <QString>
#include <QVariant>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>
#include <QTableWidget>
#include <QTableView>
#include <QAbstractItemModel>
#include <QTableWidgetItem>
#include <QThread>

#include "../base_global.h"

typedef unsigned int  UINT ;

class QFile;

namespace FileUtils
{

class BASESHARED_EXPORT FileSaverBase
{
    Q_DECLARE_TR_FUNCTIONS(FileSaverBase)
public:
    FileSaverBase();
    ~FileSaverBase();

    QString fileName() const { return m_fileName; }
    bool hasError() const { return m_hasError; }
    QString errorString() const { return m_errorString; }

    virtual bool finalize();

    bool write(const char *data, int len);
    bool write(const QByteArray &bytes);
    bool setResult(bool ok);

protected:
    QFile * m_file;
    QString m_fileName;
    QString m_errorString;
    bool m_hasError;

};

class BASESHARED_EXPORT FileSaver : public FileSaverBase
{
    Q_DECLARE_TR_FUNCTIONS(FileSaver)
public:
    explicit FileSaver(const QString &filename, QIODevice::OpenMode mode = QIODevice::NotOpen);
    QFile *file() { return m_file; }

    virtual bool finalize();

private:
};

///--------XMl文件操作----------///

class ParseXMLMethod;
class BASESHARED_EXPORT XmlFileOpt
{
public:
   static   XmlFileOpt* getOne();

   ///写入xml文件
   void writeFileHead();                        //函数无作用

   ///解析xml文件
   void setParseMethod(ParseXMLMethod *method);       //设置解析方式
   bool readXmLsFile(QString filePath);


   ParseXMLMethod *mParseMethod;

private:
    XmlFileOpt();

    static XmlFileOpt *singleton;
};


/*!
 * \brief The xmlParse class  xml文件的解析方式
 * \warning 每个解析中的所解释出来的节构体声明为public
 * \note 装饰者模式
 */
class BASESHARED_EXPORT ParseXMLMethod
{
public:
    ParseXMLMethod(ParseXMLMethod *parent=0){mParent = parent;}
    virtual ~ParseXMLMethod(){}

    bool  startParse(QDomNode &node);
protected:
    ParseXMLMethod *mParent;
    virtual bool concreteParse(QDomNode &node) {Q_UNUSED(node) return true;}
};

///--------xls表格文件操作----------///

class XlsFileProxery;

/*!
 * \brief The XlsGenerateMethod class   xls文件的生成方式
 * *\note:使用QActiveX控件时参考的网址
 *       https://blog.csdn.net/psujtfc/article/details/42740761
 *       https://blog.csdn.net/fcqwin/article/details/17885557
 *       https://www.cnblogs.com/chanrom/p/3500264.html      工作表，单元格，布局抄袭页面
 */
class  BASESHARED_EXPORT  XlsGenerateMethod:public QThread
{
   Q_OBJECT
public:
    virtual ~XlsGenerateMethod();

    void setXlsFie(QString fileName);

    void setTitle(QString title);
    int getRowCount();
    int getColCount();

    void save();
    void close();

    /********************/
    ///工作表
    /********************/
    int  getSheetsCount();
    void addWorkSheet(QString sheetName);
    void deleteSheet(const QString& sheetName);
    void deleteSheet(int sheetIndex);
    bool openWorkSheet(int index);
    bool openWorkSheet(QString sheetName);                      ///暂时不可用，detail查看函数注释

    /*******************/
    ///单元格
    /*******************/
    bool     setCellData(UINT row, UINT column, QVariant data);
    QVariant getCellData(UINT row, UINT column);                //获取指定单元数据

    void mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn);
    void clearCelll(int row,int column);

    /**********************/
    ///格式布局
    /**********************/
    void setRowHeight(int row, int height);
    void setAutoFitRow(int row);                               //自动适应行高
    void setColumnWidth(int column, int width);

    void setCellTextCenter(int row, int column);               //设置单元格居中对齐
    void setCellTextWrap(int row, int column, bool isWrap);    //作用不明
    void setCellFontSize(const QString &cell, int size);       //设置字体大小
    void setCellFontBold(const QString &cell, bool isBold);    //设置粗体

public:
    bool bIsSaveAlready;                    //避免重复保存
    QString mxlsPath;                       //xls文件路径

private:
      QAxObject *pExcel;                    //指向整个excel应用程序
      QAxObject *pWorkBook;
      QAxObject *pWorkbooks;                //指向工作簿集,excel有很多工作簿
      QAxObject *pSheets;
      QAxObject *pWorksheet;                //指向工作簿中的某个sheet表单
      bool bIsANewFile;
      bool bhasSetFileName;

protected:
      explicit XlsGenerateMethod(bool isVisible);
      explicit XlsGenerateMethod();
};

/*!
 * \brief The XlsFileProxery class  xls文件操作代理类，也是外部的接口调用类
 */
class  BASESHARED_EXPORT  XlsFileProxery:public XlsGenerateMethod
{
public:
    XlsFileProxery(bool isVisible = false);
    virtual ~XlsFileProxery();
    virtual bool ifSetData();               //是否已经设置过数据

    /***********************/
    //不同的数据接收方式
    /***********************/

    bool AddData(QTableWidget *tableWidget);
    bool AddData(QAbstractItemModel *model);

    void setXlsFie(QString fileName);

private:
    bool mHasSetFileName;
    bool mifVisible;

   //默认代理支持的数据类型
   QTableWidget *mtable;
   QAbstractItemModel *mModel;
private:
    void WriteData();

protected:
    virtual void run();
};

}

#endif // FILEUTILS_H
