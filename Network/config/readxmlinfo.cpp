/*!
 *  @details   读取udp通信配置文件
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.08.24
 *  @copyright NanJing RenGu.
 */
#include "readxmlinfo.h"
#include <QFile>
#include <QtXml/QtXml>
#include <QtXml/QDomDocument>

ReadXmlInfo::ReadXmlInfo()
{
    m_listLocalInfo.clear();
    m_listTargetInfo.clear();
    readSocketXmlInfo();
}

/*!
 * @brief 获取本机设备配置信息
 * @return 本机设备配置信息
 */
QList<UdpHostInfo> ReadXmlInfo::getLocalInfoList()
{
    return m_listLocalInfo;
}

/*!
 * @brief 获取目标设备配置信息
 * @return 目标设备配置信息列表
 */
QList<UdpHostInfo> ReadXmlInfo::getTargetInfoList()
{
    return m_listTargetInfo;
}

/*!
 * @brief 读取XML配置信息
 * @return 读取信息是否成功
 */
bool ReadXmlInfo::readSocketXmlInfo()
{
    QFile file(SOCKET_XML_PATH);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        qDebug()<<" open "<<SOCKET_XML_PATH<<" error";
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug()<<" setContent error";
        file.close();
        return false;
    }
    file.close();
    QDomElement root=doc.documentElement();
    if(root.tagName()!="SocketInfo")
    {
        qDebug()<<root.tagName();
        return false;
    }
    QDomNode node=root.firstChild();

    while(!node.isNull())
    {
        QDomElement element=node.toElement();
        if(element.nodeName()=="LocalInfo")  //获取本机地址及端口信息
        {
            QDomNodeList localList=element.childNodes();
            for(int j=0;j<localList.count();j++)
            {
                QDomNode hostNode=localList.at(j);

                if(hostNode.nodeName()=="host")
                {
                    QDomElement hostElement=hostNode.toElement();
                    UdpHostInfo localInfo;

                    if(hostElement.hasAttribute("deviceId"))
                    {
                        localInfo.iDeviceId=hostElement.attribute("deviceId").toInt();
                    }

                    if(hostElement.hasAttribute("infoType"))
                    {
                        localInfo.iInfoType=hostElement.attribute("infoType").toInt();
                    }

                    if(hostElement.hasAttribute("ip"))
                    {
                        localInfo.strIp=hostElement.attribute("ip");
                    }

                    if(hostElement.hasAttribute("sendPort"))
                    {
                        localInfo.iSendPort=hostElement.attribute("sendPort").toInt();
                    }

                    if(hostElement.hasAttribute("recvPort"))
                    {
                        localInfo.iRecvPort=hostElement.attribute("recvPort").toInt();
                    }

                    //m_listLocalInfo.append(localInfo);
                    addToList(m_listLocalInfo,localInfo);
                }
            }
        }
        else if(element.nodeName()=="TargetInfo")       //获取目标地址及端口信息
        {
            QDomNodeList targetList=element.childNodes();
            for(int j=0;j<targetList.count();j++)
            {
                QDomNode hostNode=targetList.at(j);
                if(hostNode.nodeName()=="host")
                {
                    QDomElement hostElement=hostNode.toElement();
                    UdpHostInfo targetInfo;

                    if(hostElement.hasAttribute("deviceId"))
                    {
                        targetInfo.iDeviceId=hostElement.attribute("deviceId").toInt();
                    }

                    if(hostElement.hasAttribute("infoType"))
                    {
                        targetInfo.iInfoType=hostElement.attribute("infoType").toInt();
                    }

                    if(hostElement.hasAttribute("ip"))
                    {
                        targetInfo.strIp=hostElement.attribute("ip");
                    }

                    if(hostElement.hasAttribute("sendPort"))
                    {
                        targetInfo.iSendPort=hostElement.attribute("sendPort").toInt();
                    }

                    if(hostElement.hasAttribute("recvPort"))
                    {
                        targetInfo.iRecvPort=hostElement.attribute("recvPort").toInt();
                    }
                    //m_listTargetInfo.append(targetInfo);
                    addToList(m_listTargetInfo,targetInfo);
                }
            }
        }
        node=node.nextSibling();
    }
    return true;
}

/*!
 * @brief 添加成员到链表中
 * @param[in] lisInfo 链表
 * @param[in] info 数据成员
 */
bool ReadXmlInfo::addToList(QList<UdpHostInfo>& listInfo,const UdpHostInfo& info)
{
    for(int i=0;i<listInfo.size();i++)
    {
        UdpHostInfo infoIndex=listInfo.at(i);
        if(infoIndex==info)
        {
            return false;
        }
    }
    listInfo.append(info);
    return true;
}
