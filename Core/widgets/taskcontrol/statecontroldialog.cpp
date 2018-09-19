#include "statecontroldialog.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>

#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class StateControlDialogPrivate
{
    Q_DECLARE_PUBLIC(StateControlDialog)
private:
    StateControlDialogPrivate(StateControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    StateControlDialog * q_ptr;
    QWidget * mainWidget;

    DialogProxy::StandardButton clickedButt;

    QComboBox * workmode_cbox;                      /*!< 工作模式 */
    QLineEdit * workCyclesEdit;                     /*!< 工作周期 */
    QLineEdit * workCyclesNumber_le;                /*!< 工作周期数 */
    QComboBox * workingBandwidth_cbox;              /*!< 天线带宽 */
    QComboBox * antennaSelection1_cbox;             /*!< 天线选择1 */
    QComboBox * antennaSelection2_cbox;             /*!< 天线选择2 */

    QLineEdit * le_attenuationCode1;
    QLineEdit * le_attenuationCode_2;
    QLineEdit * le_attenuationCode_3;
    QLineEdit * le_attenuationCode_4;
    QLineEdit * le_attenuationCode_5;
    QLineEdit * le_attenuationCode_6;
    QLineEdit * le_attenuationCode_7;
    QLineEdit * le_attenuationCode_8;
    QLineEdit * le_attenuationCode_9;
    QLineEdit * le_attenuationCode_10;

    StateControl * modifyInfo;
};

void StateControlDialogPrivate::initView()
{
    mainWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(mainWidget);
    gridLayout->setHorizontalSpacing(6);
    gridLayout->setVerticalSpacing(8);
    gridLayout->setContentsMargins(0, 0, -1, 0);

    //工作模式
    QLabel * label = new QLabel(mainWidget);
    label->setText(QObject::tr("Work model"));
    label->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label, 0, 0, 1, 1);

    workmode_cbox = new QComboBox(mainWidget);
    workmode_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    workmode_cbox->setView(new QListView());

    QStringList stateList;
    stateList<<QObject::tr("Self-Test")<<QObject::tr("Frequency domain Search")<<QObject::tr("Reside")<<QObject::tr("Airspace Search")
            <<QObject::tr("Intermediate Frequency acquisition")<<QObject::tr("Enemy1030")<<QObject::tr("Enemy1090");
    workmode_cbox->addItems(stateList);

    //0自检  1频域搜索  2驻留  3空域搜索  4中频采集  5敌我1030  6敌我1090

    gridLayout->addWidget(workmode_cbox, 0, 1, 1, 1);

    //工作周期
    QLabel * label_2 = new QLabel(mainWidget);
    label_2->setText(QObject::tr("Work period"));
    label_2->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_2, 0, 2, 1, 1);

    workCyclesEdit = new QLineEdit(mainWidget);
    workCyclesEdit->setFixedSize(QSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT));
    gridLayout->addWidget(workCyclesEdit, 0, 3, 1, 1);

    //工作周期数
    QLabel * label_3 = new QLabel(mainWidget);
    label_3->setText(QObject::tr("Work periods"));
    label_3->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    workCyclesNumber_le = new QLineEdit(mainWidget);
    workCyclesNumber_le->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(workCyclesNumber_le, 1, 1, 1, 1);

    QLabel * label_4 = new QLabel(mainWidget);
    label_4->setText(QObject::tr("Bandwidth"));
    label_4->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_4, 1, 2, 1, 1);

    workingBandwidth_cbox = new QComboBox(mainWidget);
    workingBandwidth_cbox->setView(new QListView());
    workingBandwidth_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    QStringList bandList;
    bandList<<QString("400M")<<QString("200M")<<QString("40M")<<QString("10M")<<QString("5M");
    workingBandwidth_cbox->addItems(bandList);
    gridLayout->addWidget(workingBandwidth_cbox, 1, 3, 1, 1);

    //天线选择
    QLabel * label_5 = new QLabel(mainWidget);
    label_5->setText(QObject::tr("Antenna selection1"));
    label_5->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    antennaSelection1_cbox = new QComboBox(mainWidget);
    antennaSelection1_cbox->setView(new QListView());
    antennaSelection1_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    QStringList antennaList;
    antennaList <<QObject::tr("Front")<<QObject::tr("Back")<<QObject::tr("Left")<<QObject::tr("Right");
    antennaSelection1_cbox->addItems(antennaList);
    gridLayout->addWidget(antennaSelection1_cbox, 2, 1, 1, 1);

    QLabel * label_6 = new QLabel(mainWidget);
    label_6->setText(QObject::tr("Antenna selection2"));
    label_6->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_6->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_6, 2, 2, 1, 1);

    antennaSelection2_cbox = new QComboBox(mainWidget);
    antennaSelection2_cbox->setView(new QListView());
    antennaSelection2_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    antennaSelection2_cbox->addItems(antennaList);
    gridLayout->addWidget(antennaSelection2_cbox, 2, 3, 1, 1);

    //衰减码1-2
    QLabel * label_7 = new QLabel(mainWidget);
    label_7->setText(QObject::tr("Decay code1"));
    label_7->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_7->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_7, 3, 0, 1, 1);

    le_attenuationCode1 = new QLineEdit(mainWidget);
    le_attenuationCode1->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode1, 3, 1, 1, 1);

    QLabel * label_8 = new QLabel(mainWidget);
    label_8->setText(QObject::tr("Decay code2"));
    label_8->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_8->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_8, 3, 2, 1, 1);

    le_attenuationCode_2 = new QLineEdit(mainWidget);
    le_attenuationCode_2->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_2, 3, 3, 1, 1);

    //衰减码3-4
    QLabel * label_9 = new QLabel(mainWidget);
    label_9->setText(QObject::tr("Decay code3"));
    label_9->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_9->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    gridLayout->addWidget(label_9, 4, 0, 1, 1);

    le_attenuationCode_3 = new QLineEdit(mainWidget);
    le_attenuationCode_3->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_3, 4, 1, 1, 1);

    QLabel * label_10 = new QLabel(mainWidget);
    label_10->setText(QObject::tr("Decay code4"));
    label_10->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_10->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_10, 4, 2, 1, 1);

    le_attenuationCode_4 = new QLineEdit(mainWidget);
    le_attenuationCode_4->setObjectName(QStringLiteral("le_attenuationCode_4"));
    le_attenuationCode_4->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);

    gridLayout->addWidget(le_attenuationCode_4, 4, 3, 1, 1);

    //衰减码5-6
    QLabel * label_11 = new QLabel(mainWidget);
    label_11->setText(QObject::tr("Decay code5"));
    label_11->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_11->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_11, 5, 0, 1, 1);

    le_attenuationCode_5 = new QLineEdit(mainWidget);
    le_attenuationCode_5->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_5, 5, 1, 1, 1);

    QLabel * label_12 = new QLabel(mainWidget);
    label_12->setText(QObject::tr("Decay code6"));
    label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    gridLayout->addWidget(label_12, 5, 2, 1, 1);

    le_attenuationCode_6 = new QLineEdit(mainWidget);
    le_attenuationCode_6->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_6, 5, 3, 1, 1);

    //衰减码7-8
    QLabel * label_13 = new QLabel(mainWidget);
    label_13->setText(QObject::tr("Decay code7"));
    label_13->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_13->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_13, 6, 0, 1, 1);

    le_attenuationCode_7 = new QLineEdit(mainWidget);
    le_attenuationCode_7->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_7, 6, 1, 1, 1);

    QLabel * label_14 = new QLabel(mainWidget);
    label_14->setText(QObject::tr("Decay code8"));
    label_14->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_14->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_14, 6, 2, 1, 1);

    le_attenuationCode_8 = new QLineEdit(mainWidget);
    le_attenuationCode_8->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_8, 6, 3, 1, 1);

    //衰减码9-10
    QLabel * label_16 = new QLabel(mainWidget);
    label_16->setText(QObject::tr("Decay code9"));
    label_16->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_16->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_16, 7, 0, 1, 1);

    le_attenuationCode_9 = new QLineEdit(mainWidget);
    le_attenuationCode_9->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(le_attenuationCode_9, 7, 1, 1, 1);

    QLabel * label_17 = new QLabel(mainWidget);
    label_17->setText(QObject::tr("Decay code10"));
    label_17->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_17->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_17, 7, 2, 1, 1);

    le_attenuationCode_10 = new QLineEdit(mainWidget);
    le_attenuationCode_10->setObjectName(QStringLiteral("le_attenuationCode_10"));
    le_attenuationCode_10->setFixedSize(QSize(100, 20));
    le_attenuationCode_10->setFixedSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT);

    gridLayout->addWidget(le_attenuationCode_10, 7, 3, 1, 1);

    mainWidget->setLayout(gridLayout);
}

