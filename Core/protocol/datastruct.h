/*!
 *  @brief     类型定义
 *  @details   定义了通用的结构体、枚举等变量
 *  @file      datastruct.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.02
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QString>
#include <QMap>
#include <QVector>

namespace Datastruct {

/*!
 *  @brief  系统通知消息的类型
 *  @details 程序内部提供了系统级别的通知-订阅模式，此数据类型用于区分广播的类型。订阅者一句类型进行对应的处理。
 */
enum MessageType
{
    MESS_BASEINFO_READY,                 /*!< 基本信息可读 */
    MESS_DATABASEINFO_READY              /*!< 数据库信息可读 */
};

/*!
 *  @brief 系统支持的服务类型
 *  @details 用户以命令行方式输入对应的服务.
 */
enum ServiceType
{
    SERVICE_TEXT,               /*!< 使用传输文本服务,默认类型 */
    SERVICE_FILE                /*!< 使用传输文件服务 */
};

/*!
 *  @brief 传输模式
 */
enum TransmissionMode
{
    TRANS_NET,              /*!< 网络传输方式 */
    TRANS_TEL               /*!< 电话传输方式 */
};

/*!
 *  @brief 数据库类型
 */
enum DatabaseType
{
    DB_NONE = 0,
    DB_MYSQL,               /*!< 使用MySql数据库*/
    DB_ORACLE               /*!< 使用Oracle数据库*/
};

/*!
 * @brief 解析结果
 */
enum ResultType
{
    PARSE_ERROR,            /*!< 发生错误*/
    VIEW_PROGRAM,           /*!< 查看程序信息*/
    EXEC_PROGRAM            /*!< 执行程序*/
};

struct CommandParameter
{
    CommandParameter()
    {
        serviceType = SERVICE_TEXT;
        transMode = TRANS_NET;
        dbType = DB_MYSQL;
        parseResult = PARSE_ERROR;
    }

    ServiceType serviceType;
    TransmissionMode  transMode;
    DatabaseType dbType;
    ResultType parseResult;
};


/*!
 *  @brief 数据库特征信息
 *  @note 保存当前使用的数据库中的功能特点
 */
struct DBFeature
{
    bool lastInsertId;          /*!< 是否支持查询最后插入数据的id */
    bool transactions;          /*!< 是否支持事务 */
};

/*!
 *  @brief 配置文件参数
 */
struct SettingConfig
{
    SettingConfig()
    {
        textRecvProcCount = 5;
        textSendProcCount = 5;
        textListenPort = 8023;
        fileListenPort = 8024;
        textIp = "127.0.0.1";
        fileIp = "127.0.0.1";
    }
    int textRecvProcCount;
    int textSendProcCount;
    ushort textListenPort;
    ushort fileListenPort;

    QString textIp;
    QString fileIp;

    QString uploadFilePath;
};

/*!
 *  @brief 数据库配置信息
 */
struct DatabaseConfigInfo
{
    DatabaseType dbType;
    QString hostName;
    QString dbName;
    QString dbUser;
    QString dbPass;
    ushort port;
};

} //namespace Datastruct

#endif // DATASTRUCT_H
