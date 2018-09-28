#ifndef SocketConnectUDP_H
#define SocketConnectUDP_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include "config/usertypedef.h"


class UdpRecvThread;
class SocketConnectUDP:public QObject
{
    Q_OBJECT
public:
    SocketConnectUDP(QObject* parent=NULL);
    virtual ~SocketConnectUDP();

    void openSocket();
    void initSocket();
    void closeSocket();
    int sendUdpData(const int &iSendDeviceId,const int &iRecvDeviceId,const char* pData,const int& iDataLen);

signals:
    void sendNetData(int iLen);

public:
    void processPendingDatagranms();

public:

    QList<UdpHostInfo> m_listLocalInfo;     //本机信息列表
    QList<UdpHostInfo> m_listTargetInfo;    //目标机地址

    //friend class UdpRecvThread;           //友元类
private:
    UdpRecvThread* m_recvThread;
    QList<QUdpSocket*>  m_pListScocket;     //udp对象指针
    QMutex mutex;                           //互斥量

};

#endif // SocketConnectUDP_H