StateControlDialog::StateControlDialog(QWidget *parent)
    :d_ptr(new StateControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("State control"));
}

StateControlDialog::~StateControlDialog()
{

}


void StateControlDialog::setWindowData(StateControl * state)
{
    Q_D(StateControlDialog);
    d->modifyInfo = state;
    //工作模式
    d->workmode_cbox->setCurrentIndex(static_cast<int>(state->workmode));
    
    d->workCyclesEdit->setText(QString("%1").arg(static_cast<int>(state->workCycles)));
    d->workCyclesNumber_le->setText(QString("%1").arg(state->workCyclesNumber));
    
    //工作带宽
    d->workingBandwidth_cbox->setCurrentIndex(static_cast<int>(state->workingBandwidth));

    //天线选择1
    d->antennaSelection1_cbox->setCurrentIndex(static_cast<int>(state->antennaSelection1));
    
    //天线选择2
    d->antennaSelection2_cbox->setCurrentIndex(static_cast<int>(state->antennaSelection2));
    
    //衰减码
    d->le_attenuationCode1 ->setText(QString("%1").arg(state->attenuationCode1));
    d->le_attenuationCode_2->setText(QString("%1").arg(state->attenuationCode2));
    d->le_attenuationCode_3->setText(QString("%1").arg(state->attenuationCode3));
    d->le_attenuationCode_4->setText(QString("%1").arg(state->attenuationCode4));
    d->le_attenuationCode_5->setText(QString("%1").arg(state->attenuationCode5));
    d->le_attenuationCode_6->setText(QString("%1").arg(state->attenuationCode6));
    d->le_attenuationCode_7->setText(QString("%1").arg(state->attenuationCode7));
    d->le_attenuationCode_8->setText(QString("%1").arg(state->attenuationCode8));
    d->le_attenuationCode_9->setText(QString("%1").arg(state->attenuationCode9));
    d->le_attenuationCode_10->setText(QString("%1").arg(state->attenuationCode10));
}

