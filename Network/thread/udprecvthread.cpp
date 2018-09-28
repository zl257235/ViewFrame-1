/*!
 *  @details   接收udp数据线程
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.08.25
 *  @copyright NanJing RenGu.
 */
#include "udprecvthread.h"
#include "netspace/networknamespace.h"
#include <QDebug>
UdpRecvThread::UdpRecvThread()
{

}

void UdpRecvThread::run()
{
    if(networkNameSpace::pSocketClass)
    {
        networkNameSpace::pSocketClass->processPendingDatagranms();
    }
}
