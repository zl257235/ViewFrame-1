/*!
 *  @brief     常量汇总
 *  @details   定义了软件运行时所需的常量信息，包括(图片、控件ID等)
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constant {

/*日期*/
const char Date_Simple[] = "yyyy-MM-dd";

/*文件路径*/
const char AppShortName[] = "Rim";
const char ApplicationName[] = "ViewFrame";
const char Version[] = "1.1.0";
const char PATH_ConfigPath[] = "/../config";
const char PATH_LocalePath[] = "/translations";
const char PATH_StylePath[] = "/style";
const char PATH_SystemIconPath[] = "/systemIcon";
const char CONFIG_LocalePath[] = "/translations";

/*************************************************<配置文件>******************************************************/

/*系统设置*/
const char GroupNetwork[] = "Network";
const char DB_THREAD[] = "MessageRecvProcCount";
const char MSG_THREAD[] = "MessageSendProcCount";
const char TEXT_PORT[] = "TextServicePort";
const char FILE_PORT[] = "FileServicePort";
const char TEXT_IP[] = "TextServiceIp";
const char FILE_IP[] = "FileServiceIp";

//日志模块
const char LOG_LOG[] = "log";
const char LOG_LOG_RECORDFILE[] = "log2File";
const char LOG_LOG_LEVEL[] = "logLevel";

//数据库模块
const char DB_SETTING[] = "Database";
const char DB_TYPE[] = "Dbtype";
const char DB_HOST[] = "Hostname";
const char DB_DATABASE_NAME[] = "DatabaseName";
const char DB_USERNAME[] = "UserName";
const char DB_PASSWORD[] = "Password";
const char DB_PORT[] = "Port";

/*************************************************</配置文件>******************************************************/


const char SELECTED_LINE_COLOR[] = "rgb(34, 175, 75)";              /*!< 选中行的背景颜色 */

//插件
const char PLUGIN_TASK_CONTROL[] = "Rimserver.Plugin.TaskControl";
const char PLUGIN_HEALTH_MANAGER[] = "Rimserver.Plugin.HealthManager";
const char PLUGIN_DATA_VIEW[] = "Rimserver.Plugin.DataView";

//菜单栏
const char MENU_BAR[] = "Rimserver.MenuBar";

//菜单栏分组
const char G_PROGRAM[]  = "Rimserver.Group.Program";
const char G_SETTING[]  = "Rimserver.Group.Setting";
const char G_HELP[]     = "Rimserver.Group.Help";
const char DEFAULT_GROUP[] = "Rimserver.Default.Group";

//G_PROGRAM 分组
const char PROGRAM_EXIT[] = "Rimserver.Program.Exit";

//Setting 分组
const char TOP_HINT[] = "Rimserver.Setting.TopHint";
const char FULL_SCREEN[] = "Rimserver.Setting.FullScreen";
const char CUSTOM_STYLE[] = "Rimserver.Setting.CustomStyle";
const char SYSTEM_LANGUAGE[] = "Rimserver.Setting.SystemLanguage";

//G_HELP 分组
const char TEC_SUPPORT[] = "Rimserver.Help.TecSupport";
const char ABOUT_PROGRAM[] = "Rimserver.Help.About";

//右键菜单
const char MENU_ROUTE_TABLE_SERVER[] = "Menu.Route.Table.Server";             //路由表中服务器右键菜单

const char ACTION_ROUTE_SERVER_NEW[] = "Action_Route_Server_New";             //新建分组
const char ACTION_ROUTE_SERVER_DELETE[] = "Action_Route_Server_Delete";       //删除分组

const char MENU_ROUTE_TABLE_CLIENT[] = "Menu.Route.Table.Client";             //路由表中客户端右键菜单

const char ACTION_ROUTE_CLIENT_NEW[] = "Action_Route_Client_New";             //新建分组
const char ACTION_ROUTE_CLIENT_MOVE[] = "Action_Route_Client_Move";           //移动分组
const char ACTION_ROUTE_CLIENT_DELETE[] = "Action_Route_Clinet_Delete";       //删除分组

}

#endif // CONSTANTS_H
