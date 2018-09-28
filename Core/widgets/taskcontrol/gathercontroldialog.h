/*!
 *  @brief     采集控制信息窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_GATHERCONTROL_H
#define RS_GATHERCONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class GatherControlDialogPrivate;

class GatherControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GatherControlDialog)
public:
    explicit GatherControlDialog(QWidget *parent = 0);
    ~GatherControlDialog();
    
    GatherControl *getWindowData();
    void setWindowData(GatherControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<GatherControlDialogPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // RS_GATHERCONTROL_H
