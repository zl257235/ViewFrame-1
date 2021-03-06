#include "allplusetable.h"

#include <QString>
#include <QHBoxLayout>
#include <QRadioButton>
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"
#include <QAction>

namespace DataView {
const int PACKAGE_START=0x1ACF1ACF; //包头
const int PACKAGE_END=0xFC1DFC1D;   //包尾
const int MIN_PACKAGE_LEN=36;       //数据包最小长度
class AllPluseTablePrivate
{
    Q_DECLARE_PUBLIC(AllPluseTable)
public:
    explicit AllPluseTablePrivate(AllPluseTable * q):q_ptr(q),showTableMode(AllPluseTable::RealTimeDisplay)
    {
        initView();
        initTableViewMenu();
    }
    void initView();
    void initTableViewMenu();
    //Table *allPluseTable;
    AllPluseTable::WorkModel showTableMode;

    QWidget * mainWidget;
    AllPluseTable * q_ptr;

    QRadioButton * radioButton_RealityShow;
    QRadioButton * radioButton_HistoryShow;

    TableView* dataViewStatictis;                   //统计信息表格
    TableViewModelCustom* dataViewModelStatictis;
    TableView* dataViewOriginal;                    //原始信息表格
    TableViewModelCustom* dataViewModelOriginal;
    int dataOutsideNo;                                              //全脉冲数据总批号
    AllPluseStatisticInfoList allPluseStatisticInfoList;            //全脉冲统计列表
    AllPulseOriginalInfoList allPulseOriginalInfoList;              //全脉冲原始信息列表
    QAction *clearAction;
};

void AllPluseTablePrivate::initView()
{
    mainWidget = new QWidget();

    QWidget * radioWidget = new QWidget();
    radioButton_RealityShow = new QRadioButton(radioWidget);
    radioButton_HistoryShow = new QRadioButton(radioWidget);

    QObject::connect(radioButton_RealityShow,SIGNAL(clicked()),q_ptr,SLOT(on_radioButton_RealityShow_clicked()));
    QObject::connect(radioButton_HistoryShow,SIGNAL(clicked()),q_ptr,SLOT(on_radioButton_HistoryShow_clicked()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(radioButton_RealityShow);
    layout->addWidget(radioButton_HistoryShow);
    radioWidget->setLayout(layout);

    dataViewStatictis=new TableView(q_ptr);
    dataViewStatictis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModelStatictis=new TableViewModelCustom(dataViewStatictis);
    dataViewStatictis->setModel(dataViewModelStatictis);
    dataViewModelStatictis->setTableCustomKind(STATISTICAL_INFO);

    dataViewOriginal=new TableView(q_ptr);
    dataViewOriginal->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModelOriginal=new TableViewModelCustom(dataViewOriginal);
    dataViewOriginal->setModel(dataViewModelOriginal);
    dataViewModelOriginal->setTableCustomKind(ORIGINAL_INFO);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);

    vlayout->addWidget(dataViewStatictis);
    vlayout->addWidget(dataViewOriginal);

    mainWidget->setLayout(vlayout);

    dataOutsideNo=1001;        //总批号初始化为1001
    QObject::connect(dataViewStatictis, SIGNAL(tableCheckDoubleSignal(QModelIndex)), q_ptr, SLOT(doubleClickedTable(QModelIndex)));

}

void AllPluseTablePrivate::initTableViewMenu()
{
    clearAction = new QAction();
    dataViewStatictis->addAction(clearAction);
    QObject::connect(clearAction, SIGNAL(triggered(bool)), q_ptr, SLOT(clearTable()));
    clearAction->setText(QObject::tr("Clear Table"));
}

AllPluseTable::AllPluseTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_ALL_PLUS_TABLE,parent),d_ptr(new AllPluseTablePrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    initAllPulseTable();
    retranslateUi();
}

AllPluseTable::~AllPluseTable()
{
}

bool AllPluseTable::initialize()
{
    Q_D(AllPluseTable);
    setWidget(d->mainWidget);
    return true;
}

void AllPluseTable::release()
{

}

void AllPluseTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void AllPluseTable::initAllPulseTable()
{
    Q_D(AllPluseTable);

    //初始化统计信息列表
    QStringList headInfo;
    headInfo<<QStringLiteral("序号")<<QStringLiteral("统计参数名称")<<QStringLiteral("最小值")<<QStringLiteral("最大值")<<QStringLiteral("均值")
               <<QStringLiteral("均方差");

    d_ptr->dataViewModelStatictis->resetHeadInfo(headInfo);
    d_ptr->dataViewStatictis->setColumnWidth(0,70);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataViewStatictis->setColumnWidth(i,110);
    }

    //初始化原始数据列表
    headInfo.clear();
    headInfo<<QStringLiteral("序号")<<QStringLiteral("属性名称")<<QStringLiteral("属性是否绘图")<<QStringLiteral("属性值");

