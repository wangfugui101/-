#include "reg_ui.h"
#include "ui_reg_ui.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlDriver>
#include <QSqlRecord>

reg_UI::reg_UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::reg_UI)
{
    ui->setupUi(this);

    usertable="mplay";
    //配置MySQL数据库
    initMySql();

    p = new KeyBoardShow(this);
    p->setGeometry(50,300,700,180);
    p->hide();

    ui->reg_name->installEventFilter(this);
    ui->reg_password->installEventFilter(this);
}
//1.注册按钮
//  1.1注册的账号不能相同
//        相同弹出警告账号名相同的弹窗，并要重新输入账号与密码来进行注册
//  1.2注册的账号如果未注册，
//        注册成功就弹出注册成功的弹窗，然后返回至登录界面
//2.取消按钮
//        返回至登录界面
reg_UI::~reg_UI()
{
    delete ui;
}

void reg_UI::on_reg_cancel_clicked()
{
    this->parentWidget()->show();
    this->deleteLater();
}
void reg_UI::on_reg_reg_clicked()
{
    QString name = ui->reg_name->text();
    QString mima = ui->reg_password->text();
    bool b = search(name);
    if(b)
    {
        QString sql = QString("INSERT INTO %3(name,password) VALUES ('%1',%2);").arg(name).arg(mima).arg(usertable);
        //qDebug()<<sql;
        QSqlQuery query=QSqlQuery(db);
        query.exec(sql);
        QMessageBox::information(this,"信息","注册成功");
        //显示上一级界面
        this->parentWidget()->show();

        //销毁当前界面
        this->deleteLater();
    }
}
void reg_UI::initMySql()
{
    static bool isFirstForPie = true;
    if(isFirstForPie) {
        db = QSqlDatabase::addDatabase("QMYSQL", "DBForPieSeries");//指定了连接名
        isFirstForPie = false;
    } else if(QSqlDatabase::contains("DBForPieSeries")) {
        db = QSqlDatabase::database("DBForPieSeries");
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
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setTable(usertable);//！！需要设置model的表名
    QString tablename = usertable;

    QString sql = "create table if not exists "+ tablename + "(name varchar(24) unique,password varchar(24));";
    //qDebug()<<sql;
    QSqlQuery query=QSqlQuery(db);
    query.exec(sql);
}

bool reg_UI::search(QString name)
{
    QString sql = QString("SELECT name FROM %1 WHERE name='%2'").arg(usertable).arg(name);
    //qDebug()<<sql;
    QSqlQuery query=QSqlQuery(db);
    query.exec(sql);
    QString tmp;
    while (query.next()) {
         tmp = query.value(0).toString();
    }
    if(tmp == name)
    {
        QMessageBox::warning(this,"警告信息","用户名已注册");
        return false;
    }
    return true;
}

bool reg_UI::eventFilter(QObject *watched, QEvent *event)
{
    if ( (watched == ui->reg_name) && (event->type() == QEvent::MouseButtonPress) )
    {
        p->show();
        ui->reg_name->setFocus();
    }
    else if ( (watched == ui->reg_password) && (event->type() == QEvent::MouseButtonPress) )
    {
        p->show();
        ui->reg_password->setFocus();
    }

    return QMainWindow::eventFilter(watched,event);
}
