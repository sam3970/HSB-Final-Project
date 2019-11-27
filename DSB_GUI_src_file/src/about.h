#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QIcon>
#include <QEvent>
namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

private slots:


    void on_btnClose_clicked();

private:
    Ui::About *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    bool pressed = false;
};

#endif // ABOUT_H
