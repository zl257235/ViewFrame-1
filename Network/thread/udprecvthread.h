#ifndef UDPRECVTHREAD_H
#define UDPRECVTHREAD_H

#include <QThread>
class UdpRecvThread:public QThread
{
    Q_OBJECT
public:
    UdpRecvThread();

protected:
    void run();
};

#endif // UDPRECVTHREAD_H
