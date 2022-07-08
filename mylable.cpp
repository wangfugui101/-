#include "mylable.h"
#include <QPainter>
#include <QDebug>

MyLable::MyLable(QWidget *parent) : QLabel(parent)
{
    m_strDrawText="";
    this->startTimer(100);

}

void MyLable::SetTextContent(QString text)
{
    m_strDrawText = text;
    update();
}
void MyLable::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    //qDebug()<<text();
    QRectF rectText = rect();
    rectText.setWidth(width() + m_nTextWidth);
    rectText.setX(-m_nCurrentIndex);
//    painter.drawText(rectText,alignment(),m_strDrawText);
    painter.drawText(rectText,alignment(),text());
}

void MyLable::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    if(text().isEmpty())
        return ;

    if(m_strText != text()){
        m_strText = text();
        m_nCurrentIndex = 0;
        m_nTextWidth = fontMetrics().horizontalAdvance(m_strText);
    }

    while(fontMetrics().horizontalAdvance(m_strDrawText) < width() + m_nTextWidth){
        m_strDrawText += text();
    }

    m_nCurrentIndex++;
    if(m_nCurrentIndex == m_nTextWidth){
        m_nCurrentIndex = 0;
    }

    update();
}

