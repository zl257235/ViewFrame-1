#ifndef USERTYPEDEF_H
#define USERTYPEDEF_H

#include <QString>
#include <QList>
using namespace std;

const QString SOCKET_XML_PATH="./user/socket.xml";  /*! SOCKET_XML_PATH xml文件路劲*/
const int CYCLE_BUFFER_SIZE=1024*1024*4;            /*! CYCLE_BUFFER_SIZE 环形缓冲区大小，4M*/
/*!
 * @brief udp主机信息结构体
 */
struct UdpHostInfo
{
    int iDeviceId;               /*!< iDeviceId 设备ID */
    int iInfoType;               /*!< iInfoType 信息类型 1：健康管理 2：应用数据 */
    QString strIp;               /*!< strIp 端口号 */
    int iSendPort;               /*!< iSendPort 发送端口号 */
    int iRecvPort;               /*!< iRecvPort 接收端口号 */
    bool operator==(const UdpHostInfo& info)        /*!< == 重载运算符 */
    {
        bool bl=(info.iDeviceId==iDeviceId)&&(info.iInfoType==iInfoType)&&(info.strIp==strIp)
                &&(info.iSendPort==iSendPort)&&(info.iRecvPort==iSendPort);
        return bl;
    }
};

#endif // USERTYPEDEF_H
