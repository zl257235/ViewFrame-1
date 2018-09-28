/*!
 *  @brief     频率控制
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_BANDCONTROL_H
#define RS_BANDCONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class BandControlDialogPrivate;

class BandControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BandControlDialog)
public:
    explicit BandControlDialog(QWidget *parent = 0);
    ~BandControlDialog();
    
    BandControl * getWindowData();
    void setWindowData(BandControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();
    
private:
    QSharedPointer<BandControlDialogPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // RS_BANDCONTROL_H
