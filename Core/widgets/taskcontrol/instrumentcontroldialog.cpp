#include "instrumentcontroldialog.h"

#include <QListView>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class InstrumentControlDialogPrivate
{
    Q_DECLARE_PUBLIC(InstrumentControlDialog)
private:
    InstrumentControlDialogPrivate(InstrumentControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    InstrumentControlDialog * q_ptr;
    QWidget * mainWidget;
    DialogProxy::StandardButton clickedButt;

    QComboBox * type_cbox;                          /*!< 信号类型 */
    QComboBox * model_cbox;                         /*!< 仪器型号 */
    QComboBox * commType_cbox;                      /*!< 仪器通信类型 */

    QLineEdit * ipAddress_le;                       /*!< 仪器IP地址 */
    QLineEdit * networkPort_le;                     /*!< 网络端口 */
    QLineEdit * GPIBPort_le;                        /*!< GPIB端口 */

    QLineEdit * signalCarrierFrequency_le;          /*!< 信号载频 */
    QLineEdit * signalRepetitionPeriod_le;          /*!< 信号重复周期 */
    QLineEdit * signalPulseWidth_le;                /*!< 信号脉宽 */

    QLineEdit * signalPower_le;                     /*!< 信号功率 */
    QCheckBox * radioFrequencySwitchControl_check;  /*!< 射频开关控制 */
    QCheckBox * ImpulseSwitchControl_check;         /*!< 脉冲开关控制 */

    QLineEdit * centreFrequency_le;                 /*!< 中心频率 */
    QLineEdit * displayBandwidth_le;                /*!< 显示带宽 */
    QLineEdit * IFBandwidth_le;                     /*!< 中频带宽 */

    QLineEdit *  videoBandwidth_le;                 /*!< 视频带宽 */
    QLineEdit *  scanTime_le;                       /*!< 扫描时间 */
    QLineEdit *  scanPoints_le;                     /*!< 扫描点数 */

    QCheckBox * ifReadspectrumData_check;           /*!< 是否读取频谱数据 */
    QLineEdit * signalBase_le;                      /*!< 信号基底 */
    QCheckBox * powerCalibrationControl_check;      /*!< 功率校准 */

    QCheckBox * immediatePowerCalibration_check;    /*!< 功率立即校准 */
    QCheckBox * intrapulseSwitchControl_check;      /*!< 脉内调制开关 */

    InstrumentControl * modifyInfo;
};

void InstrumentControlDialogPrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout *gridLayout = new QGridLayout(mainWidget);

    //仪器类型
    QLabel * label_65 = new QLabel(mainWidget);
    label_65->setText(QStringLiteral("仪器类型"));
    label_65->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_65->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_65, 0, 0, 1, 1);

    type_cbox = new QComboBox(mainWidget);
    type_cbox->setView(new QListView());
    type_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    QStringList typeList;
    typeList<<QStringLiteral("信号源")<<QStringLiteral("频谱仪");
    type_cbox->addItems(typeList);
    gridLayout->addWidget(type_cbox, 0, 1, 1, 1);

    //仪器型号
    QLabel * label_58 = new QLabel(mainWidget);
    label_58->setText(QStringLiteral("仪器型号"));
    label_58->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_58->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_58, 0, 2, 1, 1);

    model_cbox = new QComboBox(mainWidget);
    model_cbox->setView(new QListView());
    model_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    QStringList modelList;
    modelList<<QStringLiteral("安捷伦")<<QStringLiteral("安立")<<QStringLiteral("罗德斯瓦兹");
    model_cbox->addItems(modelList);
    gridLayout->addWidget(model_cbox, 0, 3, 1, 1);

    //仪器通信类型
    QLabel *label_57 = new QLabel(mainWidget);
    label_57->setText(QStringLiteral("仪器通信类型"));
    label_57->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_57->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_57, 0, 4, 1, 1);

    commType_cbox = new QComboBox(mainWidget);
    commType_cbox->setView(new QListView());
    commType_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    QStringList commList;
    commList<<QStringLiteral("网络")<<QStringLiteral("GPIB");
    commType_cbox->addItems(commList);
    gridLayout->addWidget(commType_cbox, 0, 5, 1, 1);

    //仪器IP地址
    QLabel * label_64 = new QLabel(mainWidget);
    label_64->setText(QStringLiteral("仪器IP地址"));
    label_64->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_64->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_64, 1, 0, 1, 1);

    ipAddress_le = new QLineEdit(mainWidget);
    ipAddress_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(ipAddress_le, 1, 1, 1, 1);

    //网络端口
    QLabel * label_51 = new QLabel(mainWidget);
    label_51->setText(QStringLiteral("网络端口"));
    label_51->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_51->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_51, 1, 2, 1, 1);

    networkPort_le = new QLineEdit(mainWidget);
    networkPort_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(networkPort_le, 1, 3, 1, 1);

    //GPIB端口
    QLabel * label_50 = new QLabel(mainWidget);
    label_50->setText(QStringLiteral("GPIB端口"));
    label_50->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_50->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_50, 1, 4, 1, 1);

    GPIBPort_le = new QLineEdit(mainWidget);
    GPIBPort_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(GPIBPort_le, 1, 5, 1, 1);

    //信号载频
    QLabel * label_66 = new QLabel(mainWidget);
    label_66->setText(QStringLiteral("信号载频(MHZ)"));
    label_66->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_66->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_66, 2, 0, 1, 1);

    signalCarrierFrequency_le = new QLineEdit(mainWidget);
    signalCarrierFrequency_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(signalCarrierFrequency_le, 2, 1, 1, 1);

    //信号重复周期
    QLabel * label_67 = new QLabel(mainWidget);
    label_67->setText(QStringLiteral("信号重复周期(us)"));
    label_67->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_67->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_67, 2, 2, 1, 1);

    signalRepetitionPeriod_le = new QLineEdit(mainWidget);
    signalRepetitionPeriod_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(signalRepetitionPeriod_le, 2, 3, 1, 1);

    //信号脉宽
    QLabel * label_68 = new QLabel(mainWidget);
    label_68->setText(QStringLiteral("信号脉宽(us)"));
    label_68->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_68->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_68, 2, 4, 1, 1);

    signalPulseWidth_le = new QLineEdit(mainWidget);
    signalPulseWidth_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(signalPulseWidth_le, 2, 5, 1, 1);

    //信号功率
    QLabel * label_70 = new QLabel(mainWidget);
    label_70->setText(QStringLiteral("信号功率(dBm)"));
    label_70->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_70->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_70, 3, 0, 1, 1);

    signalPower_le = new QLineEdit(mainWidget);
    signalPower_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(signalPower_le, 3, 1, 1, 1);

    //射频开关控制
    QLabel * label_71 = new QLabel(mainWidget);
    label_71->setText(QStringLiteral("射频开关控制"));
    label_71->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_71->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_71, 3, 2, 1, 1);

    radioFrequencySwitchControl_check = new QCheckBox(mainWidget);
    radioFrequencySwitchControl_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(radioFrequencySwitchControl_check, 3, 3, 1, 1);

    //脉冲开关控制
    QLabel * label_72 = new QLabel(mainWidget);
    label_72->setText(QStringLiteral("脉冲开关控制"));
    label_72->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_72->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_72, 3, 4, 1, 1);

    ImpulseSwitchControl_check = new QCheckBox(mainWidget);
    ImpulseSwitchControl_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(ImpulseSwitchControl_check, 3, 5, 1, 1);

    //中心频率
    QLabel * label_62 = new QLabel(mainWidget);
    label_62->setText(QStringLiteral("中心频率(MHz)"));
    label_62->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_62->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_62, 4, 0, 1, 1);

    centreFrequency_le = new QLineEdit(mainWidget);
    centreFrequency_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(centreFrequency_le, 4, 1, 1, 1);

    //显示带宽
    QLabel * label_59 = new QLabel(mainWidget);
    label_59->setText(QStringLiteral("显示带宽(MHz)"));
    label_59->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_59->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_59, 4, 2, 1, 1);

    displayBandwidth_le = new QLineEdit(mainWidget);
    displayBandwidth_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(displayBandwidth_le, 4, 3, 1, 1);

    //中频带宽
    QLabel * label_63 = new QLabel(mainWidget);
    label_63->setText(QStringLiteral("中频带宽(MHz)"));
    label_63->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_63->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_63, 4, 4, 1, 1);

    IFBandwidth_le = new QLineEdit(mainWidget);
    IFBandwidth_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(IFBandwidth_le, 4, 5, 1, 1);

    //视频带宽
    QLabel * label_60 = new QLabel(mainWidget);
    label_60->setText(QStringLiteral("视频带宽(MHz)"));
    label_60->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_60->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_60, 5, 0, 1, 1);

    videoBandwidth_le = new QLineEdit(mainWidget);
    videoBandwidth_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(videoBandwidth_le, 5, 1, 1, 1);

    //扫描时间
    QLabel * label_55 = new QLabel(mainWidget);
    label_55->setText(QStringLiteral("扫描时间(ms)"));
    label_55->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_55->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_55, 5, 2, 1, 1);

    scanTime_le = new QLineEdit(mainWidget);
    scanTime_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(scanTime_le, 5, 3, 1, 1);

    //扫描点数
    QLabel * label_52 = new QLabel(mainWidget);
    label_52->setText(QStringLiteral("扫描点数"));
    label_52->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_52->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_52, 5, 4, 1, 1);

    scanPoints_le = new QLineEdit(mainWidget);
    scanPoints_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(scanPoints_le, 5, 5, 1, 1);

    //是否读取频谱数据
    QLabel * label_61 = new QLabel(mainWidget);
    label_61->setText(QStringLiteral("是否读取频谱数据"));
    label_61->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_61->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_61, 6, 0, 1, 1);

    ifReadspectrumData_check = new QCheckBox(mainWidget);
    ifReadspectrumData_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(ifReadspectrumData_check, 6, 1, 1, 1);

    //信号基底
    QLabel * label_53 = new QLabel(mainWidget);
    label_53->setText(QStringLiteral("信号基底(dBm)"));
    label_53->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_53->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_53, 6, 2, 1, 1);

    signalBase_le = new QLineEdit(mainWidget);
    signalBase_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(signalBase_le, 6, 3, 1, 1);

    //功率校准控制
    QLabel * label_54 = new QLabel(mainWidget);
    label_54->setText(QStringLiteral("功率校准控制"));
    label_54->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_54->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_54, 6, 4, 1, 1);

    powerCalibrationControl_check = new QCheckBox(mainWidget);
    powerCalibrationControl_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(powerCalibrationControl_check, 6, 5, 1, 1);

    //功率立即校准
    QLabel * label_56 = new QLabel(mainWidget);
    label_56->setText(QStringLiteral("功率立即校准"));
    label_56->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_56->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_56, 7, 0, 1, 1);

    immediatePowerCalibration_check = new QCheckBox(mainWidget);
    immediatePowerCalibration_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(immediatePowerCalibration_check, 7, 1, 1, 1);

    //脉内调制
    QLabel * label_69 = new QLabel(mainWidget);
    label_69->setText(QStringLiteral("脉内调制"));
    label_69->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_69->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_69, 7, 2, 1, 1);

    intrapulseSwitchControl_check = new QCheckBox(mainWidget);
    intrapulseSwitchControl_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(intrapulseSwitchControl_check, 7, 3, 1, 1);

    mainWidget->setLayout(gridLayout);
}

