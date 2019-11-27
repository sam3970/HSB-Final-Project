#include"mylabel.h"

#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
}

void MyLabel::mousePressEvent(QMouseEvent * event)
{
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
    //if (event->buttons() & Qt::LeftButton)
    //{
    //    QPoint diff = event->pos() - mpos;
    //    QPoint newpos = this->pos() + diff;
    //    this-> parentWidget()->move(newpos);
    //}
}
