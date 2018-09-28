/*!
 *  @details   udp通信模块接口
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.08.28
 *  @copyright NanJing RenGu.
 */
#include "networkinterface.h"
#include "netspace/networknamespace.h"
#include "connect/socketconnectudp.h"
#include <QDebug>


NetworkInterface::NetworkInterface(QObject *parent):QObject(parent)
{
    networkNameSpace::pCycleBuffClass=new CCycleBuffer(CYCLE_BUFFER_SIZE);
    networkNameSpace::pSocketClass=new SocketConnectUDP(this);
    if(networkNameSpace::pSocketClass)  //启动socket
    {
        networkNameSpace::pSocketClass->openSocket();
        connect(networkNameSpace::pSocketClass,SIGNAL(sendNetData(int)),this,SLOT(recvNetData(int)));
    }
}

NetworkInterface::~NetworkInterface()
{
    if(networkNameSpace::pSocketClass)
    {
        delete networkNameSpace::pSocketClass;
    }
    if(networkNameSpace::pCycleBuffClass)
    {
        delete networkNameSpace::pCycleBuffClass;
    }
}


void NetworkInterface::recvNetData(int iLen)
{
    if(iLen>0)
    {
        //qDebug()<<"interface:"<<iLen;
        emit signalTransmit(iLen);
    }
}

/*!
 * @brief 发送数据接口
 * @param[in] iSendDeviceId 发送设备ID
 * @param[in] iRecvDeviceId 接收设备ID
 * @param[in] pData 数据
 * @param[in] iDataLen 数据长度
 * @return 成功发送的数据报长度 -1表示无效
 * @return 发送报文实际长度
 */
int NetworkInterface::sendData(const int &iSendDeviceId,const int &iRecvDeviceId,const char* pData,const int& iDataLen)
{
    int sendLen=0;
    if(networkNameSpace::pSocketClass)
    {
        sendLen=networkNameSpace::pSocketClass->sendUdpData(iSendDeviceId,iRecvDeviceId,pData,iDataLen);
    }

    return sendLen;
}

/*!
 * @brief 从缓冲区读数据
 * @param[out] pBuf 获取到的数据
 * @param[in] iDataLen 数据长度
 * @return 实际读取的字节数
 */
int NetworkInterface::recvData(char* pBuf,const int& iDataLen)
{
    int recvLen=0;
    if(networkNameSpace::pCycleBuffClass)
    {
        recvLen=networkNameSpace::pCycleBuffClass->read(pBuf,iDataLen);
    }
    return recvLen;
}
