#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reg_ui.h"
#include "music_ui.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usertable="mplay";
    //配置MySQL数据库
    initMySql();
    //新建一个自定义button类，方便制作出软键盘
    p = new KeyBoardShow(this);
    p->setGeometry(50,300,700,180);
    p->hide();
    //设置事件过滤器，只有点击到账户和密码输入框的时候才会调出软键盘
    ui->log_name->installEventFilter(this);
    ui->log_password->installEventFilter(this);

}
//点击账号或密码的时候，弹出软键盘
//      使用事件过滤器，重新定义点击事件
//1.登录按钮
//  1.1账号密码与数据库的账号密码不匹配
//        弹出警告账号或密码错误的弹窗，并要重新输入账号与密码来进行登录
//  1.2账号密码与数据库的账号密码相匹配。
//        登录成功就弹出登录成功的弹窗，然后跳转至主界面
//2.注册按钮
//        跳转至登录界面，并将登录界面的输入框内的内容清空
MainWindow::~MainWindow()
{
    delete model;//先移除模型在close数据库
    db.close();//移除该连接
    delete ui;
}

void MainWindow::on_reg_clicked()
{
    //弹出注册界面，并将登录界面的输入框内的内容清空
    reg_UI *p = new reg_UI(this);
    p->show();
    ui->log_name->setText("");
    ui->log_password->setText("");
    this->hide();
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //设置过滤器
    if ( (watched == ui->log_name) && (event->type() == QEvent::MouseButtonPress) )
    {
        //当点击账户输入框，软键盘出现
        p->show();
        ui->log_name->setFocus();
    }
    else if ( (watched == ui->log_password) && (event->type() == QEvent::MouseButtonPress) )
    {
        //当点击密码输入框，软键盘出现
        p->show();
        ui->log_password->setFocus();
    }
    return QMainWindow::eventFilter(watched,event);
}
//初始化数据库
void MainWindow::initMySql()
{
    static bool isFirst = true;
    if(isFirst) {
        db = QSqlDatabase::addDatabase("QMYSQL","first");	//指定了连接名
        isFirst = false;
    }else if(QSqlDatabase::contains("first")) {
        db = QSqlDatabase::database("first");
    }
    //db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("db");
    db.setUserName("root");
    db.setPassword("123456");
    db.setPort(3306);
    //判断是否打开成功
    if(!db.open())
    {
        qDebug() << "open failed";
        return ;
    }
    //创建数据库模型--绑定db数据库
    model = new QSqlTableModel(this,db);
    QString tablename = usertable;
    QString sql = "create table if not exists "+ tablename + "(name varchar(24) unique,password varchar(24));";
    //qDebug()<<sql;
    QSqlQuery query=QSqlQuery(db);
    query.exec(sql);
}
//设置搜索功能，只要name和password相匹配才能登录成功，否则出现警告弹窗
bool MainWindow::search(QString name, QString password)
{
    //SELECT NAME,PASSWORD FROM mplay WHERE NAME='zxz'&&PASSWORD=123;
    QString sql = QString("SELECT name,password FROM %1 WHERE name='%2'&&password=%3").arg(usertable).arg(name).arg(password);
    //qDebug()<<sql;
    QSqlQuery query=QSqlQuery(db);
    query.exec(sql);
    QString tmp;
    while (query.next()) {
         tmp = query.value(0).toString();
    }
    //查到返回true，如果是NULL说明没有查到数据
    if(tmp == NULL)
    {
        QMessageBox::warning(this,"警告信息","用户名或密码错误");
        return false;
    }
    return true;
}
void MainWindow::on_log_log_clicked()
{
    QString name = ui->log_name->text();
    QString mima = ui->log_password->text();
    bool b = search(name,mima);
    if(b)
    {
        QMessageBox::information(this,"信息","登录成功");
        //创建新界面
        Music_UI *p = new Music_UI(name,this);
        p->show();

        //隐藏当前界面
        this->hide();
    }
}


