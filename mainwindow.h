#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "keyboardshow.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include <QSqlQuery>
#include <QSqlTableModel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/*************************************************
类名称:MainWindow
类功能、性能等的描述:
    登录的显示界面
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 无
*************************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*************************************************
    方法名称:eventFilter
    功能、性能等的描述:
        重写事件过滤器
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 只要当点击账号或者密码框时才会触发事件，弹出软键盘
    *************************************************/
    bool eventFilter(QObject *watched, QEvent *event);
    /*************************************************
    方法名称:initMySql
    功能、性能等的描述:
        初始化MYSQL数据库
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 连接数据库
    *************************************************/
    void initMySql();
    /*************************************************
    方法名称:search
    功能、性能等的描述:
        查询账户密码是否匹配
    输入参数说明：
        name：       用户输入的账户
        password：   用户输入的密码
    输出参数的说明:
        true：   账户与密码匹配
        false：  账户与密码不匹配
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    bool search(QString name,QString password);
private slots:
    /*************************************************
    方法名称:on_reg_clicked
    功能、性能等的描述:
        信号槽函数，点击后跳转至注册界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_reg_clicked();
    /*************************************************
    方法名称:on_log_log_clicked
    功能、性能等的描述:
        信号槽函数，点击后跳转至音乐播放界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_log_log_clicked();

private:
    Ui::MainWindow *ui;
    KeyBoardShow *p;//软键盘
    QSqlDatabase db;//MySQL数据库连接
    QSqlTableModel *model;//数据库模型
    QString usertable;//设置数据库表名
};
#endif // MAINWINDOW_H
