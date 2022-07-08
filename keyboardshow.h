#ifndef KEYBOARDSHOW_H
#define KEYBOARDSHOW_H

#include <QMainWindow>
/*************************************************
类名称:KeyBoardShow
类功能、性能等的描述:
    软键盘的显示界面
输入参数说明：无
输出参数的说明:无
函数返回值的说明:无
其它说明: 无
*************************************************/

namespace Ui {
class KeyBoardShow;

}

class KeyBoardShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KeyBoardShow(QWidget *parent = nullptr);
    ~KeyBoardShow();

private slots:
    /*************************************************
    方法名称:on_pushButton_clicked
    功能、性能等的描述:
        信号槽函数，点击后隐藏该界面
    输入参数说明：无
    输出参数的说明:无
    函数返回值的说明:无
    其它说明: 无
    *************************************************/
    void on_pushButton_clicked();

private:
    Ui::KeyBoardShow *ui;

};

#endif // KEYBOARDSHOW_H
