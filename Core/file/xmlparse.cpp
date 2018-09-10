#include "xmlparse.h"

#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QTextCodec>
#include <QDebug>

#include "Base/util/rlog.h"
#include "Base/util/scaleswitcher.h"
#include "protocol/datastruct.h"

class RouteDom{
public:
    explicit RouteDom():rootNode("Route"),baseInfoNode("Baseinfo"),
    serverNode("Server"),clientNode("Client"),nodeId("nodeId"),ip("ip"),
    lon("lon"),lat("lat"),node("node"),port("port"),commethod("commethod"),messageFormat("messageFormat"),
    server("server"),channel("channel"),method("method"),format("format"),type("type"){

    }

    QString rootNode;
    QString baseInfoNode;
    QString serverNode;
    QString clientNode;

    QString nodeId;
    QString ip;
    QString lon;
    QString lat;

    QString node;
    QString port;
    QString commethod;
    QString messageFormat;
    QString server;
    QString channel;
    QString method;
    QString format;
    QString type;
};

XMLParse::XMLParse(QObject *parent):QObject(parent)
{

}

/*!
 * @brief 解析XX配置信息
 * @param[in] fileName 待解析的文件名
 * @param[in] routeSettings 解析后保存的结构体
 * @return 是否解析成功
 */
bool XMLParse::parseRouteSettings(const QString &fileName)
{
    QDomDocument document("document");
    if(!validateParseFile(fileName,document))
        return false;

    RouteDom routeDom;
    QDomElement rootDom = document.documentElement();


    //此处用于解析文件

    return true;
}

/*!
 * @brief 保存更新后的路由表
 * @param[in] fileName 待保存的文件名
 * @param[in] routeSettings 待保存的路由信息
 * @return  true保存成功，false保存失败
 */
bool XMLParse::saveRouteSettings(const QString &fileName)
{
    RouteDom routeDom;

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForLocale());

    QDomDocument doc("RouteTable");

    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement(routeDom.rootNode);
    doc.appendChild(root);

    //此处用于创建节点

    doc.save(stream,4);

    return true;
}

/*!
 * @brief 验证待解析的配置文件
 * @param[in] fileName 待解析的文件名
 * @return 是否验证通过
 */
bool XMLParse::validateParseFile(const QString &fileName,QDomDocument &document)
{
    QFileInfo fileInfo(fileName);
    if(!fileInfo.exists()){
        RLOG_ERROR("Configure file not existed!");
        return false;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        return false;
    }

    if(!document.setContent(file.readAll())){
        RLOG_ERROR("Read configure file error!");
        file.close();
        return false;
    }

    return true;
}

XMLParse::~XMLParse()
{

}
