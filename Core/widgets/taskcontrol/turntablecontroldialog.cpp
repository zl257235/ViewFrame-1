#include "turntablecontroldialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include <QGridLayout>
#include <QMessageBox>

#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class TurntableControlDialogPrivate
{
    Q_DECLARE_PUBLIC(TurntableControlDialog)
private:
    TurntableControlDialogPrivate(TurntableControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    TurntableControlDialog * q_ptr;
    QWidget * mainWidget;

    DialogProxy::StandardButton clickedButt;

    QLineEdit * positionEdit;
    QLineEdit * speedEdit;
    QComboBox * type_cbox;
    QComboBox * directionRotation_cbox;
    QCheckBox * zeroPosSetting_check;

    TurntableControl * modifyInfo;
};

void TurntableControlDialogPrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout *  gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(10);

    //类型
    QLabel * label_2 = new QLabel(mainWidget);
    label_2->setText(QObject::tr("Type"));
    label_2->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    type_cbox = new QComboBox(mainWidget);
    type_cbox->setView(new QListView());
    type_cbox->setObjectName(QStringLiteral("type_cbox"));
    type_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    type_cbox->addItem(QObject::tr("Type 1"));
    type_cbox->addItem(QObject::tr("Type 2"));

    gridLayout->addWidget(type_cbox, 0, 1, 1, 1);

    //转台位置
    QLabel * label_3 = new QLabel(mainWidget);
    label_3->setText(QObject::tr("Position"));
    label_3->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    positionEdit = new QLineEdit(mainWidget);
    positionEdit->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(positionEdit, 1, 1, 1, 1);

    //转台速度
    QLabel * label_4 = new QLabel(q_ptr);
    label_4->setText(QObject::tr("Speed"));
    label_4->setObjectName(QStringLiteral("label_4"));
    label_4->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    speedEdit = new QLineEdit(mainWidget);
    speedEdit->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(speedEdit, 2, 1, 1, 1);

    //转台方向
    QLabel * label_5 = new QLabel(mainWidget);
    label_5->setText(QObject::tr("Direction"));
    label_5->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_5, 3, 0, 1, 1);

    directionRotation_cbox = new QComboBox(mainWidget);
    directionRotation_cbox->setView(new QListView());
    directionRotation_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    directionRotation_cbox->addItem(QObject::tr("Clockwise"));
    directionRotation_cbox->addItem(QObject::tr("Anti-clockwise"));

    gridLayout->addWidget(directionRotation_cbox, 3, 1, 1, 1);

    //零位设置
    QLabel * label_6 = new QLabel(mainWidget);
    label_6->setText(QObject::tr("Zero setting"));
    label_6->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_6, 4, 0, 1, 1);

    zeroPosSetting_check = new QCheckBox(mainWidget);
    zeroPosSetting_check->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);

    gridLayout->addWidget(zeroPosSetting_check, 4, 1, 1, 1);

    mainWidget->setLayout(gridLayout);
}

TurntableControlDialog::TurntableControlDialog(QWidget *parent)
    :d_ptr(new TurntableControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Turning table control"));
}

TurntableControlDialog::~TurntableControlDialog()
{

}

void TurntableControlDialog::setWindowData(TurntableControl * turntable)
{
    Q_D(TurntableControlDialog);
    d->modifyInfo = turntable;
    d->type_cbox->setCurrentIndex(turntable->type);
    d->positionEdit->setText(QString("%1").arg(turntable->position));
    d->speedEdit->setText(QString("%1").arg(turntable->speed));
    d->directionRotation_cbox->setCurrentIndex(turntable->directionRotation);
    if (turntable->zeroPosSetting == 1)
        d->zeroPosSetting_check->setChecked(true);
}

TurntableControl *TurntableControlDialog::getWindowData()
{
    Q_D(TurntableControlDialog);
    if(d->clickedButt == DialogProxy::Ok){
        TurntableControl * turntable = NULL;
        if(d->modifyInfo)
            turntable = d->modifyInfo;
        else
            turntable = new TurntableControl;
        turntable->excuteTime = QDateTime::currentDateTime();
        turntable->lastTime = 1;

        turntable->type = d->type_cbox->currentIndex();
        turntable->position = d->positionEdit->text().toDouble();
        turntable->speed = d->speedEdit->text().toDouble();
        turntable->directionRotation = d->directionRotation_cbox->currentIndex();
        if (d->zeroPosSetting_check->checkState() == Qt::Checked)
            turntable->zeroPosSetting = 1;

        return turntable;
    }
    return NULL;
}

QSize TurntableControlDialog::sizeHint() const
{
    return QSize(440,330);
}

void TurntableControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(TurntableControlDialog);
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

void TurntableControlDialog::respOk()
{
    Q_D(TurntableControlDialog);
    RAndCombineValidator validator;
    validator.addValidator(new RNumericValidator<double>(d->positionEdit->text().toDouble(),RValid::Ge,0,RValid::Le,360));
    validator.addValidator(new RNumericValidator<double>(d->speedEdit->text().toDouble(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }
    respCancel();
}

void TurntableControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
