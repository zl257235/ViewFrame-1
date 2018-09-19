#include "playbackcontroldialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>

#include "Base/common/validator/rcombinevalidator.h"

namespace TaskControlModel {

class PlayBackControlDialogPrivate
{
    Q_DECLARE_PUBLIC(PlayBackControlDialog)
private:
    PlayBackControlDialogPrivate(PlayBackControlDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),modifyInfo(NULL){
        initView();
    }

    void initView();

    PlayBackControlDialog * q_ptr;
    QWidget * mainWidget;

    DialogProxy::StandardButton clickedButt;

    QLineEdit * path_TEdit;
    QLineEdit * originPos_le;
    QLineEdit * speed_le;
    QComboBox * type_cbox;

    PlayBackControl * modifyInfo;
};

void PlayBackControlDialogPrivate::initView()
{
    mainWidget = new QWidget();

    QGridLayout *gridLayout = new QGridLayout(mainWidget);
    gridLayout->setHorizontalSpacing(6);
    gridLayout->setVerticalSpacing(8);
    gridLayout->setContentsMargins(0, 0, -1, 0);

    //回放文件路径
    QLabel * label_2 = new QLabel(mainWidget);
    label_2->setText(QObject::tr("Playback file path"));
    label_2->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    path_TEdit = new QLineEdit(mainWidget);
    path_TEdit->setObjectName(QStringLiteral("path_TEdit"));
    path_TEdit->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(path_TEdit, 0, 1, 1, 1);

    //回放类型
    QLabel * label_3 = new QLabel(mainWidget);
    label_3->setText(QObject::tr("Type"));
    label_3->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    type_cbox = new QComboBox(mainWidget);
    type_cbox->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    type_cbox->setView(new QListView());
    QStringList list;
    list<<QStringLiteral("辐射源")<<QStringLiteral("全脉冲")<<QStringLiteral("中频采集")<<QStringLiteral("频谱")
          <<QStringLiteral("设备状态");
    type_cbox->addItems(list);
    gridLayout->addWidget(type_cbox, 1, 1, 1, 1);

    //回放起始位置
    QLabel* label_4 = new QLabel(mainWidget);
    label_4->setText(QObject::tr("Playback position"));
    label_4->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    originPos_le = new QLineEdit(mainWidget);
    originPos_le->setObjectName(QStringLiteral("originPos_le"));
    originPos_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(originPos_le, 2, 1, 1, 1);

    //回放速度
    QLabel * label_5 = new QLabel(mainWidget);
    label_5->setText(QObject::tr("Playback Speed"));
    label_5->setFixedSize(QSize(LABEL_MIN_WIDTH, LABEL_MIN_HEIGHT));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    gridLayout->addWidget(label_5, 3, 0, 1, 1);

    speed_le = new QLineEdit(mainWidget);
    speed_le->setFixedSize(LINEDIT_FIXED_WIDTH,LINEDIT_FIXED_HEIGHT);
    gridLayout->addWidget(speed_le, 3, 1, 1, 1);

    mainWidget->setLayout(gridLayout);
}

PlayBackControlDialog::PlayBackControlDialog(QWidget *parent)
    :d_ptr(new PlayBackControlDialogPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Playback control"));
}

PlayBackControlDialog::~PlayBackControlDialog()
{

}

void PlayBackControlDialog::setWindowData(PlayBackControl * playBack)
{
    Q_D(PlayBackControlDialog);
    d->modifyInfo = playBack;
    d->path_TEdit->setText(QString::fromLocal8Bit(playBack->path));
    d->type_cbox->setCurrentIndex(playBack->type);
    d->originPos_le->setText(QString("%1").arg(playBack->originPos));
    d->speed_le->setText(QString("%1").arg(playBack->speed));
}

PlayBackControl* PlayBackControlDialog::getWindowData()
{
    Q_D(PlayBackControlDialog);
    if(d->clickedButt == DialogProxy::Ok){
        PlayBackControl * playBack = NULL;
        if(d->modifyInfo)
            playBack = d->modifyInfo;
        else
            playBack = new PlayBackControl;
        playBack->excuteTime = QDateTime::currentDateTime();
        playBack->lastTime = 1;

        QByteArray _BYTE = d->path_TEdit->text().toLocal8Bit();
        strcpy(playBack->path, _BYTE);
        playBack->type = d->type_cbox->currentIndex();
        playBack->originPos = d->originPos_le->text().toDouble();
        playBack->speed = d->speed_le->text().toDouble();
        return playBack;
    }
    return NULL;
}

QSize PlayBackControlDialog::sizeHint() const
{
    return QSize(450,300);
}

void PlayBackControlDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(PlayBackControlDialog);
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

void PlayBackControlDialog::respOk()
{
    Q_D(PlayBackControlDialog);
    RAndCombineValidator validator;
    validator.addValidator(new RNumericValidator<double>(d->originPos_le->text().toDouble(),RValid::Ge,0,RValid::Le,1));
    validator.addValidator(new RNumericValidator<int>(d->speed_le->text().toDouble(),RValid::Ge,0));

    if(validator.validate() == RValid::Invalid){
        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
        return;
    }
    respCancel();
}

void PlayBackControlDialog::respCancel()
{
    close();
}

} //namespace TaskControlModel
