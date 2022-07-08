#ifndef MYLABLE_H
#define MYLABLE_H

#include <QLabel>

#include<QDebug>
#include<QtWidgets/qframe.h>
#include<QWidget>
#include<QTimer>
#include<sys/time.h>

/*************************************************
类名称:MyLable
类功能、性能等的描述:
    自定义QLabel的控件，实现歌曲名滚动显示
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 无
*************************************************/
class MyLable : public QLabel
{
    Q_OBJECT
public:
    explicit MyLable(QWidget *parent = nullptr);
public:
    void setInterVal(int nInterVal){startTimer(nInterVal);}
    void SetTextContent(QString text);
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    int m_nCharWidth = 0;
    int m_nCurrentIndex = 0;        //文本起始x点
    int m_nTextWidth = 0;
    QString m_strText;
    QString m_strDrawText;

};

#endif // MYLABLE_H
