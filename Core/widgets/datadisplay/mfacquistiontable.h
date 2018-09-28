﻿/*!
 *  @details   中频数据表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
 */
#ifndef MFACQUISTIONTABLE_H
#define MFACQUISTIONTABLE_H

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {
class MFAcquistionTablePrivate;

class MFAcquistionTable : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MFAcquistionTable)
public:
    explicit MFAcquistionTable(QWidget *parent = 0);
    ~MFAcquistionTable();

    bool initialize();
    void release();
    void onMessage(MessageType::MessType type);

public slots:
    void recvMFAcquistionPara(char *buff, int len);
    void clearTable();

private:
    void initMFAcquistionTable();
    void retranslateUi();
    QString getCurrentDate();

private:
    MFAcquistionTablePrivate * d_ptr;
};

} //namespace DataView

#endif // MFACQUISTIONTABLE_H
