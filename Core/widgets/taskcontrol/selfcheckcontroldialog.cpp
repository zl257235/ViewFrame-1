#include "selfcheckcontroldialog.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QComboBox>
#include <QDebug>

namespace TaskControlModel {

class SelfCheckControlDialogPrivate
{
    Q_DECLARE_PUBLIC(SelfCheckControlDialog)
private:
    SelfCheckControlDialogPrivate(SelfCheckControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton){
        initView();
    }

    void initView();

    SelfCheckControlDialog * q_ptr;
    QWidget * mainWidget;

    DialogProxy::StandardButton clickedButt;

    QLineEdit * le_workCycles;
    QLineEdit * le_workCyclesNumber;
    QLineEdit * le_originFrequency;
    QLineEdit * le_frequencyStopping;
    QLineEdit * le_stopFrequency;
    QComboBox * mode_cbox;
    QComboBox * workingBandwidth_cbox;

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

};

void SelfCheckControlDialogPrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout *gridLayout = new QGridLayout(mainWidget);
    gridLayout->setHorizontalSpacing(6);
    gridLayout->setVerticalSpacing(8);
    gridLayout->setContentsMargins(0, 0, -1, 0);

    //工作模式
    QLabel * label = new QLabel(mainWidget);
    label->setText(QObject::tr("Work model"));
    label->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label, 0, 0, 1, 1);

    mode_cbox = new QComboBox(mainWidget);
    mode_cbox->setMaximumSize(QSize(300, 25));
    QStringList modelList;
    modelList<<QObject::tr("self-checking")<<QObject::tr("working");
    mode_cbox->addItems(modelList);
    mode_cbox->setView(new QListView());
    gridLayout->addWidget(mode_cbox, 0, 1, 1, 1);

    //工作周期
    QLabel * label_2 = new QLabel(mainWidget);
    label_2->setText(QObject::tr("Work period"));
    label_2->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_2, 0, 2, 1, 1);

    le_workCycles = new QLineEdit(mainWidget);
    le_workCycles->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_workCycles, 0, 3, 1, 1);

    //工作周期数
    QLabel * label_3 = new QLabel(mainWidget);
    label_3->setText(QObject::tr("Work periods"));
    label_3->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    le_workCyclesNumber = new QLineEdit(mainWidget);
    le_workCyclesNumber->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_workCyclesNumber, 1, 1, 1, 1);

    //工作带宽
    QLabel * label_4 = new QLabel(mainWidget);
    label_4->setText(QObject::tr("Bandwidth"));
    label_4->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_4, 1, 2, 1, 1);

    workingBandwidth_cbox = new QComboBox(mainWidget);
    workingBandwidth_cbox->setView(new QListView());
    QStringList bandList;
    bandList<<QString("400M")<<QString("200M")<<QString("40M")<<QString("10M")<<QString("5M");
    workingBandwidth_cbox->addItems(bandList);
    workingBandwidth_cbox->setMaximumSize(QSize(300, 25));

    gridLayout->addWidget(workingBandwidth_cbox, 1, 3, 1, 1);

    //起始频率
    QLabel * label_5 = new QLabel(mainWidget);
    label_5->setText(QObject::tr("Starting frequency"));
    label_5->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    le_originFrequency = new QLineEdit(mainWidget);
    le_originFrequency->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_originFrequency, 2, 1, 1, 1);

    //终止频率
    QLabel * label_6 = new QLabel(mainWidget);
    label_6->setText(QObject::tr("Stop Frequency"));
    label_6->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_6->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_6, 2, 2, 1, 1);

    le_stopFrequency = new QLineEdit(mainWidget);
    le_stopFrequency->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_stopFrequency, 2, 3, 1, 1);

    //频率步进
    QLabel * label_16 = new QLabel(mainWidget);
    label_16->setText(QObject::tr("Frequency step"));
    label_16->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_16->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_16, 3, 0, 1, 1);

    le_frequencyStopping = new QLineEdit(mainWidget);
    le_frequencyStopping->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_frequencyStopping, 3, 1, 1, 1);

    //衰减码1-2
    QLabel * label_7 = new QLabel(mainWidget);
    label_7->setText(QObject::tr("Decay code1"));
    label_7->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_7->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_7, 4, 0, 1, 1);

    le_attenuationCode1 = new QLineEdit(mainWidget);
    le_attenuationCode1->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode1, 4, 1, 1, 1);

    QLabel * label_8 = new QLabel(mainWidget);
    label_8->setText(QObject::tr("Decay code2"));
    label_8->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_8->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_8, 4, 2, 1, 1);

    le_attenuationCode_2 = new QLineEdit(mainWidget);
    le_attenuationCode_2->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_2, 4, 3, 1, 1);

    //衰减码3-4
    QLabel * label_9 = new QLabel(mainWidget);
    label_9->setText(QObject::tr("Decay code3"));
    label_9->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_9->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    gridLayout->addWidget(label_9, 5, 0, 1, 1);

    le_attenuationCode_3 = new QLineEdit(mainWidget);
    le_attenuationCode_3->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_3, 5, 1, 1, 1);

    QLabel * label_10 = new QLabel(mainWidget);
    label_10->setText(QObject::tr("Decay code4"));
    label_10->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_10->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_10, 5, 2, 1, 1);

    le_attenuationCode_4 = new QLineEdit(mainWidget);
    le_attenuationCode_4->setObjectName(QStringLiteral("le_attenuationCode_4"));
    le_attenuationCode_4->setMaximumSize(QSize(300, 25));

    gridLayout->addWidget(le_attenuationCode_4, 5, 3, 1, 1);

    //衰减码5-6
    QLabel * label_11 = new QLabel(mainWidget);
    label_11->setText(QObject::tr("Decay code5"));
    label_11->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_11->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_11, 6, 0, 1, 1);

    le_attenuationCode_5 = new QLineEdit(mainWidget);
    le_attenuationCode_5->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_5, 6, 1, 1, 1);

    QLabel * label_12 = new QLabel(mainWidget);
    label_12->setText(QObject::tr("Decay code6"));
    label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    gridLayout->addWidget(label_12, 6, 2, 1, 1);

    le_attenuationCode_6 = new QLineEdit(mainWidget);
    le_attenuationCode_6->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_6, 6, 3, 1, 1);

    //衰减码7-8
    QLabel * label_13 = new QLabel(mainWidget);
    label_13->setText(QObject::tr("Decay code7"));
    label_13->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_13->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_13, 7, 0, 1, 1);

    le_attenuationCode_7 = new QLineEdit(mainWidget);
    le_attenuationCode_7->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_7, 7, 1, 1, 1);

    QLabel * label_14 = new QLabel(mainWidget);
    label_14->setText(QObject::tr("Decay code8"));
    label_14->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_14->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_14, 7, 2, 1, 1);

    le_attenuationCode_8 = new QLineEdit(mainWidget);
    le_attenuationCode_8->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_8, 7, 3, 1, 1);

    //衰减码9-10
    QLabel * label_22 = new QLabel(mainWidget);
    label_22->setText(QObject::tr("Decay code9"));
    label_22->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_22->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_22, 8, 0, 1, 1);

    le_attenuationCode_9 = new QLineEdit(mainWidget);
    le_attenuationCode_9->setMaximumSize(QSize(300, 25));
    gridLayout->addWidget(le_attenuationCode_9, 8, 1, 1, 1);

    QLabel * label_17 = new QLabel(mainWidget);
    label_17->setText(QObject::tr("Decay code10"));
    label_17->setMinimumSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_17->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_17, 8, 2, 1, 1);

    le_attenuationCode_10 = new QLineEdit(mainWidget);
    le_attenuationCode_10->setObjectName(QStringLiteral("le_attenuationCode_10"));
    le_attenuationCode_10->setMinimumSize(QSize(100, 20));
    le_attenuationCode_10->setMaximumSize(QSize(300, 25));

    gridLayout->addWidget(le_attenuationCode_10, 8, 3, 1, 1);


    mainWidget->setLayout(gridLayout);
}

