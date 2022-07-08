/************************************************************
Copyright (C), 2022-2031, 周湘祯. Co., Ltd.
文件名称: 多功能音乐播放与小说阅读器
文件说明：
        实现本地音乐与线上小说阅读
作者: 周湘祯  版本 : v1.0 创建日期: 2022年07月4日
开发系统版本(lsb_release -a)：  Ubuntu 18.04 LTS
GCC编译器版本(gcc  -v)： gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
Qt Creator 4.11.1：  Based on Qt 5.14.1 (MSVC 2017, 32 bit)
函数列表:
1. main：启动代码的入口函数
历史记录:
<作者>  <创建时间>       <版本号>
周湘祯   2022年07月4日    v1.0
***********************************************************/
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
