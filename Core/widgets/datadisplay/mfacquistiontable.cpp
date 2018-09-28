#include "mfacquistiontable.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QAction>
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"

namespace DataView {

class MFAcquistionTablePrivate
{
    Q_DECLARE_PUBLIC(MFAcquistionTable)
public:
    explicit MFAcquistionTablePrivate(MFAcquistionTable * q):q_ptr(q)
    {
        initView();
        initTableViewMenu();
    }
    void initView();
    void initTableViewMenu();

    MFAcquistionTable * q_ptr;

    QWidget * mainWidget;
    TableView* dataView;
    TableViewModelCustom* dataViewModel;
    MFAcquistionInfoList mfAcquistionInfoList;  //中频采集数据列表
    QAction *clearAction;
};

void MFAcquistionTablePrivate::initView()
{
    mainWidget = new QWidget();

    QVBoxLayout * vlayout = new QVBoxLayout;

    dataView=new TableView(q_ptr);
    dataView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModel=new TableViewModelCustom(dataView);
    dataView->setModel(dataViewModel);
    vlayout->addWidget(dataView);
    dataViewModel->setTableCustomKind(MF_ACQUISITION_INFO);

    mainWidget->setLayout(vlayout);
}

void MFAcquistionTablePrivate::initTableViewMenu()
{
    clearAction = new QAction();
    dataView->addAction(clearAction);
    QObject::connect(clearAction, SIGNAL(triggered(bool)), q_ptr, SLOT(clearTable()));
    clearAction->setText(QObject::tr("Clear Table"));
}

MFAcquistionTable::MFAcquistionTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_MF_ACQUISTION_TABLE,parent),d_ptr(new MFAcquistionTablePrivate(this))
{
    initMFAcquistionTable();
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

MFAcquistionTable::~MFAcquistionTable()
{

}

bool MFAcquistionTable::initialize()
{
    Q_D(MFAcquistionTable);
    setWidget(d->mainWidget);

    return true;
}

void MFAcquistionTable::release()
{

}

void MFAcquistionTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void MFAcquistionTable::initMFAcquistionTable()
{
    Q_D(MFAcquistionTable);

    QStringList headInfo;

    headInfo<<QStringLiteral("序号")<<QStringLiteral("采集时间")<<QStringLiteral("采集模式")<<QStringLiteral("采集脉冲个数")
                    <<QStringLiteral("采集点数");
    d_ptr->dataViewModel->resetHeadInfo(headInfo);
    d_ptr->dataView->setColumnWidth(0,90);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataView->setColumnWidth(i,130);
    }

}

void MFAcquistionTable::retranslateUi()
{
    m_name = tr("Intermediate Frequency Data List");
    setWindowTitle(m_name);
}

/*!
 * @brief 接收中频数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void MFAcquistionTable::recvMFAcquistionPara(char *buff, int len)
{
    MFAcquisitionInfo mfAcquisitionInfo;
    short mfFlag;               //中频数据标识
    int bufMemcpyPos=0;         //缓冲位置

    if(len<20)
    {
        return;
    }
    memcpy(&mfFlag,buff+2,sizeof(short));
    if(mfFlag!=0xBBBB)
    {
        return;
    }
    memcpy(&(mfAcquisitionInfo.sAcqModel),buff+6,sizeof(short));
    memcpy(&(mfAcquisitionInfo.iAcqNum),buff+8,sizeof(int));  //个数

    char* pBuff=buff+24;
    if(mfAcquisitionInfo.iAcqNum>0)
    {
        while((*pBuff!=0xCC)&&(*(pBuff+1)!=0xBB)&&(*(pBuff+2)!=0xAA))
        {
            bufMemcpyPos++;
            pBuff++;
            if(bufMemcpyPos>=(len-20))
            {
               break;
            }
        }
        mfAcquisitionInfo.iAcqDotNum=bufMemcpyPos;
    }
    else
    {
        mfAcquisitionInfo.iAcqDotNum=0;
    }
    mfAcquisitionInfo.strAcqTime=getCurrentDate();
    d_ptr->mfAcquistionInfoList.append(mfAcquisitionInfo);
    d_ptr->dataViewModel->updateMFAcquistionInfoList(d_ptr->mfAcquistionInfoList);
}

/*!
 * @brief 获取当前时间
 */
QString MFAcquistionTable::getCurrentDate()
{
    QString strDate;
    QDateTime date=QDateTime::currentDateTime();
    strDate=date.toString("yyyy.MM.dd hh:mm:ss.zzz");
    return strDate;
}

/*!
 * @brief 清空表格
 */
void MFAcquistionTable::clearTable()
{
    d_ptr->mfAcquistionInfoList.clear();
    d_ptr->dataViewModel->updateMFAcquistionInfoList(d_ptr->mfAcquistionInfoList);
}

} //namespace DataView
