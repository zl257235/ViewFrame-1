/*!
 *  @brief     回放控制信息窗口
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_PLAYBACKCONTROL_H
#define RS_PLAYBACKCONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class PlayBackControlDialogPrivate;

class PlayBackControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PlayBackControlDialog)
public:
    explicit PlayBackControlDialog(QWidget *parent = 0);
    ~PlayBackControlDialog();
    
    PlayBackControl * getWindowData();
    void setWindowData(PlayBackControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<PlayBackControlDialogPrivate> d_ptr;
};

} // namespace TaskControlModel

#endif // RS_PLAYBACKCONTROL_H