    d_ptr->dataViewModelOriginal->resetHeadInfo(headInfo);
    d_ptr->dataViewOriginal->setColumnWidth(0,70);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataViewOriginal->setColumnWidth(i,170);
    }
}


void AllPluseTable::retranslateUi()
{
    Q_D(AllPluseTable);
    m_name = tr("Full Pulse List");
    setWindowTitle(m_name);

    d->radioButton_RealityShow->setText(tr("Real-time display"));
    d->radioButton_HistoryShow->setText(tr("Historical display"));
}

/*!
 * @brief 实时模式单选框槽
 */
void AllPluseTable::on_radioButton_RealityShow_clicked()
{
    Q_D(AllPluseTable);
    if(d->showTableMode != RealTimeDisplay)
    {
        d->showTableMode = RealTimeDisplay;
        AllPulseOriginalInfoList originalList;      //清空原始数据
        d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
    }
}

/*!
 * @brief 历史模式单选框槽
 */
void AllPluseTable::on_radioButton_HistoryShow_clicked()
{
    Q_D(AllPluseTable);
    if(d->showTableMode!=HistoricalDisplay)
    {
        d->showTableMode = HistoricalDisplay;
        AllPulseOriginalInfoList originalList;      //清空原始数据
        d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
    }
}

/*!
 * @brief 接收全脉冲数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void AllPluseTable::recvAllPlusePara(char *buff, int len)
{
    if((buff==NULL)||(len<MIN_PACKAGE_LEN))
    {
        return;
    }
    int startCode;          //包头
    int endCode;            //包尾
    int bufMemcpyPos=0;     //缓冲拷贝位置
    int iPackageLen;        //缓冲区总长度(从包头至包尾)
    AllPluseStatisticInfoBase statisticInfoBase;
    AllPluseOriginalInfoAttributeBase originalInfoBase;
    AllPluseStatisticInfo statisticInfo;
    AllPluseOriginalInfoAttribute originalInfo;
    AllPulseOriginalInfoList originalInfoTempList;  //全脉冲原始信息临时列表
    int statisticInfoEff;       //统计信息有效标识 1:有效
    int originalInfoEff;        //原始数据有效标识
    int statisticInfoNumN=0;    //统计参数个数N
    int originalInfoNumM=0;     //原始数据个数M
    int attributeInfoNumL=0;    //原始数据属性个数L

    memcpy(&startCode,buff,sizeof(int));
    bufMemcpyPos=4+2;
    memcpy(&iPackageLen,buff+bufMemcpyPos,sizeof(int));
    if(iPackageLen<MIN_PACKAGE_LEN)
    {
        return;
    }

    memcpy(&endCode,buff+iPackageLen-4,sizeof(int));
    if((startCode!=PACKAGE_START)||(endCode!=PACKAGE_END))
    {
        return;
    }

    bufMemcpyPos+=sizeof(int);
    memcpy(&statisticInfoEff,buff+bufMemcpyPos,sizeof(int));

    bufMemcpyPos+=sizeof(int);
    memcpy(&statisticInfoNumN,buff+bufMemcpyPos,sizeof(int));

    if(statisticInfoEff==1) //统计信息有效
    {
        bufMemcpyPos+=sizeof(int);
        if((bufMemcpyPos+sizeof(AllPluseStatisticInfoBase)*statisticInfoNumN)>=iPackageLen)    //判断长度,防止缓冲越界
        {
            return;
        }

        for(int i=0;i<statisticInfoNumN;i++)
        {
            memset(&statisticInfoBase,0x00,sizeof(AllPluseStatisticInfoBase));
            memcpy(&statisticInfoBase,buff+bufMemcpyPos,sizeof(AllPluseStatisticInfoBase));

            statisticInfo.iDataOutsideNo=d_ptr->dataOutsideNo;
            statisticInfo.iDataInsideNo=i;
            statisticInfo.allPluseStatisticInfoBase=statisticInfoBase;
            d_ptr->allPluseStatisticInfoList.append(statisticInfo);
            bufMemcpyPos+=sizeof(AllPluseStatisticInfoBase);
        }
    }

    //bufMemcpyPos+=sizeof(int);
    memcpy(&originalInfoEff,buff+bufMemcpyPos,sizeof(int));
    if(originalInfoEff==1) //原始信息有效
    {
        bufMemcpyPos+=sizeof(int);
        memcpy(&originalInfoNumM,buff+bufMemcpyPos,sizeof(int));

        bufMemcpyPos+=sizeof(int);
        memcpy(&attributeInfoNumL,buff+bufMemcpyPos,sizeof(int));

        if((bufMemcpyPos+68*attributeInfoNumL+8*attributeInfoNumL*originalInfoNumM)>=iPackageLen)    //判断长度,防止缓冲越界
        {
            return;
        }

        bufMemcpyPos+=sizeof(int);
        for(int l=0;l<attributeInfoNumL;l++)
        {
            int drawFlag; //是否绘图
            memset(&originalInfoBase,0x00,sizeof(AllPluseOriginalInfoAttributeBase));
            memcpy(originalInfoBase.arrOrgInfoAttributeName,(char*)buff+bufMemcpyPos,64);
            bufMemcpyPos+=64;
            memcpy(&drawFlag,buff+bufMemcpyPos,sizeof(int));
            originalInfoBase.iDrawFlag=drawFlag;
            bufMemcpyPos+=sizeof(int);
            for(int m=0;m<originalInfoNumM;m++)
            {
                originalInfo.allPluseOriginalInfoBase=originalInfoBase;
                originalInfo.iDataOutsideNo=d_ptr->dataOutsideNo;
                originalInfo.iDataInsideNo=l;
                originalInfoTempList.append(originalInfo);
            }
        }

        for(int m=0;m<originalInfoNumM;m++)
        {
            int move;   //偏移量,从包头到当前值
            double value;
            for(int ll=0;ll<attributeInfoNumL;ll++)
            {
               move=18+sizeof(AllPluseStatisticInfoBase)*statisticInfoNumN+12+68*attributeInfoNumL+sizeof(double)*m+ll;
               memcpy(&value,buff+move,sizeof(double));
               originalInfo=originalInfoTempList.at(m*attributeInfoNumL+ll);
               originalInfo.allPluseOriginalInfoBase.dValue=value;
               originalInfoTempList.replace(m*attributeInfoNumL+ll,originalInfo);
            }
        }

        for(int i=0;i<originalInfoTempList.size();i++)  //将原始数据信息存放到全局列表
        {
            originalInfo=originalInfoTempList.at(i);
            d_ptr->allPulseOriginalInfoList.append(originalInfo);
        }
    }

    setTableData();
    d_ptr->dataOutsideNo++;
}

/*!
 * \brief 设置表格数据
 */