SelfCheckControlDialog::SelfCheckControlDialog(QWidget *parent)
    :d_ptr(new SelfCheckControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Selfcheck control"));
    
//    QRegExp reint("^(-[0-9]\\d*|[0-9]\\d*)?$");  //整数类型
//    QRegExpValidator *Validator_int = new QRegExpValidator(reint, this);
//    ui->le_workCyclesNumber->setValidator(Validator_int);
    
//    QRegExp refloat("^((-[0-9]\\d*|[0-9]\\d*)\\.?)?([0-9]){5}$");  //浮点类型 可输入小数点后五位
//    QRegExpValidator *Validator_float = new QRegExpValidator(refloat, this);
//    ui->le_originFrequency->setValidator(Validator_float);
//    ui->le_stopFrequency->setValidator(Validator_float);
//    ui->le_frequencyStopping->setValidator(Validator_float);
//    ui->le_attenuationCode1->setValidator(Validator_float);
//    ui->le_attenuationCode_2->setValidator(Validator_float);
//    ui->le_attenuationCode_3->setValidator(Validator_float);
//    ui->le_attenuationCode_4->setValidator(Validator_float);
//    ui->le_attenuationCode_5->setValidator(Validator_float);
//    ui->le_attenuationCode_6->setValidator(Validator_float);
//    ui->le_attenuationCode_7->setValidator(Validator_float);
//    ui->le_attenuationCode_8->setValidator(Validator_float);
//    ui->le_attenuationCode_9->setValidator(Validator_float);
//    ui->le_attenuationCode_10->setValidator(Validator_float);
}

SelfCheckControlDialog::~SelfCheckControlDialog()
{

}

