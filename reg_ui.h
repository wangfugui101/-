#ifndef REG_UI_H
#define REG_UI_H

#include "keyboardshow.h"

#include <QMainWindow>
#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
namespace Ui {
class reg_UI;
}
/*************************************************
类名称:reg_UI
类功能、性能等的描述:
    注册的显示界面
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 无
*************************************************/
class reg_UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit reg_UI(QWidget *parent = nullptr);
    ~reg_UI();
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
    输出参数的说明:
        true：   账户与密码匹配
        false：  账户与密码不匹配
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    bool search(QString name);
private slots:
    /*************************************************
    方法名称:on_reg_cancel_clicked
    功能、性能等的描述:
        信号槽函数，取消按钮，点击后跳转至登录界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_reg_cancel_clicked();
    /*************************************************
    方法名称:on_reg_reg_clicked
    功能、性能等的描述:
        信号槽函数，点击后注册成功，会自动跳转至登录界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_reg_reg_clicked();

private:
    Ui::reg_UI *ui;
    KeyBoardShow *p;
    QSqlDatabase db;//MySQL数据库连接
    QSqlTableModel *model;//数据库模型
    QString usertable;//设置数据库表名
};

#endif // REG_UI_H
