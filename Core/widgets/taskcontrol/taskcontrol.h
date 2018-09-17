#ifndef TASKCONTROL_H
#define TASKCONTROL_H

#include <QSharedPointer>
#include <QModelIndex>
#include <QWidget>

#include "head.h"
#include "Base/pluginmanager/observer.h"

class QTimerEvent;

namespace TaskControlModel {

class TaskControlPrivate;

class TaskControl : public QWidget , public Base::Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TaskControl)
public:
    explicit TaskControl(QWidget *parent = 0);
    ~TaskControl();

    void retranslateUi();
    void onMessage(MessageType::MessageType type);
    
private slots:
    void viewTask(QModelIndex); /*!< 查看任务信息 */
    void modifyTask();          /*!< 修改任务信息 */
    void deleteTask();          /*!< 删除任务信息 */
    void copyTable();           /*!< 复制任务信息 */
    void pasteTask();           /*!< 粘贴任务信息 */

    void addNewTask();          /*!< 添加任务 */
    void distributeTask();      /*!< 下发任务 */

    void tableContextPoint(QPoint point);

    virtual void timerEvent(QTimerEvent *event);
    
private:
    Type selectType(QModelIndex);
    void initNetWork();
    QModelIndex currentIndex(bool &validIndex);
    void showEditWindow(QModelIndex index);

private:
    QSharedPointer<TaskControlPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // TASKCONTROL_H
