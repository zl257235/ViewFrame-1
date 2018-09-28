/*!
 *  @brief    仪器控制信息窗口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RS_INSTRUMENTCONTROL_H
#define RS_INSTRUMENTCONTROL_H

#include <QSharedPointer>
#include "head.h"
#include "Base/selfwidget/dialogproxy.h"

namespace TaskControlModel {

class InstrumentControlDialogPrivate;

class InstrumentControlDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(InstrumentControlDialog)
public:
    explicit InstrumentControlDialog(QWidget *parent = 0);
    ~InstrumentControlDialog();
    
    InstrumentControl * getWindowData();
    void setWindowData(InstrumentControl * info);

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    QSharedPointer<InstrumentControlDialogPrivate> d_ptr;
};

} // namespace TaskControlModel

#endif // RS_INSTRUMENTCONTROL_H
