#include "novel_ui.h"
#include "ui_novel_ui.h"

Novel_UI::Novel_UI(QString path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Novel_UI)
{
    ui->setupUi(this);

   this->path = path;//将传递过来的连接保存在path里面
   showDirectory();
   //关联网络请求
   connect(&manager1,&QNetworkAccessManager::finished,this,&Novel_UI::readdata);
   connect(&manager2,&QNetworkAccessManager::finished,this,&Novel_UI::readnovel);
}

Novel_UI::~Novel_UI()
{
    delete ui;
}
void Novel_UI::showDirectory()
{
    qDebug("下面开始爬去小说目录...");
    QUrl url1(path);
    QNetworkRequest request1(url1);
    manager1.get(request1);
    //qDebug()<<path;
    //点击搜索按钮后，如果listwidget里面有数据，就将listwidget清空和存放搜索界面的链接的hreflist也清空。
    int num = ui->novel_director->count();
    if(num != 0)
    {
        for (int i =0;i<num;i++) {
            ui->novel_director->takeItem(0);
        }
        hreflist1.clear();
    }
}

void Novel_UI::readdata(QNetworkReply *reply)
{
//http://www.cqwsjds.com/bqg/*

    QString s = reply->readAll();
    //qDebug()<<s;
    QRegExp rx;

    //定义查询的正则表达式
    rx.setPattern("\\<ul class=\"section-list fix\">(.*)</ul>(.*)<ul class=\"section-list fix\">(.*)</ul>\\s");
    //判断正则表达式是否书写正确
    //qDebug() << rx.isValid();
    //设置为非贪心匹配
    rx.setMinimal(true);
    int pos = rx.indexIn(s);
    //qDebug() <<"pos="<<pos;
    if (pos > -1) {
        value1 = rx.cap(3);
        //合并所有空格变成一个空格
        value1=value1.simplified();

        value1.remove("<li class=\"book-item\">");
        value1.remove("</li>");
        //qDebug()<<value1;
        //qDebug()<<value1;
     }
    int a;

    //循环将每一个章节放进listwidget里面
    //getList();
    while (1) {
        a=getList();
        if(a==-1)
        {
            break;
        }
        getList();
    }
    //value1.remove("</a>");
    //获取最后一章
    //qDebug()<<value1;
    rx.setPattern("<a style=\"\" href=\"(.*).html\">(.*)</a>");
    //qDebug() << rx.isValid();
    pos = rx.indexIn(value1);
    //qDebug()<<pos;
    if(pos != -1)
    {
        QString tmp = rx.cap(1);
        QString name = rx.cap(2);
        //qDebug()<<tmp;
        //qDebug()<<name;
        value1.remove(tmp);
        value1.remove(name);
        //qDebug()<<value1;
        QString link= "http://www.cqwsjds.com"+tmp+".html";
        hreflist1.append(link);
        ui->novel_director->addItem(name);
    }

    qDebug()<<"目录爬取成功！";
}
//将爬取的内容存进listwidget里面
int Novel_UI::getList()
{
    QRegExp rx;
    //qDebug()<<value1;
    //定义查询的正则表达式
    //rx.setPattern("\\<li>(.*)</li>\\s");
    //<a style=\"\" href=\"/bqg/5002/4779167.html\">第八章 星空古路</a>
    rx.setPattern("\\<a style=\"\" href=\"(.*).html\">(.*)</a>\\s");
    //判断正则表达式是否书写正确
    //qDebug() << rx.isValid();
    //设置为非贪心匹配
    rx.setMinimal(true);
    int pos = rx.indexIn(value1);
    //qDebug() <<"pos="<<pos;
    //获取正则表达式所获取的内容并去除杂项
    if (pos != -1) {
        QString tmp = rx.cap(1);//"/bqg/5002/4796247"
        QString name = rx.cap(2);//第八章 星空古路
        value1=value1.simplified();
        value1.remove(tmp);

        value1.remove("<a style=\"\" href=\".html\">");
        //http://www.cqwsjds.com/bqg/5255/3217278.html
        QString link= "http://www.cqwsjds.com"+tmp+".html";
        //qDebug()<<tmp;
        //qDebug()<<name;
        hreflist1.append(link);
        ui->novel_director->addItem(name);
     }

    return pos;
}
void Novel_UI::on_back_clicked()
{
    this->parentWidget()->show();
    delete this;
}

void Novel_UI::on_novel_director_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug("下面开始爬取小说内容");
    int row = ui->novel_director->currentRow();
    QUrl url2(hreflist1.at(row));
    QNetworkRequest request2(url2);
    manager2.get(request2);
}

void Novel_UI::readnovel(QNetworkReply *reply)
{
    //http://www.cqwsjds.com/bqg/5731/3336382.html
    QString s = reply->readAll();
    //qDebug()<<s;
    QRegExp rx;

    rx.setPattern("\\举报后维护人员会在两分钟内校正章节内容,请耐心等待,并刷新页面。</div>(.*)<br />\\s");
    //qDebug() << rx.isValid();

    int pos = rx.indexIn(s);
    //qDebug() <<"pos="<<pos;
    if (pos > -1) {
         QString tmp = rx.cap(1);
         //qDebug() << tmp;
         tmp=tmp.simplified();
         ui->textEdit->setText(tmp);
     }
    qDebug()<<"内容爬取成功！";
}
