#include "radiationsourcetable.h"
#include "ui_radiationsourcetable.h"

#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QHBoxLayout>

#include "table.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"

namespace DataView {

const int INIT_TABLE_COL_NUM =50;                                   //表格初始化为50列
const int INIT_TABLE_ROW_NUM=40;                                    //表格初始化为20行
const int RADIASOURCE_START_POINT=140;                              //数据源基信息起始位置

class RadiationSourceTablePrivate
{
    Q_DECLARE_PUBLIC(RadiationSourceTable)
public:
    explicit RadiationSourceTablePrivate(RadiationSourceTable * q):q_ptr(q),enRSReKind(RadiationSourceTable::SCROLL_RENOVATE),
        m_currentRowToInsert(-1)
    {
        initView();
    }
    void initView();
    void intRSTable();

    RadiationSourceTable * q_ptr;

    QWidget * mainWidget;

    QRadioButton * radioButtonCoverReno;
    QRadioButton * radioButtonScrollReno;

    RadiationSourceTable::DataRefreshModel enRSReKind;                //刷新方式
    Table * rsTable;
    QMap<int,RadiationSourceRenovate> rsReMap;          //覆盖刷新下的map,key：数据源批号 value:数据源的具体信息
    int m_currentRowToInsert;                           //当前表格总行数
};

void RadiationSourceTablePrivate::initView()
{
    mainWidget = new QWidget();

    QWidget * radioWidget = new QWidget();
    radioButtonCoverReno = new QRadioButton(radioWidget);
    radioButtonScrollReno = new QRadioButton(radioWidget);

    QObject::connect(radioButtonCoverReno,SIGNAL(clicked()),q_ptr,SLOT(on_radioButtonCoverReno_clicked()));
    QObject::connect(radioButtonScrollReno,SIGNAL(clicked()),q_ptr,SLOT(on_radioButtonScrollReno_clicked()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(radioButtonCoverReno);
    layout->addWidget(radioButtonScrollReno);

    radioWidget->setLayout(layout);

    rsTable = new Table(mainWidget);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);
    vlayout->addWidget(rsTable);

    mainWidget->setLayout(vlayout);
}

RadiationSourceTable::RadiationSourceTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_RADIATION_SOURCE_TABLE,parent),d_ptr(new RadiationSourceTablePrivate(this))
{
    initRSTable();
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

RadiationSourceTable::~RadiationSourceTable()
{

}

bool RadiationSourceTable::initialize()
{
    Q_D(RadiationSourceTable);
    setWidget(d->mainWidget);
    return true;
}

void RadiationSourceTable::release()
{

}

void RadiationSourceTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

/*!
 * @brief 初始化数据源表格
 */
void RadiationSourceTable::initRSTable()
{
    Q_D(RadiationSourceTable);
    changeTableHeaderInfo(false);
    d->rsTable->setHeaderWidth(0,60);
    d->rsTable->addRowValue(30,INIT_TABLE_ROW_NUM,true);
}

/*!
 * @brief 滚动刷新单选框
 */
void RadiationSourceTable::on_radioButtonScrollReno_clicked()
{
    Q_D(RadiationSourceTable);
    if(d->enRSReKind!=SCROLL_RENOVATE)
    {
        d->enRSReKind=SCROLL_RENOVATE;
        changeTableHeaderInfo(false);
        d->m_currentRowToInsert=0;
    }
}

/*!
 * @brief 覆盖刷新单选框
 */
void RadiationSourceTable::on_radioButtonCoverReno_clicked()
{
    Q_D(RadiationSourceTable);
    if(d->enRSReKind!=COVER_RENOVATE)
    {
        d->enRSReKind=COVER_RENOVATE;
        changeTableHeaderInfo(true);
    }
}

/*!
 * @brief 改变表格头部信息以及列数
 * @param[in] blAddColFlag 是否新增列
 */
void RadiationSourceTable::changeTableHeaderInfo(bool blAddColFlag)
{
    Q_D(RadiationSourceTable);
    d->rsTable->clear();
    for(int i=0;i<d->rsTable->rowCount();i++)
    {
        QString strValue=QString("%1").arg(i+1);           //行号
        d->rsTable->addRowValue(i,0,strValue);
    }

    QStringList headInfo;
    int colCount=INIT_TABLE_COL_NUM;    //列数
    headInfo<<QStringLiteral("序号")<<QStringLiteral("辐射源批号")<<QStringLiteral("脉间类型")<<QStringLiteral("脉内类型")<<QStringLiteral("载频个数")
               <<QStringLiteral("连续波标记")<<QStringLiteral("载频脉组内脉冲数")<<QStringLiteral("载频频段码")<<QStringLiteral("RF1")<<QStringLiteral("RF2")
                <<QStringLiteral("RF3")<<QStringLiteral("RF4")<<QStringLiteral("RF5")<<QStringLiteral("RF6")<<QStringLiteral("RF7")
                  <<QStringLiteral("RF8")<<QStringLiteral("重频类型")<<QStringLiteral("重频个数")<<QStringLiteral("重频脉组内脉冲数")<<QStringLiteral("PRI1")
                    <<QStringLiteral("PRI2")<<QStringLiteral("PRI3")<<QStringLiteral("PRI4")<<QStringLiteral("PRI5")<<QStringLiteral("PRI6")
                      <<QStringLiteral("PRI7")<<QStringLiteral("PRI8")<<QStringLiteral("脉宽类型")<<QStringLiteral("脉宽个数")<<QStringLiteral("脉宽脉组内脉冲数")
                        <<QStringLiteral("PW1")<<QStringLiteral("PW2")<<QStringLiteral("PW3")<<QStringLiteral("PW4")<<QStringLiteral("PW5")
                          <<QStringLiteral("PW6")<<QStringLiteral("PW7")<<QStringLiteral("PW8")<<QStringLiteral("数字幅度")<<QStringLiteral("模拟幅度")
                            <<QStringLiteral("数字功率")<<QStringLiteral("模拟功率")<<QStringLiteral("方位角")<<QStringLiteral("俯仰角")<<QStringLiteral("经度")
                              <<QStringLiteral("纬度")<<QStringLiteral("高度")<<QStringLiteral("脉内有效标识")<<QStringLiteral("脉内特征信息")<<QStringLiteral("CRC校验");


    if(blAddColFlag) //新增两列
    {
        headInfo<<QStringLiteral("截获次数")<<QStringLiteral("截获时间");
        colCount+=2;
        d->rsTable->insertColumn(colCount);
    }

    d->rsTable->setColumnValue(colCount, headInfo);
}

/*!
 * @brief 接收辐射源
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void RadiationSourceTable::recvRSPara(char *buff, int len)
{
    Q_D(RadiationSourceTable);
    if((NULL==buff)||(len<=RADIASOURCE_START_POINT)) //小于辐射源首部字段固定长度
    {
        return;
    }

    int rsNum;
    RadiationSourceBase rsData;
    //rsList.clear();
    memcpy(&rsNum,buff+RADIASOURCE_START_POINT-4,4);
    for(int i=0;i<rsNum;i++)
    {
        memcpy(&rsData,buff+RADIASOURCE_START_POINT,sizeof(RadiationSourceBase));
        if(d->enRSReKind==COVER_RENOVATE) //覆盖刷新模式
        {
            int num=rsData.usSourceNo; //数据源批号
            RadiationSourceRenovate rsReData;

            bool blExist=false;
            if(!d->rsReMap.isEmpty())
            {
                QMap<int,RadiationSourceRenovate>::Iterator iter;
                iter=d->rsReMap.find(num);
                if(iter!=d->rsReMap.end())
                {
                    blExist=true;
                    rsReData=iter.value();
                    rsReData.iHoldCount++;
                    rsReData.strHoldTime=getCurrentDate();
                    rsReData.rsInfo=rsData;
                    d->rsReMap[num]=rsReData;  //重新赋值
                }
            }

            if(!blExist)        //新增key
            {
                rsReData.iHoldCount=1;
                rsReData.strHoldTime=getCurrentDate();
                rsReData.rsInfo=rsData;
                rsReData.iInsertRow=d->rsReMap.size();
                d->rsReMap.insert(num,rsReData);
            }
            rsReData.rsInfo=rsData;
        }
        showTSPara(rsData);
        //rsList.append(rsData);
    }
}

/*!
 * @brief 显示辐射源信息
 * @param[in] rsData 辐射源信息
 */
void RadiationSourceTable::showTSPara(const RadiationSourceBase& rsData)
{
    Q_D(RadiationSourceTable);
    int col=0;
    int row=0;
    QString strValue;
    if((rsData.usStartCode!=0x55AA)||(rsData.usEndCode!=0x55AA))
    {
        return;
    }

    if(d->enRSReKind==COVER_RENOVATE) //覆盖刷新模式
    {
        row=d->rsReMap[rsData.usSourceNo].iInsertRow;
    }
    else
    {
        row=d->m_currentRowToInsert;
        d->m_currentRowToInsert++;
    }

    strValue=QString("%1").arg(row+1);                                //行号
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usSourceNo);                  //辐射源序号
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucPulseType);                 //脉间类型
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucIntraPulseType);            //脉内类型
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucCarrierCount);              //载频,个数
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucContinuousWaveLabeling);    //载频,连续波标记,0 无效 1 有效
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usPulseGroupInCount);         //载频,脉组内脉冲数
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usFrequencyBandCode);         //载频,频段码
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF1);        //载频,RF1
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF2);        //载频,RF2
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF3);        //载频,RF3
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF4);        //载频,RF4
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF5);        //载频,RF5
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF6);        //载频,RF6
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF7);        //载频,RF7
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dRF8);        //载频,RF8
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usMultiPluseType);            //重频,类型,0 固定,1 抖动,2 滑变,3 成组参差,4 固定参差
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usMultiPluseCount);           //重频,个数
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ulMultiPulseGroupInCount);    //重频,脉组内脉冲数
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI1);        //重频,周期PRI1
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI2);        //重频,周期PRI2
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI3);        //重频,周期PRI3
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI4);        //重频,周期PRI4
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI5);        //重频,周期PRI5
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI6);        //重频,周期PRI6
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI7);        //重频,周期PRI7
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPRI8);        //重频,周期PRI8
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usPluseWidthType);                //脉宽,类型,0 固定,1 变化,0xff 未知
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usPluseWidthCount);               //脉宽,个数,≤8
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ulPluseWidthGroupInCount);        //脉宽,脉组内脉冲数
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW1);        //脉宽,PW1
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW2);        //脉宽,PW2
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW3);        //脉宽,PW3
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW4);        //脉宽,PW4
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW5);        //脉宽,PW5
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW6);        //脉宽,PW6
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW7);        //脉宽,PW7
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dPW8);        //脉宽,PW8
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucDigitalPA);             //数字幅度,0-255
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ucAnalogPA);              //模拟幅度,0-255
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dDigitalPower);           //数字功率,单位:dBm,大于9999无效
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dAnalogPower);            //模拟功率,单位:dBm,大于9999无效
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dAziAngle);               //测量信息,方位角
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dEleAngle);               //测量信息,俯仰角
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dLon);                    //定位结果,经度
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dLat);                    //定位结果,纬度
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.dHight);                  //定位结果,高度
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.ulIntraPulseEffFlag);     //脉内调制信息,脉内有效标识,1 有效,0 无效
    d->rsTable->addRowValue(row,col++,strValue);

    char *pBuff=new char[32];
    memcpy(pBuff,rsData.ucIntraPulseInfo,32);
    strValue=QString(QLatin1String(pBuff));                     //脉内调制信息,脉内特征信息
    d->rsTable->addRowValue(row,col++,strValue);

    strValue=QString("%1").arg(rsData.usCRC);                   //CRC校验
    d->rsTable->addRowValue(row,col++,strValue);

    if(d->enRSReKind==COVER_RENOVATE) //覆盖刷新模式
    {
        row=d->rsReMap[rsData.usSourceNo].iInsertRow;

        strValue=QString("%1").arg(d->rsReMap[rsData.usSourceNo].iHoldCount);        //截获次数
        d->rsTable->addRowValue(row,col++,strValue);

        strValue=d->rsReMap[rsData.usSourceNo].strHoldTime;                          //截获时间
        d->rsTable->addRowValue(row,col++,strValue);
    }
}

/*!
 * @brief 获取当前时间
 */
QString RadiationSourceTable::getCurrentDate()
{
    QString strDate;
    QDateTime date=QDateTime::currentDateTime();
    strDate=date.toString("yyyy.MM.dd hh:mm:ss.zzz");
    return strDate;
}

void RadiationSourceTable::retranslateUi()
{
    Q_D(RadiationSourceTable);
    m_name = tr("Data source Map");
    setWindowTitle(m_name);

    d->radioButtonCoverReno->setText(tr("Overwrite Refresh"));
    d->radioButtonScrollReno->setText(tr("Scrolling Refresh"));
}

} //namespace DataView
