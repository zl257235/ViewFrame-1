/*!
 *  @details   udp通信接口
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.08.20
 *  @copyright NanJing RenGu.
 */
#include "SocketConnectUDP.h"
#include <QString>
#include <QDebug>
#include <QHostAddress>
#include <QTime>
#include <QCoreApplication>
#include "config/readxmlinfo.h"
#include "netspace/networknamespace.h"
#include "thread/udprecvthread.h"

SocketConnectUDP::SocketConnectUDP(QObject *parent):QObject(parent)
{
    //获取socket信息
    ReadXmlInfo readSocketInfo;
    m_listLocalInfo=readSocketInfo.getLocalInfoList();
    m_listTargetInfo=readSocketInfo.getTargetInfoList();
}

SocketConnectUDP::~SocketConnectUDP()
{
    if(m_recvThread)
    {
        m_recvThread->exit();
    }
    closeSocket();
    qDeleteAll(m_pListScocket);
    m_pListScocket.clear();
}

void SocketConnectUDP::openSocket()
{
    initSocket();

    m_recvThread=new UdpRecvThread();
    m_recvThread->start();
}

/*!
 * @brief 初始化udp,监听socket端口
 */
void SocketConnectUDP::initSocket()
{
    for(int i=0;i<m_listLocalInfo.size();i++)
    {
        QUdpSocket* pSocket= new QUdpSocket(this);
        m_pListScocket.append(pSocket);

        QHostAddress localAddr;
        unsigned short usLocalPort=m_listLocalInfo.at(i).iRecvPort;
        localAddr.setAddress(m_listLocalInfo.at(i).strIp);

        int ioptval = 1024*64;
        pSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,ioptval);
        pSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,ioptval);

        //设置TTL
        ioptval = 64;
        pSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,ioptval);

        // 设置组播回环
        ioptval = 1;
        pSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption,ioptval);

        //QObject::connect(pSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagranms()));
        bool result=pSocket->bind(localAddr,usLocalPort);
        if(!result)
        {
            qDebug()<<"udp bind error!";
            return;
        }
    }
}

/*!
 * @brief 关闭socket
 */
void SocketConnectUDP::closeSocket()
{
    for(int i=0;i<m_pListScocket.size();i++)
    {
         m_pListScocket.at(i)->close();
    }
}

/*!
 * @brief 发送UDP数据信息接口
 * @param[in] iSendDeviceId 发送设备ID
 * @param[in] iRecvDeviceId 接收设备ID
 * @param[in] pData 数据
 * @param[in] iDataLen 数据长度
 * @return 成功发送的数据报长度 -1表示无效
 */
int SocketConnectUDP::sendUdpData(const int &iSendDeviceId,const int &iRecvDeviceId,const char* pData,const int& iDataLen)
{
    QMutexLocker locker(&mutex);
    int ret=-1;
    if((pData==NULL)||(iDataLen<=0))
    {
        return ret;
    }
    /*
    if(strlen(pData)!=iDataLen)
    {
        qDebug()<<"pData's!=iDataLen "<<strlen(pData)<<" : "<<iDataLen;
        return ret;
    }*/

    bool blIdExsit=false;
    UdpHostInfo sendUdpInfo;
    QUdpSocket* pUdpSocket;
    for(int i=0;i<m_listLocalInfo.count();i++) //根据ID从链表中找到要发送至的socket信息
    {
        if(m_pListScocket.size()==m_listLocalInfo.size())
        {
            sendUdpInfo=m_listLocalInfo.at(i);
            if(sendUdpInfo.iDeviceId==iSendDeviceId)
            {
                pUdpSocket=m_pListScocket.at(i);
                blIdExsit=true;
                break;
            }
        }
    }

    if(!blIdExsit)  //未从列表中找到ID
    {
        return ret;
    }

    for(int i=0;i<m_listTargetInfo.count();i++) //根据ID从链表中找到要发送至的socket信息
    {
        sendUdpInfo=m_listTargetInfo.at(i);
        if(sendUdpInfo.iDeviceId==iRecvDeviceId)
        {
            blIdExsit=true;
            break;
        }
    }

    if(!blIdExsit)  //未从列表中找到ID
    {
        return ret;
    }

    int iMsgLen=0;
    unsigned short usPort=sendUdpInfo.iRecvPort;
    QHostAddress target;                         //QHostAddress::Broadcast
    target.setAddress(sendUdpInfo.strIp);
    if((iMsgLen=pUdpSocket->writeDatagram(pData,iDataLen,target,usPort))!=iDataLen)
    {
        qDebug()<<"udp writeDatagram error!,send len="<<iMsgLen;
    }
    ret=iMsgLen;
    return ret;
}

/*!
 * @brief 处理网络接收到的报文
 */
void SocketConnectUDP::processPendingDatagranms()
{
    while(true)
    {
        for(int i=0;i<m_pListScocket.size();i++)
        {
            QUdpSocket* pSocket= m_pListScocket.at(i);
            if(pSocket->isValid())
            {
                if(pSocket->hasPendingDatagrams())
                {
                   QByteArray datagram;
                   datagram.resize(pSocket->pendingDatagramSize());
                   QHostAddress srcAddress;
                   unsigned short srcPort;
                   pSocket->readDatagram(datagram.data(),datagram.size(),&srcAddress,&srcPort);
                   if(datagram.size()>0)
                   {
                       networkNameSpace::pCycleBuffClass->write(datagram.data(),datagram.size());
                       emit sendNetData(datagram.size());
                   }
                }
            }
        }

        QTime t;
        t.start();
        while(t.elapsed()<1)    //延时1秒
            QCoreApplication::processEvents();
    }
}
