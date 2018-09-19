#include "gathercontroldialog.h"
#include "ui_gathercontroldialog.h"

#include <QMessageBox>
#include <QListView>
#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class GatherControlDialogPrivate
{
    Q_DECLARE_PUBLIC(GatherControlDialog)
private:
    GatherControlDialogPrivate(GatherControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    GatherControlDialog * q_ptr;
    QWidget * mainWidget;

    DialogProxy::StandardButton clickedButt;

    Ui::GatherControlDialog *ui;

    GatherControl * modifyInfo;
};

void GatherControlDialogPrivate::initView()
{
    mainWidget = new QWidget();
    ui =  new Ui::GatherControlDialog;
    ui->setupUi(mainWidget);

    ui->cbox_GatherWay->setView(new QListView());
    ui->cbox_GatherType->setView(new QListView());
    ui->cbox_SendingPause->setView(new QListView());

}

GatherControlDialog::GatherControlDialog(QWidget *parent):
    d_ptr(new GatherControlDialogPrivate(this))
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Gather control"));
}

GatherControlDialog::~GatherControlDialog()
{

}

QSize GatherControlDialog::sizeHint() const
{
    return QSize(615,450);
}

void GatherControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(GatherControlDialog);
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

void GatherControlDialog::respOk()
{
    respCancel();
}

void GatherControlDialog::respCancel()
{
    Q_D(GatherControlDialog);
    RAndCombineValidator validator;
    validator.addValidator(new RNumericValidator<float>(d->ui->le_Time->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<int>(d->ui->le_ImpulseNumber->text().toInt(),RValid::Ge,0,RValid::Le,1000000));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_FrequencyMin->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_FrequencyMax->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_PulseyMin->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_PulseyMax->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_PowerMin->text().toFloat(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<float>(d->ui->le_PowerMax->text().toFloat(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }

    close();
}

void GatherControlDialog::setWindowData(GatherControl * gather)
{
    Q_D(GatherControlDialog);
    d->modifyInfo = gather;
    //方式
    d->ui->cbox_GatherWay->setCurrentIndex(static_cast<int>(gather->gatherWay));
    //类型
    d->ui->cbox_GatherType->setCurrentIndex(static_cast<int>(gather->gatherType));
    //采集指令
    d->ui->cbox_SendingPause->setCurrentIndex(static_cast<int>(gather->sendingPause));
    //抽取开关
    d->ui->check_ExtractSwitch->setChecked(gather->extractSwitch);

    //采集时长
    d->ui->le_Time->setText(QString("%1").arg(gather->gatherTime));
    //采集脉冲个数
    d->ui->le_ImpulseNumber->setText(QString("%1").arg(gather->gatherImpulseNumber));
    //采集频率下限
    d->ui->le_FrequencyMin->setText(QString("%1").arg(gather->gatherFrequencyMin));
    //采集频率上限
    d->ui->le_FrequencyMax->setText(QString("%1").arg(gather->gatherFrequencyMax));
    //采集脉宽下限
    d->ui->le_PulseyMin->setText(QString("%1").arg(gather->gatherPulseyMin));
    //采集脉宽上限
    d->ui->le_PulseyMax->setText(QString("%1").arg(gather->gatherPulseyMax));
    //采集功率下限
    d->ui->le_PowerMin->setText(QString("%1").arg(gather->gatherPowerMin));
    //采集功率上限
    d->ui->le_PowerMax->setText(QString("%1").arg(gather->gatherPowerMax));
    
    int i = gather->gatherTacticsSwitch;
    
    if (i & 1)
        d->ui->check_TimeFlag->setChecked(true);
    
    if (i & (1<<1))
        d->ui->check_NumberFlag->setChecked(true);
    
    if (i & (1<<2))
        d->ui->check_FrequencyFlag->setChecked(true);
    
    if (i & (1<<3))
        d->ui->check_PulseyFlag->setChecked(true);
    
    if (i & (1<<4))
        d->ui->check_PowerFlag->setChecked(true);
}


GatherControl * GatherControlDialog::getWindowData()
{
    Q_D(GatherControlDialog);
    if(d->clickedButt == DialogProxy::Ok){
        GatherControl * gather = NULL;
        if(d->modifyInfo)
            gather = d->modifyInfo;
        else
            gather = new GatherControl;

        gather->excuteTime = QDateTime::currentDateTime();
        gather->lastTime = 1;
        int temp_I = -1;
        gather->gatherWay = d->ui->cbox_GatherWay->currentIndex();    //采集方式
        gather->gatherType = d->ui->cbox_GatherType->currentIndex();   //采集类型
        gather->sendingPause = d->ui->cbox_SendingPause->currentIndex(); //采集指令
        gather->extractSwitch = d->ui->check_ExtractSwitch->checkState();//抽取开关
        gather->gatherTime = d->ui->le_Time->text().toFloat();   //采集时长

        temp_I = d->ui->le_ImpulseNumber->text().toInt();
        if (temp_I < 0)
            temp_I = 0;
        else if (temp_I > 1000000)
            temp_I = 1000000;
        gather->gatherImpulseNumber = temp_I;  //脉冲个数

        gather->gatherFrequencyMin = d->ui->le_FrequencyMin->text().toFloat();   //频率下限
        gather->gatherFrequencyMax = d->ui->le_FrequencyMax->text().toFloat();   //频率上限
        gather->gatherPulseyMin = d->ui->le_PulseyMin->text().toFloat();  //脉宽下限
        gather->gatherPulseyMax = d->ui->le_PulseyMax->text().toFloat();   //脉宽上限
        gather->gatherPowerMin = d->ui->le_PowerMin->text().toFloat();   //功率下限
        gather->gatherPowerMax = d->ui->le_PowerMax->text().toFloat();   //功率上限

        //采集策略开关
    //    gather->GatherTacticsSwitch &= 0xff/* = nullptr*/;
        if (d->ui->check_TimeFlag->checkState() == Qt::Checked)   //时长是否有效
            gather->gatherTacticsSwitch |= 1;

        if (d->ui->check_NumberFlag->checkState() == Qt::Checked)    //个数是否有效
            gather->gatherTacticsSwitch |= (1<<1);

        if (d->ui->check_FrequencyFlag->checkState() == Qt::Checked) //频率是否有效
            gather->gatherTacticsSwitch |= (1<<2);

        if (d->ui->check_PulseyFlag->checkState() == Qt::Checked)    //脉宽是否有效
            gather->gatherTacticsSwitch |= (1<<3);

        if (d->ui->check_PowerFlag->checkState() == Qt::Checked)     //功率是否有效
            gather->gatherTacticsSwitch |= (1<<4);
        return gather;
    }
    
    return NULL;
}

} //namespace TaskControlModel
