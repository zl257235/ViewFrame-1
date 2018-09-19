/*!
 *  @details   数据源表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.10
 *  @copyright NanJing RenGu.
 */
#ifndef RADIATIONSOURCETABLE_H
#define RADIATIONSOURCETABLE_H

#include "protocol/datastruct.h"
using namespace Datastruct;

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {
class RadiationSourceTablePrivate;

class RadiationSourceTable : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RadiationSourceTable)
public:
    explicit RadiationSourceTable(QWidget *parent = 0);
    ~RadiationSourceTable();

    /*!
     *  @brief 数据源刷新方式
     */
    enum DataRefreshModel
    {
        SCROLL_RENOVATE = 1,            /*!< 滚动刷新 */
        COVER_RENOVATE                  /*!< 覆盖刷新 */
    };

    bool initialize();
    void release();
    void onMessage(MessageType::MessType type);

public slots:
    void recvRSPara(char *buff,int len);

private slots:
    void on_radioButtonScrollReno_clicked();
    void on_radioButtonCoverReno_clicked();

private:
    void initRSTable();
    void changeTableHeaderInfo(bool blAddColFlag);
    void showTSPara(const RadiationSourceBase& rsData);
    QString getCurrentDate();
    void retranslateUi();

private:
    RadiationSourceTablePrivate * d_ptr;
};

} //namespace DataView

#endif // RADIATIONSOURCETABLE_H
