#include "taskcontrolpanel.h"

#include "Base/constants.h"
#include "taskcontrol.h"

using namespace TaskControlModel;

TaskControlPanel::TaskControlPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_TASK_CONTROL,parent),controlWidget(nullptr)
{
    m_name = tr("TaskControl panel");
}

TaskControlPanel::~TaskControlPanel()
{

}

bool TaskControlPanel::initialize()
{
    controlWidget = new TaskControl(this);
    setWidget(controlWidget);

    return true;
}

void TaskControlPanel::release()
{

}

void TaskControlPanel::onMessage(MessageType::MessageType type)
{

}

QSize TaskControlPanel::sizeHint() const
{
     return QSize(350,200);
}
