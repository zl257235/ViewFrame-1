#ifndef NETWORK_H
#define NETWORK_H
#include <QObject>
#include "network_global.h"


class NETWORKSHARED_EXPORT NetworkInterface: public QObject
{
    Q_OBJECT

public:
    NetworkInterface(QObject* parent=NULL);
    ~NetworkInterface();

    int sendData(const int &iSendDeviceId, const int &iRecvDeviceId, const char *pData, const int &iDataLen);
    int recvData(char* pBuf,const int& iDataLen);

signals:
    void signalTransmit(int iPara);

private slots:
    void recvNetData(int iLen);

};

#endif // NETWORK_H