void SelfCheckControlDialog::setWindowData(SelfCheckControl * selfCheck)
{
    Q_D(SelfCheckControlDialog);
    //工作模式
    if (selfCheck->workmode == 0)    //0自检
        d->mode_cbox->setCurrentIndex(0);
    else if (selfCheck->workmode == 1)   //工作
        d->mode_cbox->setCurrentIndex(1);
    else
        d->mode_cbox->setCurrentIndex(0);
    
    d->le_workCycles->setText(QString("%1").arg(selfCheck->workCycles));
    d->le_workCyclesNumber->setText(QString("%1").arg(selfCheck->workCyclesNumber));
    
    //工作带宽
    if (selfCheck->workingBandwidth == 0)    //400M
        d->workingBandwidth_cbox->setCurrentIndex(0);
    else if (selfCheck->workingBandwidth == 1)   //200M
        d->workingBandwidth_cbox->setCurrentIndex(1);
    else if (selfCheck->workingBandwidth == 2)   //40M
        d->workingBandwidth_cbox->setCurrentIndex(2);
    else if (selfCheck->workingBandwidth == 3)   //10M
        d->workingBandwidth_cbox->setCurrentIndex(3);
    else if (selfCheck->workingBandwidth == 4)   //5M
        d->workingBandwidth_cbox->setCurrentIndex(4);
    else
        d->workingBandwidth_cbox->setCurrentIndex(0);
    
    d->le_originFrequency->setText(QString("%1").arg(selfCheck->originFrequency));  //起始频率
    d->le_stopFrequency->setText(QString("%1").arg(selfCheck->stopFrequency));    //终止频率
    d->le_frequencyStopping->setText(QString("%1").arg(selfCheck->frequencyStopping));//频率步进
    
    //衰减码
    d->le_attenuationCode1 ->setText(QString("%1").arg(selfCheck->attenuationCode1));
    d->le_attenuationCode_2->setText(QString("%1").arg(selfCheck->attenuationCode2));
    d->le_attenuationCode_3->setText(QString("%1").arg(selfCheck->attenuationCode3));
    d->le_attenuationCode_4->setText(QString("%1").arg(selfCheck->attenuationCode4));
    d->le_attenuationCode_5->setText(QString("%1").arg(selfCheck->attenuationCode5));
    d->le_attenuationCode_6->setText(QString("%1").arg(selfCheck->attenuationCode6));
    d->le_attenuationCode_7->setText(QString("%1").arg(selfCheck->attenuationCode7));
    d->le_attenuationCode_8->setText(QString("%1").arg(selfCheck->attenuationCode8));
    d->le_attenuationCode_9->setText(QString("%1").arg(selfCheck->attenuationCode9));
    d->le_attenuationCode_10->setText(QString("%1").arg(selfCheck->attenuationCode10));
}


SelfCheckControl *SelfCheckControlDialog::getWindowData()
{
    Q_D(SelfCheckControlDialog);
//    if (d->le_originFrequency->text() == NULL || d->le_stopFrequency->text() == NULL ||
//            d->le_frequencyStopping->text() == NULL)
//        return;

    if(d->clickedButt == DialogProxy::Ok){
        SelfCheckControl * selfCheck = new SelfCheckControl;
        selfCheck->excuteTime = QDateTime::currentDateTime();
        selfCheck->lastTime = 1;

        selfCheck->workmode = d->mode_cbox->currentIndex();
        selfCheck->workCycles = d->le_workCycles->text().toInt();
        selfCheck->workCyclesNumber = d->le_workCyclesNumber->text().toFloat();
        selfCheck->workingBandwidth = d->workingBandwidth_cbox->currentIndex();

        selfCheck->originFrequency = d->le_originFrequency->text().toDouble();
        selfCheck->stopFrequency = d->le_stopFrequency->text().toDouble();
        selfCheck->frequencyStopping = d->le_frequencyStopping->text().toDouble();

        //衰减码
        selfCheck->attenuationCode1 = d->le_attenuationCode1->text().toFloat();
        selfCheck->attenuationCode2 = d->le_attenuationCode_2->text().toFloat();
        selfCheck->attenuationCode3 = d->le_attenuationCode_3->text().toFloat();
        selfCheck->attenuationCode4 = d->le_attenuationCode_4->text().toFloat();
        selfCheck->attenuationCode5 = d->le_attenuationCode_5->text().toFloat();
        selfCheck->attenuationCode6 = d->le_attenuationCode_6->text().toFloat();
        selfCheck->attenuationCode7 = d->le_attenuationCode_7->text().toFloat();
        selfCheck->attenuationCode8 = d->le_attenuationCode_8->text().toFloat();
        selfCheck->attenuationCode9 = d->le_attenuationCode_9->text().toFloat();
        selfCheck->attenuationCode10 = d->le_attenuationCode_10->text().toFloat();
        return selfCheck;
    }
    return NULL;
}


QSize SelfCheckControlDialog::sizeHint() const
{
    return QSize(615,410);
}

void SelfCheckControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(SelfCheckControlDialog);
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

void SelfCheckControlDialog::respOk()
{
    respCancel();
}

void SelfCheckControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
