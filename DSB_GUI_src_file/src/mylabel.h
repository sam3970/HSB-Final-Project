#ifndef MYLABEL_H
#define MYLABEL_H

#include<QEvent>
#include<QObject>
#include<QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    QPoint mpos;

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};


#endif // MYLABEL_H
