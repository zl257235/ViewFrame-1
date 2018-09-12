#include "TableViewDelegate.h"

#include "QDateTimeEdit"
#include "QDateTime"
#include <QDebug>

#include "../head.h"

namespace TaskControlModel {

TableViewDelegate::TableViewDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
TableViewDelegate::~TableViewDelegate()
{
}


QWidget *TableViewDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if (index.column() == TaskHead::T_E_TIME)
    {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        
        QDateTimeEdit *datetime = new QDateTimeEdit(parent);
        connect(datetime, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
        datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        datetime->setCalendarPopup(true); // 日历弹出
        datetime->setMaximumDateTime(QDateTime::fromString("6000-12-12 23::59:59","yyyy-MM-dd HH:mm:ss"));
        datetime->setMinimumDateTime(QDateTime::currentDateTime());
        datetime->setDateTime(QDateTime::fromString(value,"yyyy-MM-dd HH:mm:ss"));
        
        return datetime;
    }
    else
        return QItemDelegate::createEditor(parent,option,index);
}

void TableViewDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    if (index.column() == TaskHead::T_E_TIME)
    {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        
        QDateTimeEdit *datetime = static_cast<QDateTimeEdit *>(editor);
        datetime->setDateTime(QDateTime::fromString(value, "yyyy-MM-dd HH:mm:ss"));
    }
    else
        return QItemDelegate::setEditorData(editor,index);
}

void TableViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    if (index.column() == TaskHead::T_E_TIME)
    {
        QDateTimeEdit *datetime = static_cast<QDateTimeEdit *>(editor);
        datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        
        model->setData(index, datetime->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
    }
    else
        return QItemDelegate::setModelData(editor,model,index);
}

void TableViewDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    Q_UNUSED(index);
    if (index.column() == TaskHead::T_E_TIME)
    {
        editor->setGeometry(option.rect);
    }
    else
        return QItemDelegate::updateEditorGeometry(editor,option,index);
}

void TableViewDelegate::commitAndCloseEditor()
{
    QDateTimeEdit *datetimeedit = qobject_cast<QDateTimeEdit *>(sender());
    emit commitData(datetimeedit);
    emit closeEditor(datetimeedit);
}

} //namespace TaskControlModel

