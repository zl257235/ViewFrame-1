/*!
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

namespace Ui {
class MainWindow;
}

class BaseInfoDockPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initMenu();

private slots:
    void programExit();

    void windowTopHint(bool flag);
    void windowFullScreen(bool);

    void technicalSupport();
    void aboutProgram();

private:
    void initComponent();

private:
    Ui::MainWindow *ui;

    BaseInfoDockPanel * baseInfoPanel;
};

#endif // MAINWINDOW_H
