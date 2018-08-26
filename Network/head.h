/*!
 *  @brief     网络层协议文件
 *  @details   定义即时通讯软件网路层传输控制协议
 *  @file      head.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.02.23
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef HEAD_H
#define HEAD_H

#include <QByteArray>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#include <Windows.h>
#endif

#define IO_BUFF_SIZE  1024
#define MAX_RECV_SIZE 1024*8

#define MAX_PACKET 512                          /*!< 应用层数据最大发送长度，超过此长度后，网络层会进行拆包。 @attention 未加入网络层协议头， @link DataPacket @endlink */
#define MAX_SEND_BUFF (MAX_PACKET + 24)         /*!< 网络层最大发送长度 */

#define SOCK_CHAR_BUFF_LEN  32                  /*!< 存储ip地址集合长度 */

/*!
 *  @brief  接收/发送魔数
 *  @details 用于对接收数据的校验，服务器的接收与客户端的发送保持一致，服务器的发送与客户端的接收保持一致。
 */
#define RECV_MAGIC_NUM 0x7DBCD6AC
#define SEND_MAGIC_NUM 0xD7CB6DCA

#pragma pack(push)
#pragma pack(1)

/*!
 *  @brief 网络层数据传输控制头
 *  @details 在传输时网络层包裹了应用层的数据内容，用于即时通讯软件网络层交互时对大数据的包的分包传输以及组包，同时可对数据的正确性进行校验。
 */
struct DataPacket
{
    unsigned int magicNum;                   /*!< 魔数 @link RECV_MAGIC_NUM @endlink */
    unsigned int packId;                     /*!< 数据包身份，一个id至少对应一个数据帧。 */
    unsigned int currentIndex;               /*!< 当前帧索引 @attention 起始索引为0  */
    unsigned int totalIndex;                 /*!< 总帧数量 */
    unsigned int currentLen;                 /*!< 当前数据长度 */
    unsigned int totalLen;                   /*!< 总数据长度 */
};

/*!
 *  @brief 传输方式
 *  @details 数据从本机出去的方式包含TCP、UDP、总线，至于北斗等方式，也是UDP传输至中间某一台位后，再转发。
 */
enum CommMethod{
    C_NONE,     /*!< 错误方式*/
    C_UDP,      /*!< UDP方式 */
    C_TCP,      /*!< TCP方式 */
    C_BUS       /*!< 总线方式 */
};


/*!
 *  @brief 对数据进行协议组包的时候，必须要用到的外部描述信息集合
 *  @warning 【若此结构发生变动，同步修改SQL/Datatable/RChat716Cache类】
 *  @details 填写协议头部信息时，像包长这类的字段信息可以实时获取，但是像站点等类型的信息的无法在实时组包的时候获取
 *  @author shangchao
 */
struct ProtocolPackage
{
    QByteArray data;               /*!< 正文内容 */

    ProtocolPackage()
    {
    }

    ProtocolPackage(QByteArray dataArray){
        this->data = dataArray;
    }

    ProtocolPackage operator=(const ProtocolPackage & package)
    {
        this->data = package.data;
        return *this;
    }
};

/*!
 *  @brief 待发送数据单元描述，包含了发送时使用的方式，以及待发送的数据 \n
 *         ProtocolPackage可作为通用的数据发送格式，用户可在此结构体内部扩充信息。
 */
struct SendUnit
{
    SendUnit(){}
    SOCKET sockId;                  /*!< 客户端Socket标识Id */
    CommMethod method;              /*!< 发送方式 */
    ProtocolPackage dataUnit;       /*!< 待发送数据包及描述 */
};

/*!
 *  @brief  接收时由网络层提取出的信息字段，该结构信息可用于应用层进行数据处理
 *  @note   需要由网络层向应用层传递数据时，可在此结构体内添加字段，并在网络层中将对应的信息赋值即可。
 */
struct ExtendData
{
    ExtendData(){
        sockId = 0;
    }
    SOCKET sockId;                  /*!< 客户端Socket标识Id */
    CommMethod method;              /*!< 接收方式 */
};

/*!
 *  @brief 数据处理单元
 *  @details 封装了客户端的每个请求，服务器处理线程从列表中取出每个单元，根据应用层传输协议进行相应处理。
 */
struct RecvUnit
{
    ExtendData extendData;      /*!< 可扩充数据信息，包含网络层相关信息 */
    QByteArray data;            /*!< 请求数据内容，已经去除网络层( @link DataPacket @endlink )数据头，但其它应用层的数据格式头没有去除 */
};

#pragma pack(pop)

#endif // HEAD_H
