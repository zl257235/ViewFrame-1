#include "taskcontrol.h"

#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <QArrayData>
#include <QDateTime>
#include <QModelIndex>
#include <QTimerEvent>
#include <QPushButton>
#include <QGridLayout>
#include <QList>
#include <QMessageBox>

#include "modelview/tableview.h"
#include "modelview/tableviewmodel.h"
#include "modelview/tableviewdelegate.h"
#include "utils.h"
#include "task.h"

#include "bandcontroldialog.h"
#include "turntablecontroldialog.h"
#include "statecontroldialog.h"
#include "playbackcontroldialog.h"
#include "selfcheckcontroldialog.h"
#include "instrumentcontroldialog.h"
#include "gathercontroldialog.h"

namespace TaskControlModel {

const char ACTION_TYPE[] = "ActionType";

class TaskControlPrivate
{
    Q_DECLARE_PUBLIC(TaskControl)
public:
    explicit TaskControlPrivate(TaskControl * q):q_ptr(q),cacheTaskInfo(NULL){
        initTableView();
        initTableViewMenu();
        initView();
    }

    void initView();
    void initTableView();
    void initTableViewMenu();

    bool bPendingFlag = false;                   //是否处于下发任务状态
    int iPendingTimerID;                         //下发任务定时器ID
    QList<pendingTaskState> m_pendingTaskInfo;   //当前任务下发信息

    TableView* taskView;
    TableViewModel * taskViewModel;
    TableViewDelegate * taskViewDelegate;

    QPushButton * m_MissionButton;
    QWidget * m_centralWidget;

    QWidget * mainWidget;

    TaskInfoList taskInfoList;          /*!< 任务集合 */
    QPoint contextPoint;                /*!< 表格右键事件菜单 */
    TaskInfo * cacheTaskInfo;           /*!< 复制的任务信息 */

    TaskControl * q_ptr;
};

void TaskControlPrivate::initView()
{
    mainWidget = new QWidget(q_ptr);
    m_MissionButton = new QPushButton(QStringLiteral("Distribute task"),mainWidget);
    QObject::connect(m_MissionButton, SIGNAL(pressed()), q_ptr, SLOT(distributeTask()));
    m_MissionButton->setMaximumHeight(50);
    m_MissionButton->setMinimumHeight(35);
    m_MissionButton->show();

    QGridLayout * m_grdLayout = new QGridLayout(mainWidget);
    m_grdLayout->addWidget(taskView, 0, 0, 10,10);
    m_grdLayout->addWidget(m_MissionButton, 10, 4, 1, 2, Qt::AlignVCenter | Qt::AlignVCenter);
    m_grdLayout->setHorizontalSpacing(10);
    m_grdLayout->setVerticalSpacing(6);
    m_grdLayout->setContentsMargins(10, 5, 10, 5);

    mainWidget->setLayout(m_grdLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);
}

void TaskControlPrivate::initTableView()
{
    taskView = new TableView(q_ptr);

    taskViewModel = new TableViewModel;
    taskViewDelegate = new TableViewDelegate(taskView);

    taskView->setModel(taskViewModel);
    taskView->setItemDelegateForColumn(3, taskViewDelegate);
    QObject::connect(taskView,SIGNAL(contextMenuPos(QPoint)),q_ptr,SLOT(tableContextPoint(QPoint)));
    QObject::connect(taskView, SIGNAL(tableCheckDoubleSignal(QModelIndex)), q_ptr, SLOT(viewTask(QModelIndex)));
}

void TaskControlPrivate::initTableViewMenu()
{
    QMenu *m_AddMenu = new QMenu(q_ptr);
    QAction *action = new QAction(QObject::tr("Band Control"));
    action->setProperty(ACTION_TYPE,Type::Band);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("State Control"));
    action->setProperty(ACTION_TYPE,Type::State);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("Gather Control"));
    action->setProperty(ACTION_TYPE,Type::Gather);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("SelfCheck Control"));
    action->setProperty(ACTION_TYPE,Type::SelfCheck);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("Instrument Control"));
    action->setProperty(ACTION_TYPE,Type::Instrument);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("Turntable Control"));
    action->setProperty(ACTION_TYPE,Type::Turntable);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);

    action = new QAction(QObject::tr("PlayBack Control"));
    action->setProperty(ACTION_TYPE,Type::PlayBack);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(action);


    action = new QAction(QObject::tr("Add task"));
    action->setMenu(m_AddMenu);
    taskView->addAction(action);

    action = new QAction(QObject::tr("Delete task"));
    action->setShortcut(QKeySequence::Delete);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(deleteTask()));
    taskView->addAction(action);

    action = new QAction(QObject::tr("Modify task"));
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(modifyTask()));
    taskView->addAction(action);

    action = new QAction(QObject::tr("Copy task"));
    action->setShortcut(QKeySequence::Copy);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(copyTable()));
    taskView->addAction(action);

    action = new QAction(QObject::tr("Paste task"));
    action->setShortcut(QKeySequence::Paste);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(pasteTask()));
    taskView->addAction(action);
}

