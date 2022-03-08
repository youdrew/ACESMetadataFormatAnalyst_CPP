#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow       // 这个类的名字应该是会和主窗口名字一样
{
    Q_OBJECT
                                            // 往下会有一个SETUPUI的函数，算是界面初始化
public:                                     // 这里面应该会有一些*文件，创建一些子窗口
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
                                            //这个下面应该还会有一个叫Ui的名字空间
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
