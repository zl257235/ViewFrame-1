#ifndef READXMLINFO_H
#define READXMLINFO_H

#include <QObject>
#include "usertypedef.h"

class ReadXmlInfo
{
public:
    ReadXmlInfo();

public:
    bool readSocketXmlInfo();

    QList<UdpHostInfo> getLocalInfoList();
    QList<UdpHostInfo> getTargetInfoList();

private:
    bool addToList(QList<UdpHostInfo>& listInfo,const UdpHostInfo& info);

private:
    //UdpHostInfo m_localHostInfo;            //本机地址
    QList<UdpHostInfo> m_listLocalInfo;     //本机信息列表
    QList<UdpHostInfo> m_listTargetInfo;    //目标机信息列表
};

#endif // READXMLINFO_H