TaskControl::TaskControl(QWidget *parent) :
    QWidget(parent),d_ptr(new TaskControlPrivate(this))
{

}

TaskControl::~TaskControl()
{

}

void TaskControl::timerEvent(QTimerEvent *event)
{
    Q_D(TaskControl);
    if (event->timerId() == d->iPendingTimerID)
    {
        for (int i = 0; i < d->m_pendingTaskInfo.count(); i++)
        {
            pendingTaskState pendingtaskstate = d->m_pendingTaskInfo.at(i);
            //当前任务是否已经下发
            if (!pendingtaskstate.issued)
            {
                QDateTime tasktime = QDateTime::fromString(pendingtaskstate.ExcutionTime, "yyyy-MM-dd HH:mm:ss");
                QDateTime nowTime = QDateTime::currentDateTime();
                if (nowTime >= tasktime)
                {
//                    Task* taskData = d->m_AllDisplayTasks->getAllTaskData(i);
//                    if (taskData == NULL)
//                        return;
                    
//                    //当前任务已下发执行下发操作，复位标识位
//                    pendingtaskstate.issued = true;
//                    d->m_pendingTaskInfo.replace(i,pendingtaskstate);
                    
//                    QByteArray buffArray;
//                    int controlLength = 0;
//                    BandControl bandcontrol;
                    
//                    Type type = pendingtaskstate.type;
                    
//                    if (type == Type::Band)
//                    {
//                        BandTask *band = taskData->getControlClass<BandTask>();
//                        if (band == NULL)
//                            return;
//                        /*BandControl */bandcontrol = band->getTaskControlData();
//                        controlLength = sizeof(bandcontrol);
                        
//                        buffArray.resize(controlLength); //设置容量
//                        memcpy(buffArray.data(),&bandcontrol,sizeof(BandControl)); //指针移动,写入多个数据
                        
//                        BandControl *person=(BandControl*)buffArray.data();
//                    }
//                    else if (type == Type::State)
//                    {
//                        StateTask *state = taskData->getControlClass<StateTask>();
//                        StateControl bandcontrol = state->getTaskControlData();
//                    }
//                    else if (type == Type::Gather)
//                    {
//                        GatherTask *gather = taskData->getControlClass<GatherTask>();
//                        GatherControl bandcontrol = gather->getTaskControlData();
//                    }
//                    else if (type == Type::SelfCheck)
//                    {
//                        SelfCheckTask *selfcheck = taskData->getControlClass<SelfCheckTask>();
//                        SelfCheckControl bandcontrol = selfcheck->getTaskControlData();
//                    }
//                    else if (type == Type::Instrument)
//                    {
//                        InstrumentTask *instrument = taskData->getControlClass<InstrumentTask>();
//                        InstrumentControl bandcontrol = instrument->getTaskControlData();
//                    }
//                    else if (type == Type::Turntable)
//                    {
//                        TurntableTask *turntable = taskData->getControlClass<TurntableTask>();
//                        TurntableControl bandcontrol = turntable->getTaskControlData();
//                    }
//                    else if (type == Type::PlayBack)
//                    {
//                        PlayBackTask *playback = taskData->getControlClass<PlayBackTask>();
//                        PlayBackControl bandcontrol = playback->getTaskControlData();
//                    }
                }
            }
        }   //End For
        
    }
}