StateControl * StateControlDialog::getWindowData()
{
    Q_D(StateControlDialog);
    
    if(d->clickedButt == DialogProxy::Ok){
        StateControl *state = NULL;
        if(d->modifyInfo)
            state = d->modifyInfo;
        else
            state = new StateControl;
        state->excuteTime = QDateTime::currentDateTime();
        state->lastTime = 1;

        state->workmode = d->workmode_cbox->currentIndex();
        state->workCycles = d->workCyclesEdit->text().toInt();
        state->workCyclesNumber = d->workCyclesNumber_le->text().toShort();
        state->workingBandwidth = d->workingBandwidth_cbox->currentIndex();
        state->antennaSelection1 = d->antennaSelection1_cbox->currentIndex();
        state->antennaSelection2 = d->antennaSelection2_cbox->currentIndex();

        state->attenuationCode1 = d->le_attenuationCode1->text().toFloat();
        state->attenuationCode2 = d->le_attenuationCode_2->text().toFloat();
        state->attenuationCode3 = d->le_attenuationCode_3->text().toFloat();
        state->attenuationCode4 = d->le_attenuationCode_4->text().toFloat();
        state->attenuationCode5 = d->le_attenuationCode_5->text().toFloat();
        state->attenuationCode6 = d->le_attenuationCode_6->text().toFloat();
        state->attenuationCode7 = d->le_attenuationCode_7->text().toFloat();
        state->attenuationCode8 = d->le_attenuationCode_8->text().toFloat();
        state->attenuationCode9 = d->le_attenuationCode_9->text().toFloat();
        state->attenuationCode10 = d->le_attenuationCode_10->text().toFloat();
        return state;
    }
    return NULL;
}

QSize StateControlDialog::sizeHint() const
{
    return QSize(615,410);
}

void StateControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(StateControlDialog);
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

void StateControlDialog::respOk()
{
    Q_D(StateControlDialog);
    RAndCombineValidator validator;
    validator.addValidator(new RNumericValidator<char>(d->workCyclesEdit->text().toInt(),RValid::Ge,0,RValid::Le,20));
    validator.addValidator(new RNumericValidator<short>(d->workCyclesNumber_le->text().toShort(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode1->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_2->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_3->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_4->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_5->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_6->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_7->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_8->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_9->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->le_attenuationCode_10->text().toFloat(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }
    respCancel();
}

void StateControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
