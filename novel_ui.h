#ifndef NOVEL_UI_H
#define NOVEL_UI_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>
#include <QRegExp>
#include <QListWidget>
namespace Ui {
class Novel_UI;
}
/*************************************************
类名称:Novel_UI
类功能、性能等的描述:
    小说章节和小说内容的显示界面
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 暂时未完成对小说内容的文本设置功能
*************************************************/
class Novel_UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit Novel_UI(QString path, QWidget *parent = nullptr);
    ~Novel_UI();
    /*************************************************
    方法名称:showDirectory
    功能、性能等的描述:
        将爬取的小说章节存进listwidget里面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 小说章节存入QlistWidget
    *************************************************/
    void showDirectory();
    /*************************************************
    方法名称:getList
    功能、性能等的描述:
        将爬取的内容存进listwidget里面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    int getList();

private slots:
    /*************************************************
    方法名称:readdata
    功能、性能等的描述:
        信号槽函数，根据Url发送get请求获取网页源码
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 使用正则表达式，并设置为非贪心匹配，处理文本，再显示在listWidget。
    *************************************************/
    void readdata(QNetworkReply *reply);
    /*************************************************
    方法名称:on_back_clicked
    功能、性能等的描述:
        信号槽函数，跳转至音乐播放界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_back_clicked();
    /*************************************************
    方法名称:on_novel_director_itemDoubleClicked
    功能、性能等的描述:
        信号槽函数，双击获取小说内容并显示在右边的文本框中
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_novel_director_itemDoubleClicked(QListWidgetItem *item);
    /*************************************************
    方法名称:readnovel
    功能、性能等的描述:
        信号槽函数，根据Url发送get请求获取网页源码
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 使用正则表达式，并设置为非贪心匹配，处理文本，再显示在文本框中
    *************************************************/
    void readnovel(QNetworkReply *reply);


private:
    Ui::Novel_UI *ui;
    QString path;//保存传过来的链接
    QNetworkAccessManager manager1;//用于爬取搜索界面的接口
    QString value1;//存放所爬取的网页的目标源代码
    QStringList hreflist1;//存储搜索的连接
    QNetworkAccessManager manager2;//用于爬取小说界面的接口
};

#endif // NOVEL_UI_H
