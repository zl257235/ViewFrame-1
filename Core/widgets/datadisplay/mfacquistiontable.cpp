#include "mfacquistiontable.h"

#include <QVBoxLayout>
#include <QHeaderView>

#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "table.h"

namespace DataView {

class MFAcquistionTablePrivate
{
    Q_DECLARE_PUBLIC(MFAcquistionTable)
public:
    explicit MFAcquistionTablePrivate(MFAcquistionTable * q):q_ptr(q)
    {
        initView();
    }
    void initView();

    MFAcquistionTable * q_ptr;

    QWidget * mainWidget;
    Table *mfTable;
};

void MFAcquistionTablePrivate::initView()
{
    mainWidget = new QWidget();

    mfTable = new Table(mainWidget);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(mfTable);

    mainWidget->setLayout(vlayout);
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
    for(int i=0;i<d->mfTable->rowCount();i++)
    {
        QString strValue=QString("%1").arg(i+1);
        d->mfTable->addRowValue(i,0,strValue);
    }

    QStringList headInfo;
    int colCount=6;

    headInfo<<QStringLiteral("序号")<<QStringLiteral("采集时间")<<QStringLiteral("采集模式")<<QStringLiteral("采集脉冲个数")<<QStringLiteral("均值")
                    <<QStringLiteral("采集点数");

    d->mfTable->setColumnValue(colCount, headInfo);
    d->mfTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int row=40;
    d->mfTable->addRowValue(30,row,true);
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

}

} //namespace DataView
