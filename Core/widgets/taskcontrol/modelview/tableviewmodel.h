#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>

#include "../task.h"

namespace TaskControlModel {

struct Task;

class TableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableViewModel(QObject *parent = 0);
    ~TableViewModel();
    
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    inline virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void updateTaskList(TaskInfoList & list);
  
public:
    enum ColumnType
    {
        NO,                 //状态
        TYPE,               //数据类型
        PARAMENTER,         //任务参数
        EXCUTIONTTIME,      //任务执行时间
        EXCUTIONTTIMELONG   //任务执行时长
    };

private:
    void resetData();
    
private:
    QStringList headInfo;
    TaskInfoList taskList;
};

} //namespace TaskControlModel

#endif // TABLEVIEWMODEL_H