/**
 * @brief TaskControl::SendTaskControl
 * @param b 
 * 任务下发按钮事件 将表格中所有任务控制信息通过udp向外发送
 */
void TaskControl::distributeTask()
{
    Q_D(TaskControl);
    //如果当前没有任务下发则下发当前所有信息
//    if (d->m_pendingTaskInfo.isEmpty())
//    {
//        QList<TaskInfo> taskinfo = d->taskViewModel->getTaskInfoList();
//        if (taskinfo.isEmpty())
//            return;
        
//        for (int i = 0; i < taskinfo.count(); i++)
//        {
//            pendingTaskState pendingtask;
//            pendingtask.ifExists = true;
//            pendingtask.issued = false;
//            pendingtask.type = taskinfo.at(i).type;
//            pendingtask.ExcutionTime = taskinfo.at(i).taskExcutionTime;
//            d->m_pendingTaskInfo.append(pendingtask);
//        }
//    }
//    else    //如果已经有下发任务则添加没有下发过的任务
//    {
//        QList<TaskInfo> taskinfo = d->taskViewModel->getTaskInfoList();
//        if (taskinfo.isEmpty())
//            return;
        
//        for (int i = 0; i < taskinfo.count(); i++)
//        {
//            if (i+1 > d->m_pendingTaskInfo.count())
//            {
//                pendingTaskState pendingtask;
//                pendingtask.ifExists = true;
//                pendingtask.issued = false;
//                pendingtask.type = taskinfo.at(i).type;
//                pendingtask.ExcutionTime = taskinfo.at(i).taskExcutionTime;
//                d->m_pendingTaskInfo.insert(i, pendingtask);
//            }
//        }
//    }
    
    if (d->bPendingFlag)
    {
        d->m_MissionButton->setText(QStringLiteral("下发任务"));
        this->killTimer(d->iPendingTimerID);
    }
    else
    {
        d->m_MissionButton->setText(QStringLiteral("下发中..."));
        d->iPendingTimerID = this->startTimer(2000);
    }
    d->bPendingFlag = !d->bPendingFlag;
}

/*!
 * @brief 表格右键菜单触发的坐标
 * @param[in] point 右键事件触发位置
 */
void TaskControl::tableContextPoint(QPoint point)
{
    Q_D(TaskControl);
    d->contextPoint = point;
}

void TaskControl::initNetWork()
{
    //    m_InterNetWork = new NetworkInterface(this);
}

/*!
 * @brief 获取右键菜单下的单元行
 * @param[in/out] valideIndex 当前行是否有效
 * @return 当前单元格索引
 */
QModelIndex TaskControl::currentIndex(bool & validIndex)
{
    Q_D(TaskControl);
    QModelIndex index = d->taskView->indexAt(d->contextPoint);
    if(index.isValid() && index.row() >= 0 && index.row() < d->taskInfoList.size())
        validIndex = true;
    else
        validIndex = false;
    return index;
}

