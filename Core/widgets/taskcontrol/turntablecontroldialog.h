/*!
 *  @brief     转台控制信息窗口
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_TURNTABLECONTROL_H
#define RS_TURNTABLECONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class TurntableControlDialogPrivate;

class TurntableControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TurntableControlDialog)
public:
    explicit TurntableControlDialog(QWidget *parent = 0);
    ~TurntableControlDialog();
    
    TurntableControl * getWindowData();
    void setWindowData(TurntableControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<TurntableControlDialogPrivate> d_ptr;
    
};

} //namespace TaskControlModel

#endif // RS_TURNTABLECONTROL_H
