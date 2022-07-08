#include "hpushbutton.h"
#include <QCoreApplication>
#include <QKeyEvent>
#include <QDebug>

HPushButton::HPushButton(QWidget *parent)
    :QPushButton(parent)
{
    //去掉当前按钮的焦点
    this->setFocusPolicy(Qt::NoFocus);
}

HPushButton::HPushButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{

}

void HPushButton::mousePressEvent(QMouseEvent *event)
{

}

void HPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //先获取按钮上的文本
    QString str = this->text();
    if(str != "清除")
    {
        //获取文本的第一个字母，转换char类型数据
        char ch = str.at(0).toLatin1();
        //qDebug()<<ch;
        //创建自定义事件---按键事件，不需要其他按键辅助，文本是str
        QKeyEvent *e = new QKeyEvent(QKeyEvent::KeyPress,ch,Qt::NoModifier,str);
        //向目标窗口发送事件---焦点所在的窗口
        QCoreApplication::postEvent(this->window()->focusWidget(),e,Qt::NormalEventPriority);
    }
    else if(str == "shift")
    {
        char ch = str.at(0).toLatin1();
        QKeyEvent *e = new QKeyEvent(QKeyEvent::KeyPress,ch,Qt::ShiftModifier,str.toLower());
        QCoreApplication::postEvent(this->window()->focusWidget(),e,Qt::NormalEventPriority);
    }
    else{
        QKeyEvent *e = new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier,str);
        //向目标窗口发送事件---焦点所在的窗口
        QCoreApplication::postEvent(this->window()->focusWidget(),e,Qt::NormalEventPriority);
    }
}