InstrumentControlDialog::InstrumentControlDialog(QWidget *parent)
    :d_ptr(new InstrumentControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Instrument Control"));
}

InstrumentControlDialog::~InstrumentControlDialog()
{

}

void InstrumentControlDialog::setWindowData(InstrumentControl * instrument)
{
    Q_D(InstrumentControlDialog);
    d->modifyInfo = instrument;
    //仪器类型
    d->type_cbox->setCurrentIndex(instrument->instrumentType);
    //仪器型号
    d->model_cbox->setCurrentIndex(instrument->instrumentModel);
    //仪器通信类型
    d->commType_cbox->setCurrentIndex(instrument->instrumentCommType);
    
    //仪器IP地址
    d->ipAddress_le->setText(QString::fromLocal8Bit(instrument->IPAddress));
    //网络端口
    d->networkPort_le->setText(QString("%1").arg(instrument->networkPort));
    //GPIB端口
    d->GPIBPort_le->setText(QString("%1").arg(instrument->GPIBPort));
    //信号载频
    d->signalCarrierFrequency_le->setText(QString("%1").arg(instrument->signalCarrierFrequency));
    //信号重复周期
    d->signalRepetitionPeriod_le->setText(QString("%1").arg(instrument->signalRepetitionPeriod));
    //信号脉宽
    d->signalPulseWidth_le->setText(QString("%1").arg(instrument->signalPulseWidth));
    //信号功率
    d->signalPower_le->setText(QString("%1").arg(instrument->signalPower));
    //射频开关控制
    if (instrument->radioFrequencySwitchControl == 1)
        d->radioFrequencySwitchControl_check->setChecked(true);
    //脉内调制开关控制
    if (instrument->intrapulseSwitchControl == 1)
        d->intrapulseSwitchControl_check->setChecked(true);
    //脉冲开关控制
    if (instrument->ImpulseSwitchControl == 1)
        d->ImpulseSwitchControl_check->setChecked(true);
    //中心频率
    d->centreFrequency_le->setText(QString("%1").arg(instrument->centreFrequency));
    //显示带宽
    d->displayBandwidth_le->setText(QString("%1").arg(instrument->displayBandwidth));
    //中频带宽
    d->IFBandwidth_le->setText(QString("%1").arg(instrument->IFBandwidth));
    //视频带宽
    d->videoBandwidth_le->setText(QString("%1").arg(instrument->videoBandwidth));
    //扫描时间
    d->scanTime_le->setText(QString("%1").arg(instrument->scanTime));
    //扫描点数
    d->scanPoints_le->setText(QString("%1").arg(instrument->scanPoints));
    //是否读取频谱数据
    if (instrument->ifReadspectrumData == 1)
        d->ifReadspectrumData_check->setChecked(true);
    //信号基底
    d->signalBase_le->setText(QString("%1").arg(instrument->signalBase));
    //功率校准控制
    if (instrument->powerCalibrationControl == 1)
        d->powerCalibrationControl_check->setChecked(true);
    //功率立即校准
    if (instrument->immediatePowerCalibration == 1)
        d->immediatePowerCalibration_check->setChecked(true);
}