void AllPluseTable::setTableData()
{
    if(d_ptr->showTableMode == RealTimeDisplay) //实时显示模式
    {
        d_ptr->dataViewModelStatictis->updateAllPluseStatisticInfoList(d_ptr->allPluseStatisticInfoList);
        int size=d_ptr->allPluseStatisticInfoList.size();
        if(size>0)
        {
            int outsideNo=d_ptr->allPluseStatisticInfoList.at(size-1).iDataOutsideNo;
            int insideNo=d_ptr->allPluseStatisticInfoList.at(size-1).iDataInsideNo;
            AllPulseOriginalInfoList originalList;
            AllPluseOriginalInfoAttribute originalInfo;
            for(int i=0;i<d_ptr->allPulseOriginalInfoList.size();i++)
            {
                if((d_ptr->allPulseOriginalInfoList.at(i).iDataOutsideNo==outsideNo)
                   &&(d_ptr->allPulseOriginalInfoList.at(i).iDataInsideNo==insideNo))
                {
                    originalInfo=d_ptr->allPulseOriginalInfoList.at(i);
                    originalList.append(originalInfo);
                }
            }
            d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
        }
    }
}

/*!
 * \brief 双击统计信息表格
 * \param index 信息
 */
void AllPluseTable::doubleClickedTable(QModelIndex index)
{
    int row=index.column();
    if(d_ptr->showTableMode == HistoricalDisplay) //历史显示模式
    {
        int size=d_ptr->allPluseStatisticInfoList.size();
        if(size>row)
        {
            int outsideNo=d_ptr->allPluseStatisticInfoList.at(row).iDataOutsideNo;
            int insideNo=d_ptr->allPluseStatisticInfoList.at(row).iDataInsideNo;
            AllPulseOriginalInfoList originalList;
            AllPluseOriginalInfoAttribute originalInfo;
            for(int i=0;i<d_ptr->allPulseOriginalInfoList.size();i++)
            {
                if((d_ptr->allPulseOriginalInfoList.at(i).iDataOutsideNo==outsideNo)
                   &&(d_ptr->allPulseOriginalInfoList.at(i).iDataInsideNo==insideNo))
                {
                    originalInfo=d_ptr->allPulseOriginalInfoList.at(i);
                    originalList.append(originalInfo);
                }
            }
            d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
        }
    }
}

/*!
 * @brief 清空表格
 */
void AllPluseTable::clearTable()
{
    d_ptr->allPluseStatisticInfoList.clear();
    d_ptr->allPulseOriginalInfoList.clear();

    d_ptr->dataViewModelStatictis->updateAllPluseStatisticInfoList(d_ptr->allPluseStatisticInfoList);
    d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(d_ptr->allPulseOriginalInfoList);
}

} //namespace DataView