void TaskControl::showEditWindow(QModelIndex index)
{
    Q_D(TaskControl);
    switch(d->taskInfoList.at(index.row())->taskType){
        case Type::Band:
            {
                BandControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<BandControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::State:
            {
                StateControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<StateControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::Gather:
            {
                GatherControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<GatherControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::SelfCheck:
            {
                SelfCheckControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<SelfCheckControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::Instrument:
            {
                InstrumentControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<InstrumentControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::Turntable:
            {
                TurntableControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<TurntableControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        case Type::PlayBack:
            {
                PlayBackControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<PlayBackControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
            }
            break;
        default:
            break;
    }

    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 查看任务详细信息
 * @details 直接调用修改的模块，即可查看，也可修改
 * @param[in] index 待查看行单元索引
 */
void TaskControl::viewTask(QModelIndex index)
{
    Q_D(TaskControl);
    showEditWindow(index);
}

/*!
 * @brief 删除指定任务
 * @return
 */
void TaskControl::deleteTask()
{
    Q_D(TaskControl);

    bool flag = false;
    QModelIndex index = currentIndex(flag);
    if(flag){
        QMessageBox::StandardButton butt = QMessageBox::information(this,tr("information"),tr("Delete selected task?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(butt = QMessageBox::Yes)
        {
            TaskInfo * deletedTask = d->taskInfoList.at(index.row());
            d->taskInfoList.removeAt(index.row());
            d->taskViewModel->updateTaskList(d->taskInfoList);
            delete deletedTask;
        }
    }
}

/*!
 * @brief 修改任务信息
 * @param[in]
 * @return
 */
void TaskControl::modifyTask()
{
    Q_D(TaskControl);
    bool flag = false;
    QModelIndex index = currentIndex(flag);
    if(flag){
        showEditWindow(index);
    }
}

/*!
 * @brief 添加新的任务信息
 * @details 根据触发此事件的对象类型，创建不同的输入提示框，可供用户填写。
 * @param[in]  无
 * @return 无
 */
void TaskControl::addNewTask()
{
    Q_D(TaskControl);
    Type tt =  static_cast<Type>(QObject::sender()->property(ACTION_TYPE).toInt());
    switch(tt)
    {
        case Type::Band:
            {
                BandControlDialog dialog(this);
                dialog.exec();
                BandControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::State:
            {
                StateControlDialog dialog(this);
                dialog.exec();
                StateControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Gather:
            {
                GatherControlDialog dialog(this);
                dialog.exec();
                GatherControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::SelfCheck:
            {
                SelfCheckControlDialog dialog(this);
                dialog.exec();
                SelfCheckControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Instrument:
            {
                InstrumentControlDialog dialog(this);
                dialog.exec();
                InstrumentControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Turntable:
            {
                TurntableControlDialog dialog(this);
                dialog.exec();
                TurntableControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::PlayBack:
            {
                PlayBackControlDialog dialog(this);
                dialog.exec();
                PlayBackControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        default:
            break;
    }

    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 复制任务信息
 * @details 复制时根据当前的任务类型，创建对应的任务信息，并将复制行的内容拷贝至暂存区；
 *          每次复制时删除上一次复制的缓冲区内容，防止出现内存泄漏问题；
 */
void TaskControl::copyTable()
{
    Q_D(TaskControl);
    bool flag = false;
    QModelIndex index = currentIndex(flag);
    if(flag){

    }
}

/*!
 * @brief 粘贴任务信息
 * @details 将复制的任务信息粘贴到当前行的下方
 */
void TaskControl::pasteTask()
{
    Q_D(TaskControl);
}

Type TaskControl::selectType(QModelIndex index)
{
    QString type = index.model()->index(index.row(), 1).data().toString();
    
    if (type.contains(QStringLiteral("频段控制"), Qt::CaseSensitive)) {
        return Type::Band;
    } else if (type.contains(QStringLiteral("状态控制"), Qt::CaseSensitive)) {
        return Type::State;
    } else if (type.contains(QStringLiteral("采集控制"), Qt::CaseSensitive)) {
        return Type::Gather;
    } else if (type.contains(QStringLiteral("自检控制"), Qt::CaseSensitive)) {
        return Type::SelfCheck;
    } else if (type.contains(QStringLiteral("仪器控制"), Qt::CaseSensitive)) {
        return Type::Instrument;
    } else if (type.contains(QStringLiteral("转台控制"), Qt::CaseSensitive)) {
        return Type::Turntable;
    } else if (type.contains(QStringLiteral("回放控制"), Qt::CaseSensitive)) {
        return Type::PlayBack;
    }
}

} //namespace TaskControlModel
