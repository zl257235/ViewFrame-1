#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QFile>

#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/action.h"
#include "Base/pluginmanager/pluginmanager.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "Base/common/stylemanager.h"
#include "Base/common/languagemanager.h"
#include "Base/pluginmanager/subject.h"
#include "Base/util/rlog.h"

#include "widgets/taskcontrol/taskcontrolpanel.h"

using namespace Base;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("ViewFrame"));
    showMaximized();

    //TODO 20180915 待添加对配置路径下文件搜索
    RSingleton<StyleManager>::instance()->addStyle(CustomStyle(tr("Dark style"),":/resource/style/Black.qss",true));
    RSingleton<StyleManager>::instance()->addStyle(CustomStyle(tr("Light style"),":/resource/style/White.qss",false));

    RSingleton<Subject>::instance()->attach(this);

    initMenu();
    initComponent();

    updateLanguage(curLanguageName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMessage(MessageType::MessageType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void MainWindow::initMenu()
{
    ActionContainer *menubar = ActionManager::instance()->createMenuBar(Constant::MENU_BAR);

    setMenuBar(menubar->menuBar());
    menubar->appendGroup(Constant::G_PROGRAM);
    menubar->appendGroup(Constant::G_SETTING);
    menubar->appendGroup(Constant::G_HELP);

    //程序菜单
    serverMenu = ActionManager::instance()->createMenu(Constant::G_PROGRAM);
    serverMenu->appendGroup(Constant::G_PROGRAM);
    menubar->addMenu(serverMenu, Constant::G_PROGRAM);

    exitAction = new QAction(this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(programExit()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::PROGRAM_EXIT,exitAction);

    serverMenu->addAction(serverDelete,Constant::G_PROGRAM);

    //设置菜单
    settingsMenu = ActionManager::instance()->createMenu(Constant::G_SETTING);
    settingsMenu->appendGroup(Constant::G_SETTING);
    menubar->addMenu(settingsMenu, Constant::G_SETTING);

    topHintAction = new QAction(this);
    topHintAction->setCheckable(true);
    connect(topHintAction,SIGNAL(triggered(bool)),this,SLOT(windowTopHint(bool)));
    Action * topHint = ActionManager::instance()->registAction(Constant::TOP_HINT,topHintAction);
    settingsMenu->addAction(topHint,Constant::G_SETTING);

    fullScreenAction = new QAction(this);
    fullScreenAction->setCheckable(true);
    connect(fullScreenAction,SIGNAL(triggered(bool)),this,SLOT(windowFullScreen(bool)));
    Action * fullScreen = ActionManager::instance()->registAction(Constant::FULL_SCREEN,fullScreenAction);
    settingsMenu->addAction(fullScreen,Constant::G_SETTING);

    settingsMenu->addSeparator(Constant::G_SETTING);

    //样式
    settingsMenu->appendGroup(Constant::CUSTOM_STYLE);
    styleMenu = ActionManager::instance()->createMenu(Constant::CUSTOM_STYLE);
    styleMenu->appendGroup(Constant::CUSTOM_STYLE);
    settingsMenu->addMenu(styleMenu,Constant::CUSTOM_STYLE);

    StyleList styles = RSingleton<StyleManager>::instance()->styles();
    for(int i = 0;i<styles.size();i++){
        QAction * action = new QAction(styles.at(i).getStyleName());
        styleActionList.append(action);
        action->setCheckable(true);
        if(styles.at(i).isSelected()){
            action->setChecked(styles.at(i).isSelected());
            updateStyle(i);
        }
        connect(action,SIGNAL(triggered()),this,SLOT(switchStyle()));
        action->setProperty(Constant::CUSTOM_STYLE,i);
        Action * tmpStyle = ActionManager::instance()->registAction(QString("StyleAction%1").arg(i).toLocal8Bit().data(),action);
        styleMenu->addAction(tmpStyle,Constant::CUSTOM_STYLE);
    }

    //翻译文件
    settingsMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
    lanMenu = ActionManager::instance()->createMenu(Constant::SYSTEM_LANGUAGE);
    lanMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
    settingsMenu->addMenu(lanMenu,Constant::SYSTEM_LANGUAGE);

    LanguageList lans = RSingleton<LanguageManager>::instance()->languages();
    for(int i = 0;i<lans.size();i++){
        QAction * action = new QAction(lans.at(i).getName());
        lanActionList.append(action);
        action->setCheckable(true);
        if(lans.at(i).isSelected()){
            action->setChecked(lans.at(i).isSelected());
            curLanguageName = lans.at(i).getFileName();
        }
        connect(action,SIGNAL(triggered()),this,SLOT(switchLanguage()));
        action->setProperty(Constant::SYSTEM_LANGUAGE,lans.at(i).getFileName());
        Action * tmpStyle = ActionManager::instance()->registAction(QString("LanAction%1").arg(i).toLocal8Bit().data(),action);
        lanMenu->addAction(tmpStyle,Constant::SYSTEM_LANGUAGE);
    }

    //帮助菜单
    helpMenu = ActionManager::instance()->createMenu(Constant::G_HELP);
    helpMenu->appendGroup(Constant::G_HELP);
    menubar->addMenu(helpMenu, Constant::G_HELP);

    supportAction = new QAction(this);
    connect(supportAction,SIGNAL(triggered()),this,SLOT(technicalSupport()));
    Action * support = ActionManager::instance()->registAction(Constant::TEC_SUPPORT,supportAction);
    helpMenu->addAction(support,Constant::G_HELP);

    helpMenu->addSeparator(Constant::G_HELP);

    aboutPorgramAction = new QAction(this);
    connect(aboutPorgramAction,SIGNAL(triggered()),this,SLOT(aboutProgram()));
    Action * aboutProgram = ActionManager::instance()->registAction(Constant::ABOUT_PROGRAM,aboutPorgramAction);
    helpMenu->addAction(aboutProgram,Constant::G_HELP);
}

/*!
 * @brief   程序退出
 */
void MainWindow::programExit()
{
    exit(0);
}

/*!
 * @brief   程序置顶
 */
void MainWindow::windowTopHint(bool flag)
{
    Qt::WindowFlags flags = windowFlags();
    if(flag)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags = flags & ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    show();
}

void MainWindow::windowFullScreen(bool)
{
    Qt::WindowStates state = windowState();
    if(!isFullScreen())
        state |= Qt::WindowFullScreen;
    else
        state = state & ~Qt::WindowFullScreen;

    setWindowState(state);
}

/*!
 * @brief 切换显示样式
 */
void MainWindow::switchStyle()
{
    int switchIndex = QObject::sender()->property(Constant::CUSTOM_STYLE).toInt();
    StyleList list = RSingleton<StyleManager>::instance()->styles();
    if(switchIndex >= 0 && switchIndex < list.size()){
        updateStyle(switchIndex);
    }
}

/*!
 *  @brief 切换显示语言
 */
void MainWindow::switchLanguage()
{
    QString switchLan = QObject::sender()->property(Constant::SYSTEM_LANGUAGE).toString();
    updateLanguage(switchLan);
}

void MainWindow::updateStyle(int index)
{
    RSingleton<StyleManager>::instance()->switchStyle(index);
    for(int i = 0;i < styleActionList.size(); i++){
        if( index != i){
            styleActionList.at(i)->setChecked(false);
        }
    }
}

/*!
 * @brief 根据翻译文件名切换文件
 * @param[in]  lanFileName 要切换的文件名
 */
void MainWindow::updateLanguage(QString lanFileName)
{
    if(RSingleton<LanguageManager>::instance()->switchLanguage(lanFileName)){
        std::for_each(lanActionList.begin(),lanActionList.end(),[&](QAction * action){
            if(action->property(Constant::SYSTEM_LANGUAGE).toString() != lanFileName){
                action->setChecked(false);
            }
        });
        RSingleton<Subject>::instance()->notify(MessageType::MESS_LAN_CHANGED);
    }
}

void MainWindow::retranslateUi()
{
    serverMenu->menu()->setTitle(tr("&Program"));
    exitAction->setText(tr("&Exit"));

    settingsMenu->menu()->setTitle(tr("&Settings"));
    topHintAction->setText(tr("Top hint"));
    fullScreenAction->setText(tr("Full screen"));

    styleMenu->menu()->setTitle(tr("Styles"));
    lanMenu->menu()->setTitle(tr("Language"));
    helpMenu->menu()->setTitle(tr("&Help"));
    supportAction->setText(tr("Technical support"));
    aboutPorgramAction->setText(tr("About program"));
}

void MainWindow::technicalSupport()
{

}

void MainWindow::aboutProgram()
{

}

/*!
 * @brief   初始化各个组件
 */
void MainWindow::initComponent()
{
    setDockNestingEnabled(true);
    QWidget * centeral = takeCentralWidget();
    if (centeral)
        delete centeral;

    //TODO 20180824 待从各个dll中读取
    TaskControlModel::TaskControlPanel * taskControl = new TaskControlModel::TaskControlPanel;

    addDockWidget(Qt::LeftDockWidgetArea,taskControl);

//    addDockWidget(Qt::RightDockWidgetArea,connectionPanel);

    RSingleton<PluginManager>::instance()->addPlugin(taskControl);

    RSingleton<PluginManager>::instance()->load();
    PluginManager::ComponentMap maps = RSingleton<PluginManager>::instance()->plugins();

    PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        RComponent * comp = iter.value();
        comp->setFeatures(QDockWidget::DockWidgetMovable);
        comp->initialize();

        iter++;
    }
}
