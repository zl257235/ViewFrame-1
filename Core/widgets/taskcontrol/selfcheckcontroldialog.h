/*!
 *  @brief     自检控制信息窗口
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_SELFCHECKCONTROL_H
#define RS_SELFCHECKCONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class SelfCheckControlDialogPrivate;

class SelfCheckControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SelfCheckControlDialog)
public:
    explicit SelfCheckControlDialog(QWidget *parent = 0);
    ~SelfCheckControlDialog();
    
    SelfCheckControl  *getWindowData();
    void setWindowData(SelfCheckControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<SelfCheckControlDialogPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // RS_SELFCHECKCONTROL_H
