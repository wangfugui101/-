#include "keyboardshow.h"
#include "ui_keyboardshow.h"

KeyBoardShow::KeyBoardShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyBoardShow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

KeyBoardShow::~KeyBoardShow()
{
    delete ui;
}

void KeyBoardShow::on_pushButton_clicked()
{
    this->hide();
}

