﻿/*!
 *  @brief     通用显示框架
 *  @details   以Dock方式运行子程序页面
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.22
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
class QAction;

#include "Base/pluginmanager/observer.h"
#include "Network/NetworkInterface.h"
#include "datadisplay/radiationsourcetable.h"
#include "datadisplay/allplusetable.h"
#include "datadisplay/mfacquistiontable.h"

namespace Ui {
class MainWindow;
}

class ActionContainer;
class BaseInfoDockPanel;

class MainWindow : public QMainWindow , public Base::Observer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onMessage(MessageType::MessType type);

private:
    void initMenu();

private slots:
    void programExit();

    void windowTopHint(bool flag);
    void windowFullScreen(bool);

    void switchStyle();
    void switchLanguage();

    void technicalSupport();
    void aboutProgram();

    void recvTranst(int iLen);
private:
    void initComponent();
    void updateStyle(int index);
    void updateLanguage(QString lanFileName);
    void retranslateUi();
    void initNetwork();
public:
    NetworkInterface* networkInterface;
private:
    Ui::MainWindow *ui;

    QList<QAction *> styleActionList;
    QList<QAction *> lanActionList;

    QString curLanguageName;

    ActionContainer * serverMenu;
    ActionContainer * settingsMenu;
    ActionContainer * styleMenu;
    ActionContainer * lanMenu;
    ActionContainer * helpMenu;
    QAction * exitAction;
    QAction * topHintAction;
    QAction * fullScreenAction;
    QAction * supportAction;
    QAction * aboutPorgramAction;

    DataView::RadiationSourceTable * radiationTable;
    DataView::AllPluseTable * allPluseTable;
    DataView::MFAcquistionTable * acquistionTable;

};

#endif // MAINWINDOW_H
