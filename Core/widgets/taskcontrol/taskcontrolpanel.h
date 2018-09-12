/*!
 *  @brief     任务控制模块
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKCONTROLPANEL_H
#define TASKCONTROLPANEL_H

#include "Base/component/rcomponent.h"

namespace TaskControlModel {

class TaskControl;

class TaskControlPanel : public RComponent
{
    Q_OBJECT
public:
    explicit TaskControlPanel(QWidget * parent = 0);
    ~TaskControlPanel();

    bool initialize();
    void release();

    void onMessage(MessageType::MessageType type);

    QSize sizeHint()const;

private:
    TaskControl * controlWidget;

};

} //namespace TaskControlModel

#endif // TASKCONTROLPANEL_H
