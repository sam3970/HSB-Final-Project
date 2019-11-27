#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QSysInfo>
#include <QTemporaryDir>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include <qpainter.h>
#include <QDialog>
#include <QStyle>
#include <QStyleOptionTitleBar>
#include "settings.h"
#include "about.h"
//#include "mylabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QStringList, QWidget *parent = 0);
    ~MainWindow();

    //setting//void ayarKayit();
protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent* event);
public slots:
    void procStart();
    void procStop();

    void processOutput();
    void processError();
    void handleState();
    //void paintEvent(QPaintEvent *);
    void onActionAyarlar();
    void onActionAbout();

    void onDefaultParamCheckState(Qt::CheckState state);

    QStringList prepareParameters(bool isComboParametreEnabled);

    void catchError(QProcess::ProcessError err);


signals:


private slots:
    void on_btnAyarlar_clicked();

    void on_btnAbout_clicked();



    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnClose_clicked();

    void on_btnMin_clicked();

private:
    Ui::MainWindow *ui;

    QSysInfo *info;
    QTemporaryDir *tmpDir;
    QSettings *settings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    QAction *hideAction;
    QAction *showAction;
    QAction *closeAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *settingsAction;

    QProcess *proc;

    QStringList args;

    Settings *ayarlar;
    About hakkinda;

    //bool eventFilter(QObject *object,QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    bool pressed = false;
    /*
    bool setabt = false;
    bool setset = false*/
//
    enum ParameterList{
        russia_blacklist,
        russia_blacklist_dnsredir,
        all,
        all_dnsredir,
        all_dnsredir_hardcore
    };
};

#endif // MAINWINDOW_H
