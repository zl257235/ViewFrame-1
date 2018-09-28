#include "TableViewModel.h"

#include <QDebug>
#include "Base/util/rsingleton.h"

namespace TaskControlModel {

TableViewModel::TableViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

TableViewModel::~TableViewModel()
{

}

int TableViewModel::rowCount(const QModelIndex & index) const
{
    return taskList.count();
}

int TableViewModel::columnCount(const QModelIndex & index) const
{
    return headInfo.size();
}

//数据更改后将新数据重新填充至表格，并更新链表
bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int col = index.column();
    int row = index.row();
    
//    TaskInfo taskInfo = m_taskInfo.at(row);
    
//    switch (role) {
//    case Qt::EditRole:
//        /*if (col == NO)
//        {
//            taskInfo.no = value.toInt();
//            m_taskInfo.replace(row, taskInfo);
//            emit dataChanged(index, index);
//            break;
//        } else if (col == TYPE) {
//            taskInfo.type = (Type)value.toInt();
//            emit dataChanged(index, index);
//            m_taskInfo.replace(row, taskInfo);
//            break;
//        } else */if (col == PARAMENTER) {
//            taskInfo.parameter = value.toString();
//            m_taskInfo.replace(row, taskInfo);
//            emit dataChanged(index, index);
//            break;
//        } else if (col == EXCUTIONTTIME) {
//            taskInfo.taskExcutionTime = value.toString();
//            m_taskInfo.replace(row, taskInfo);
//            emit dataChanged(index, index);
//            break;
//        } else if (col == EXCUTIONTTIMELONG) {
//            taskInfo.taskExcutionTimeLong = value.toString();
//            m_taskInfo.replace(row, taskInfo);
//            emit dataChanged(index, index);
//            break;
//        }
//        break;
//    case Qt::DisplayRole:
//    case Qt::CheckStateRole:
//    case Qt::UserRole:
//        break;
//    default:
//        return false;
//    }
    return true;
}

//返回一个项的任意角色的值，这个项被指定为QModelIndex
QVariant TableViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= taskList.size())
        return QVariant();

    int row = index.row();
    switch(role)
    {
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;

        case Qt::DisplayRole:
            {
                switch (static_cast<TaskHead>(index.column())) {
                    case T_No:
                        return QString("%1").arg(row+1);
                        break;
                    case T_TYPE:
                        {
                            switch(taskList.at(row)->taskType){
                                case Type::Band:
                                        return QObject::tr("Band control");
                                case Type::State:
                                        return QObject::tr("State control");
                                case Type::Gather:
                                        return QObject::tr("Gather control");
                                case Type::SelfCheck:
                                        return QObject::tr("Selfcheck control");
                                case Type::Instrument:
                                        return QObject::tr("Instrument Control");
                                case Type::Turntable:
                                        return QObject::tr("Turning table control");
                                case Type::PlayBack:
                                        return QObject::tr("Playback control");
                                default:
                                    return QVariant("");
                            }
                        }
                        break;
                    case T_PARAMETERS:
                        return taskList.at(row)->parameter;
                        break;
                    case T_E_TIME:
                        return taskList.at(row)->excuteTime;
                        break;
                    case T_E_TIME_LONG:
                        return taskList.at(row)->lastTime;
                        break;
                }
                    }
        break;

        default:
            break;
    }

    return QVariant();
}

QVariant TableViewModel::headerData(int section,Qt::Orientation orientation,int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    
    if (Qt::Horizontal == orientation && section < headInfo.size()){
        return headInfo.at(section);
    }
        
    return QVariant();
}

Qt::ItemFlags TableViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    switch (static_cast<TaskHead>(index.column())) {
        case T_PARAMETERS:
        case T_E_TIME:
        case T_E_TIME_LONG:
                return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
            break;
        default:
            break;
    }
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TableViewModel::updateTaskList(TaskInfoList &list)
{
    taskList = list;
    resetData();
}

void TableViewModel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
                retranslateUi();
                resetData();
        break;
        default:
            break;
    }
}

void TableViewModel::resetData()
{
    beginResetModel();
    endResetModel();
}

void TableViewModel::retranslateUi()
{
    headInfo.clear();
    headInfo<<QObject::tr("Index")<<QObject::tr("Type")<<QObject::tr("Parameter")
                  <<QObject::tr("Dispatch Time")<<QObject::tr("Execute Time");
}

} //namespace TaskControlModel
