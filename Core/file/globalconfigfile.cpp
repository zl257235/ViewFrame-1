#include "globalconfigfile.h"

#include "Base/util/rutil.h"
#include "Base/util/rlog.h"
#include "Base/util/rsingleton.h"
#include "constants.h"

GlobalConfigFile::GlobalConfigFile():settings(nullptr)
{

}

bool GlobalConfigFile::parseFile()
{
    if(settings == nullptr)
        return false;

    //解析日志模块
    logConfig.isRecord2File = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_RECORDFILE,true).toBool();
    logConfig.level = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_LEVEL,RLog::RINFO).toInt();

    //数据库配置
    databaseConfig.hostName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_HOST,databaseConfig.hostName).toString();
    databaseConfig.dbName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_DATABASE_NAME,databaseConfig.dbName).toString();
    databaseConfig.dbUser = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_USERNAME,databaseConfig.dbUser).toString();
    databaseConfig.dbPass = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PASSWORD,databaseConfig.dbPass).toString();
    databaseConfig.port = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PORT,databaseConfig.port).toInt();

    return true;
}
