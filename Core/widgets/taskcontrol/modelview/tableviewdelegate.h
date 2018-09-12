#ifndef TableViewDelegate_H
#define TableViewDelegate_H

#include <QItemDelegate>
#include "../head.h"

namespace TaskControlModel {

class TableViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableViewDelegate(QObject *parent = 0);
    ~TableViewDelegate();
    
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model, 
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    
private slots:
    void commitAndCloseEditor();
};

} //namespace TaskControlModel

#endif // TableViewDelegate_H
