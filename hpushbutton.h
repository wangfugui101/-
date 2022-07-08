#ifndef HPUSHBUTTON_H
#define HPUSHBUTTON_H

#include <QPushButton>
/*************************************************
类名称:HPushButton
类功能、性能等的描述:
    自定义QPushButton的控件，实现软键盘的点击
输入参数说明：
    无
输出参数的说明:
    无
函数返回值的说明:
    无
其它说明: 无
*************************************************/
class HPushButton : public QPushButton
{
public:
    explicit HPushButton(QWidget *parent = nullptr);
    explicit HPushButton(const QString &text, QWidget *parent = nullptr);
/*************************************************
函数名称:mousePressEvent
函数功能、性能等的描述:
    鼠标按下事件
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 无
*************************************************/
    virtual void mousePressEvent(QMouseEvent *event);//鼠标按下
/*************************************************
函数名称:mouseReleaseEvent
函数功能、性能等的描述:
    重写鼠标左键释放事件
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 将自定义软键盘的每个按钮的值传递给界面
*************************************************/
    virtual void mouseReleaseEvent(QMouseEvent *event);//鼠标释放
};

#endif // HPUSHBUTTON_H
