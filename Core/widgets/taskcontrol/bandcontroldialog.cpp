#include "bandcontroldialog.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QIntValidator>
#include <QMessageBox>

#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class BandControlDialogPrivate
{
    Q_DECLARE_PUBLIC(BandControlDialog)
private:
    BandControlDialogPrivate(BandControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    BandControlDialog * q_ptr;
    QWidget * mainWidget;

    QLineEdit * startingFrequencyEdit;
    QLineEdit * stopFrequencyEdit;
    QLineEdit * frequencyStepEdit;

    DialogProxy::StandardButton clickedButt;

    BandControl * modifyInfo;
};

void BandControlDialogPrivate::initView()
{
    mainWidget = new QWidget();

    QRegExp rx("^((-[0-9]\\d*|[0-9]\\d*)\\.?)?([0-9]){5}$");  //浮点类型 可输入小数点后五位
//    QRegExpValidator *m_IPValidator = new QRegExpValidator(rx, this);

    QGridLayout *gridLayout = new QGridLayout(mainWidget);

    QLabel * label_2 = new QLabel(mainWidget);
    label_2->setText(QObject::tr("Starting frequency"));
    label_2->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_2->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    startingFrequencyEdit = new QLineEdit(mainWidget);
    startingFrequencyEdit->setFixedSize(QSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT));
    startingFrequencyEdit->setEchoMode(QLineEdit::Normal);

    gridLayout->addWidget(startingFrequencyEdit, 1, 1, 1, 1);

    QLabel * label_3 = new QLabel(mainWidget);
    label_3->setText(QObject::tr("Stop Frequency"));
    label_3->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_3->setLayoutDirection(Qt::RightToLeft);
    label_3->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    stopFrequencyEdit = new QLineEdit(mainWidget);
    stopFrequencyEdit->setFixedSize(QSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT));
    gridLayout->addWidget(stopFrequencyEdit, 2, 1, 1, 1);

    QLabel * label_4 = new QLabel(mainWidget);
    label_4->setText(QObject::tr("Frequency step"));
    label_4->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_4->setLayoutDirection(Qt::RightToLeft);
    label_4->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_4, 3, 0, 1, 1);

    frequencyStepEdit = new QLineEdit(mainWidget);
    frequencyStepEdit->setFixedSize(QSize(LINEDIT_FIXED_WIDTH, LINEDIT_FIXED_HEIGHT));

    gridLayout->addWidget(frequencyStepEdit, 3, 1, 1, 1);

    mainWidget->setLayout(gridLayout);
}

BandControlDialog::BandControlDialog(QWidget *parent)
    :d_ptr(new BandControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Band control"));
}

BandControlDialog::~BandControlDialog()
{

}

BandControl * BandControlDialog::getWindowData()
{
    Q_D(BandControlDialog);

    if(d->clickedButt == DialogProxy::Ok){
        BandControl * band = NULL;
        if(d->modifyInfo)
            band = d->modifyInfo;
        else
            band = new BandControl;
        band->excuteTime = QDateTime::currentDateTime();
        band->lastTime = 1;
        band->originFrequency = d->startingFrequencyEdit->text().toDouble();
        band->stopFrequency = d->stopFrequencyEdit->text().toDouble();
        band->frequencyStopping = d->frequencyStepEdit->text().toDouble();
        return band;
    }
    return NULL;
}

void BandControlDialog::setWindowData(BandControl *info)
{
    Q_D(BandControlDialog);
    d->modifyInfo = info;
    d->startingFrequencyEdit->setText(QString("%1").arg(info->originFrequency));
    d->stopFrequencyEdit->setText(QString("%1").arg(info->stopFrequency));
    d->frequencyStepEdit->setText(QString("%1").arg(info->frequencyStopping));
}

QSize BandControlDialog::sizeHint() const
{
    return QSize(350,280);
}

void BandControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(BandControlDialog);
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

void BandControlDialog::respOk()
{
    Q_D(BandControlDialog);
    RAndCombineValidator validator;
    validator.addValidator(new RNumericValidator<double>(d->startingFrequencyEdit->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->stopFrequencyEdit->text().toDouble(),RValid::Ge,0));
    validator.addValidator(new RNumericValidator<double>(d->frequencyStepEdit->text().toDouble(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }
    respCancel();
}

void BandControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
