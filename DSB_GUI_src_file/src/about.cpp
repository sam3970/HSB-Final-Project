#include "about.h"
#include "ui_about.h"
#include <QLabel>
#include <QMouseEvent>
About::About(QWidget *parent) :
    QDialog(parent,Qt::FramelessWindowHint),
    ui(new Ui::About)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    //setWindowTitle("정보");
    //setWindowIcon(QIcon(":/images/images/info-button.png"));
    QPixmap pix1(":/images/images/info-button.png");
    ui->label_11->setPixmap(pix1);
    ui->label_11->setScaledContents(true);
    ui->label_11->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label_10->setStyleSheet("QLabel {color : white; }");

    QPixmap pix(":/images/images/network-3537400_1920.png");
    ui->iconLabel->setPixmap(pix);
    ui->iconLabel->setScaledContents(true);
    ui->iconLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->aboutLabel->setTextFormat(Qt::RichText);
    ui->aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->aboutLabel->setOpenExternalLinks(true);

}
void About::mousePressEvent(QMouseEvent *event) {
//title bar move
    if(((ui->label_2->pos().x()   <= event->x())&&(event->pos().x() <= ui->label_2->geometry().width()))
        &&((ui->label_2->pos().y()<= event->y())&&(event->pos().y() <= ui->label_2->geometry().height()))
        &&(event->type()==QMouseEvent::MouseButtonPress))
    {
        pressed = true;
    }
    else
        pressed = false;
/*    if(((ui->label_2->pos().x()   <= event->x())&&(event->pos().x() <= ui->label_2->geometry().width()))
        &&((ui->label_2->pos().y()<= event->y())&&(event->pos().y() <= ui->label_2->geometry().height()))
        &&(event->type()== QMouseEvent::MouseButtonRelease))
    {

    }
*/
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void About::mouseMoveEvent(QMouseEvent *event) {
    if(pressed == true){
        move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}
About::~About()
{
    delete ui;
}

void About::on_btnClose_clicked()
{
    this->close();
}