InstrumentControl *InstrumentControlDialog::getWindowData()
{
    Q_D(InstrumentControlDialog);
    if(d->clickedButt == DialogProxy::Ok){
        InstrumentControl * instrument = NULL;
        if(d->modifyInfo)
            instrument = d->modifyInfo;
        else
            instrument = new InstrumentControl;

        instrument->excuteTime = QDateTime::currentDateTime();
        instrument->lastTime = 1;

        //仪器类型
        instrument->instrumentType = d->type_cbox->currentIndex();
        //仪器型号
        instrument->instrumentModel = d->model_cbox->currentIndex();
        //仪器通信类型
        instrument->instrumentCommType = d->commType_cbox->currentIndex();
        //仪器IP地址
        QByteArray byte = d->ipAddress_le->text().toLocal8Bit();
        strcpy(instrument->IPAddress, byte);
        //网络端口
        instrument->networkPort = d->networkPort_le->text().toShort();
        //GPIB端口
        instrument->GPIBPort = d->GPIBPort_le->text().toShort();
        //信号载频
        instrument->signalCarrierFrequency = d->signalCarrierFrequency_le->text().toDouble();
        //信号重复周期
        instrument->signalRepetitionPeriod = d->signalRepetitionPeriod_le->text().toDouble();
        //信号脉宽
        instrument->signalPulseWidth = d->signalPulseWidth_le->text().toDouble();
        //信号功率
        instrument->signalPower = d->signalPower_le->text().toDouble();

        //射频开关控制
        if (d->radioFrequencySwitchControl_check->checkState() == Qt::Checked)
            instrument->radioFrequencySwitchControl = 1;

        //脉内调制开关控制
        if (d->intrapulseSwitchControl_check->checkState() == Qt::Checked)
            instrument->intrapulseSwitchControl = 1;

        //脉冲开关控制
        if (d->ImpulseSwitchControl_check->checkState() == Qt::Checked)
            instrument->ImpulseSwitchControl = 1;

        //中心频率
        instrument->centreFrequency = d->centreFrequency_le->text().toDouble();
        //显示带宽
        instrument->displayBandwidth = d->displayBandwidth_le->text().toDouble();
        //中频带宽
        instrument->IFBandwidth = d->IFBandwidth_le->text().toDouble();
        //视频带宽
        instrument->videoBandwidth = d->videoBandwidth_le->text().toDouble();
        //扫描时间
        instrument->scanTime = d->scanTime_le->text().toInt();
        //扫描点数
        instrument->scanPoints = d->scanPoints_le->text().toInt();
        //是否读取频谱数据
        if (d->ifReadspectrumData_check->checkState() == Qt::Checked)
            instrument->ifReadspectrumData = 1;

        //信号基底
        instrument->signalBase = d->signalBase_le->text().toDouble();

        //功率校准控制
        if (d->powerCalibrationControl_check->checkState() == Qt::Checked)
            instrument->powerCalibrationControl = 1;

        //功率立即校准
        if (d->immediatePowerCalibration_check->checkState() == Qt::Checked)
            instrument->immediatePowerCalibration = 1;

        return instrument;
    }
    return NULL;
}

QSize InstrumentControlDialog::sizeHint() const
{
    return QSize(700,500);
}

void InstrumentControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(InstrumentControlDialog);
    d->clickedButt = butt;
    switch(butt){
        case DialogProxy::Ok:
                respOk();
            break;
        case DialogProxy::Cancel:
                respCancel();
            break;
        default:
            break;
    }
}

void InstrumentControlDialog::respOk()
{
    Q_D(InstrumentControlDialog);

    RAndCombineValidator validator;
    validator.addValidator(new RStringValidator(d->ipAddress_le->text(),RValid::R_IP));
    validator.addValidator(new RNumericValidator<short>(d->networkPort_le->text().toShort(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<short>(d->GPIBPort_le->text().toShort(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->signalCarrierFrequency_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->signalRepetitionPeriod_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->signalPulseWidth_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->signalPower_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->centreFrequency_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->displayBandwidth_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->IFBandwidth_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->videoBandwidth_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<int>(d->scanTime_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<int>(d->scanPoints_le->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->signalBase_le->text().toDouble(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }
    respCancel();
}

void InstrumentControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
