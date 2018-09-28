/*!
 *  @brief     状态控制信息窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_STATECONTROL_H
#define RS_STATECONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class StateControlDialogPrivate;

class StateControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(StateControlDialog)
public:
    explicit StateControlDialog(QWidget *parent = 0);
    ~StateControlDialog();
    
    StateControl *getWindowData();
    void setWindowData(StateControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<StateControlDialogPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // RS_STATECONTROL_H
