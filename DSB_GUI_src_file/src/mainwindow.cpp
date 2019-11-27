#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDirIterator>
#include <QListIterator>
#include <QMovie>
#include <memory>
#include <Windows.h>
#include <QLabel>

MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow),
    tmpDir(new QTemporaryDir()),
    settings(new QSettings("HexOpenSource", "DSB_GUI", this)),
    trayIcon(new QSystemTrayIcon(this)),
    trayMenu(new QMenu(this)),
    hideAction(new QAction(tr("Gizle"), this)),
    showAction(new QAction(tr("Göster"), this)),
    closeAction(new QAction(tr("Çıkış"), this)),
    startAction(new QAction(QIcon(":/images/images/play-button.png"), tr("Başlat"), this)),
    stopAction(new QAction(QIcon(":/images/images/stop-button.png"), tr("Durdur"), this)),
    settingsAction(new QAction(QIcon(":/images/images/settings-gears-button.png"), tr("Ayarlar"), this)),
    proc(new QProcess(this)),
    ayarlar(new Settings())
{
    ui->setupUi(this);


    QPixmap pix(":/images/images/info-button.png");
    ui->label_5->setPixmap(pix);
    ui->label_5->setScaledContents(true);
    ui->label_5->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    QPixmap pix2(":/images/images/settings-gears-button.png");
    ui->label_6->setPixmap(pix2);
    ui->label_6->setScaledContents(true);
    ui->label_6->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    QPixmap pix3(":/images/images/padlock.png");
    ui->label_9->setPixmap(pix3);
    ui->label_9->setScaledContents(true);
    ui->label_9->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    QPixmap pix4(":/images/images/padlock-unlock.png");
    ui->label_8->setPixmap(pix4);
    ui->label_8->setScaledContents(true);
    ui->label_8->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    /*QPixmap pix5(":/images/images/close_ver2.png");
    ui->label_10->setPixmap(pix5);
    ui->label_10->setScaledContents(true);
    ui->label_10->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );*/

    //QIcon closeIcon(":/images/images/close.png");
    //ui->btnClose->setFocusProxy(this);
    //ui->btnClose = new QPushButton(closeIcon, tr(""), this);

    ui->label_10->setStyleSheet("QLabel {color : white; }");
    ui->label_11->setStyleSheet("QLabel {color : white; }");

    /*QMovie *Movie=new QMovie(":/images/images/test2.gif");
    ui->label->setMovie(Movie);
    Movie->start();*/
    //QPixmap pix(":/images/images/icon.png");
    //ui->btnAyarlar->setPaletteBackgroundPixmap(Pix);
    setWindowIcon(QIcon(":/images/images/icon.ico"));
    setWindowTitle("DSB_GUI");
    //ui->btnAyarlar->setStyleSheet(QStringLiteral("QPushButton {background-image: url(:/images/images/test.png)};"));
/*투명하게*/
    ui->btnAyarlar->setWindowIcon(QIcon(":/images/images/settings-gears-button.png"));

    //setAutoFillBackground(false);
    //setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    //ui->setupUi(this);       툴바 본체

    trayIcon->setIcon(QIcon(":/images/images/icon.ico"));
    trayIcon->setToolTip("DSB_GUI by Dankook Univ DSB Team");

    //ui->labelParameters->setWordWrap(true); //수정

    //connect(ui->label_2,SIGNAL(MousePress()))
    //mouse label move

    //For using lambda functions with traymenu
    auto& traymn = trayMenu;

    //Setting traymenu actions.
    connect(startAction, &QAction::triggered, this, &MainWindow::procStart);
    connect(stopAction, &QAction::triggered, this, &MainWindow::procStop);
    connect(closeAction, &QAction::triggered, [this](){
        QCoreApplication::quit();
    });
    connect(hideAction, &QAction::triggered, [this, traymn](){
        this->hide();
        traymn->actions().at(5)->setEnabled(false);
        traymn->actions().at(4)->setEnabled(true);
    });
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(showAction, &QAction::triggered, [this, traymn](){
        this->show();
        traymn->actions().at(5)->setEnabled(true);
        traymn->actions().at(4)->setEnabled(false);
    });

    QList<QAction*> actionList;
    actionList << startAction << stopAction << settingsAction << showAction << hideAction << closeAction;

    trayMenu->addActions(actionList);
    trayMenu->insertSeparator(showAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    //Set false Stop and Hide actions
    trayMenu->actions().at(4)->setEnabled(false);
    trayMenu->actions().at(1)->setEnabled(false);

    connect(ui->actionAyarlar, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);

    //Checking if default parameters enabled or not due to enable/disable parameters combo box.
    if(!settings->value("Parametre/defaultParam").toBool())
    {
        ui->comboParametre->setEnabled(false);
    }

    //Capturing state of default parameters checkbox for enable/disable parameters combo box.
    connect(ayarlar, &Settings::defaultParamStateChanged, this, &MainWindow::onDefaultParamCheckState, Qt::QueuedConnection);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::procStart);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::procStop);
    //connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::procStart);

    connect(proc, &QProcess::stateChanged, this, &MainWindow::handleState);
//선택 리스트
    ui->comboParametre->addItem("(Full Features Activate)모든 기능 활성화", QVariant("-1"));

    ui->comboParametre->setCurrentIndex(0);

    ui->btnStop->setEnabled(false);

    connect(ui->comboParametre, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]()
    {
        prepareParameters(true);
    });

    //Capturing ouput of DSB.exe
    connect(proc, &QProcess::readyReadStandardOutput, this, &MainWindow::processOutput);
    connect(proc, &QProcess::readyReadStandardError, this, &MainWindow::processError);

    if(settings->value("Parametre/defaultParam").toBool())
        prepareParameters(true);
    else
        prepareParameters(false);

    if(!this->isVisible())
    {
        hideAction->setEnabled(false);
        showAction->setEnabled(true);
    }

    connect(proc, &QProcess::errorOccurred, this, &MainWindow::catchError);
    //connect(ui->label_2,SIGNAL(clicked()),this,SLOT(&mylabel::mousePressEvent(QMouseEvent *event)));
    //connect(ui->label_2,SIGNAL(clicked()),this,SLOT(&MainWindow::mouseMoveEvent(QMouseEvent *event)));
    //connect()
}
/*
bool MainWindow::eventFilter(QObject *object, QMouseEvent *event)
{
    if (object == ui->label_2 && event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;
        return true;
    }
    if (object == ui->label_2 && event->type() == QEvent::MouseButtonRelease)
    {
        pressed = false;
        return true;
    }
    else
        return false;
}*/
/*bool MainWindow::eventFilter(QObject *object,QMouseEvent *event)
{
    ui->label_10->setText(QString("%1").arg(event->x()));

    return true;
    //마우스의 위치값만큼 더해준다.
    *//*
    if(((ui->label_2->pos().x() <= event->pos().x())&&(event->pos().x() <= (ui->label_2->pos().x() + ui->label_2->geometry().width())))
        &&((ui->label_2->pos().y()<=event->pos().y())&&(event->pos().y()<= (ui->label_2->pos().y() + ui->label_2->geometry().height())))
        &&(event->type()==QMouseEvent::MouseButtonPress))
    {
        pressed = true;
        return true;
    }
    if(((ui->label_2->pos().x() <= event->pos().x())&&(event->pos().x() <= (ui->label_2->pos().x() + ui->label_2->geometry().width())))
         &&((ui->label_2->pos().y()<=event->pos().y())&&(event->pos().y()<= (ui->label_2->pos().y() + ui->label_2->geometry().height())))
         &&(event->type()== QMouseEvent::MouseButtonRelease))
    {
        pressed = false;
        return true;
    }
    else
        return false;
*/
//}

void MainWindow::mousePressEvent(QMouseEvent *event) {
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

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(pressed == true){
        move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}


void MainWindow::paintEvent(QPaintEvent *event)
{// 투명도
    /*
    Q_UNUSED(event)
     QPainter p(this);
     QStyle* style = this->style();
     QRect active_area = this->rect();
     int titlebar_height = 0; // Titlebar.
     QStyleOptionTitleBar t_opt;
     t_opt.initFrom(this);
     titlebar_height = style->pixelMetric(QStyle::PM_TitleBarHeight, &t_opt, this);
     t_opt.rect = QRect(0, 0, this->width(), titlebar_height);

     //t_opt.titleBarState = this->windowState();
     //t_opt.text = t_opt.fontMetrics.elidedText(this->windowTitle(), Qt::ElideRight, t_opt.rect.width());
     style->drawComplexControl(QStyle::CC_TitleBar, &t_opt, &p, this);
     //style->drawItemText(&p, t_opt.rect, Qt::AlignCenter, t_opt.palette, true, t_opt.text, QPalette::ToolTipText); // Background widget.

     active_area.setTopLeft(QPoint(0, titlebar_height));
     this->setContentsMargins(0, titlebar_height, 0, 0);
     QStyleOption w_opt;
     w_opt.initFrom(this);
     w_opt.rect = active_area;
     style->drawPrimitive(QStyle::PE_Widget, &w_opt, &p, this);*/
}
MainWindow::~MainWindow()
{
    delete ui;
    proc->kill();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug() << settings->value("System/systemTray").toString();
    if(settings->value("System/systemTray").toString() == "true" && (this->isTopLevel() || this->isVisible()))
    {
        event->ignore();
        this->hide();
        trayMenu->actions().at(4)->setEnabled(true);
        trayMenu->actions().at(5)->setEnabled(false);

        if(!settings->value("System/disableNotifications").toBool())
        {
            qDebug() << "Message will shown";
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage("DSB GUI", tr("Arka planda çalışıyor."), icon, 1000);
        }
    }
    else
    {
        ayarlar->close();
    }
}

void MainWindow::procStart()
{
    //proc->setArguments(prepareParameters(ui->comboParametre->isEnabled()));
    //ui->debugArea->appendPlainText("\"" + QDir::currentPath() + QString("/DSB/DSB.exe\"") + " " +prepareParameters(ui->comboParametre->isEnabled()).join(" "));//수정
    //It's only way GoodbyeDPI works, because no matter what I try, It crashes with different arguments except "-1" If I use argument list method like start(program, arglist, mode)
    //I have to add manual "(quotes) for PATHs that contains space, because It start function tries to execute it like command prompt and you can't use space char at command prompt.

//경로 지정
    if(info->productVersion() != "7")
    {
        proc->start("\"" + QDir::currentPath() + QString("/DSB/DSB.exe\"") + " " +prepareParameters(ui->comboParametre->isEnabled()).join(" "), QProcess::ReadOnly);

    }
    else {
        proc->setArguments(prepareParameters(ui->comboParametre->isEnabled()));
        proc->setProgram(QDir::currentPath() + QString("/DSB/DSB.exe"));
        proc->start(QProcess::ReadOnly);
    }

    proc->waitForStarted(1000);

    if(!settings->value("System/disableNotifications").toBool() && !this->isVisible())
    {
        qDebug() << "Message will shown";
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("DSB_GUI", tr("Başlatıldı."), icon, 1000);
    }
}
//종료
void MainWindow::procStop()
{
    proc->close();
    proc->waitForFinished(2000);
    QProcess::execute(":/service_remove.cmd");
}

void MainWindow::processOutput()
{
    proc->setReadChannel(QProcess::StandardOutput);
    QString output = proc->readAllStandardOutput();

    if(!output.isEmpty())
    {
        QString prettyOutput = QString::fromStdString(output.toStdString());
        prettyOutput.replace(",", "\n");
        //수정ui->debugArea->appendPlainText(prettyOutput);
    }
}

void MainWindow::processError()
{
    proc->setReadChannel(QProcess::StandardError);
    QString errout = proc->readAllStandardError();
    if(!errout.isEmpty())
    {
        //수정ui->debugArea->appendPlainText(proc->errorString());
    }
}
void MainWindow::handleState()
{
    if(proc->state() == QProcess::NotRunning)
    {
        //수정ui->debugArea->appendPlainText(tr("[-] Durduruldu"));
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(false);
        trayMenu->actions().at(0)->setEnabled(true);
    }
    else if(proc->state() == QProcess::Running)
    {
        //수정ui->debugArea->appendPlainText(tr("[+] Başlatıldı\n[+] PID:") + QString::number(proc->processId()) + "\n");
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        trayMenu->actions().at(0)->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(true);
    }
}

void MainWindow::onActionAyarlar()
{
    ayarlar->show();
}

void MainWindow::onActionAbout()
{
    hakkinda.exec();
}

void MainWindow::onDefaultParamCheckState(Qt::CheckState state)
{
    if(state == Qt::Checked)
    {
        ui->comboParametre->setEnabled(true);
        prepareParameters(true);
    }
    else
    {
       ui->comboParametre->setEnabled(false);
       prepareParameters(false);
    }

}

QStringList MainWindow::prepareParameters(bool isComboParametreEnabled)
{
    QStringList defaultparameters;
    QStringList customParameters;
    QStringList quickParameters;
    QStringList param2Box;
    QStringList param1Box;

    //PARAMBOX1
    /*if(settings->value("Parametre/paramP").toBool())
        param1Box << "-p";*/
    if(settings->value("Parametre/paramR").toBool())
        param1Box << "-r";
    /*if(settings->value("Parametre/paramS").toBool())
        param1Box << "-s";
    if(settings->value("Parametre/paramM").toBool())
        param1Box << "-m";*/
    if(settings->value("Parametre/paramF").toString() != "false")
        param1Box << settings->value("Parametre/paramF").toString();
    /*if(settings->value("Parametre/paramK").toString() != "false")
        param1Box << settings->value("Parametre/paramK").toString();
    if(settings->value("Parametre/paramN").toBool())
        param1Box << "-n";*/
    if(settings->value("Parametre/paramE").toString() != "false")
        param1Box << settings->value("Parametre/paramE").toString();
    /*if(settings->value("Parametre/paramA").toBool())
        param1Box << "-a";
    if(settings->value("Parametre/paramW").toBool())
        param1Box << "-w";
    if(settings->value("Parametre/paramPort").toString() != "false")
        param1Box << settings->value("Parametre/paramPort").toString();
    if(settings->value("Parametre/paramIpId").toString() != "false")
        param1Box << settings->value("Parametre/paramIpId").toString();

    //PARAMBOX2
    if(settings->value("Parametre/paramDnsAddr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsAddr").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsv6Addr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Addr").toString();
    if(settings->value("Parametre/paramDnsv6Port").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Port").toString();
    if(settings->value("Parametre/paramBlacklist").toString() != "false")
        param2Box << "--blacklist blacklist.txt";

    //QUICKSETTINGS
    if(settings->value("Parametre/paramQuick").toString() == "-1")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 2" << param2Box;
    if(settings->value("Parametre/paramQuick").toString() == "-2")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-3")
        quickParameters << "-p -r -s -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-4")
        quickParameters << "-p -r -s" << param2Box;
*/
    //DEFAULTPARAMETERS
    //인자 리스트
    switch (ui->comboParametre->currentIndex()) {
    case 0:
        defaultparameters << "-1";
        break;
    }

    //CUSTOMPARAMETERS
    customParameters << param1Box;// << param2Box;

    //UPDATE Parameter Label
    if(isComboParametreEnabled)
    {
        //수정ui->labelParameters->setText("DSB.exe " + defaultparameters.join(" "));
        return defaultparameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        //수정ui->labelParameters->setText("DSB.exe " + customParameters.join(" "));
        return customParameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "false" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        //수정ui->labelParameters->setText("DSB.exe");
        return QStringList();
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "true")
    {
        //수정ui->labelParameters->setText("DSB.exe" + quickParameters.join(" "));
        return QStringList();
    }
    else
    {
        //수정ui->labelParameters->setText("DSB.exe " + quickParameters.join(" "));
        return quickParameters;
    }

}

void MainWindow::catchError(QProcess::ProcessError err)
{
    //수정ui->debugArea->appendPlainText(proc->errorString());
}

void MainWindow::on_btnAyarlar_clicked()
{
    ayarlar->close();
    ayarlar->show();
}

void MainWindow::on_btnAbout_clicked()
{
    hakkinda.close();
    hakkinda.show();
}



void MainWindow::on_btnStart_clicked()
{
    QMovie *Movie=new QMovie(":/images/images/giphy.gif");
    ui->label_3->setMovie(Movie);
    Movie->start();
    ui->label_8->lower();
    ui->btnStart->lower();
    ui->label_9->raise();
    ui->btnStop->raise();
    ui->label_7->setText("STOP");
}

void MainWindow::on_btnStop_clicked()
{
    QMovie *Movie=new QMovie(":/images/images/giphy.gif");
    ui->label_3->setMovie(Movie);
    Movie->stop();
    ui->label_8->raise();
    ui->btnStart->raise();
    ui->label_9->lower();
    ui->btnStop->lower();
    ui->label_7->setText("START");
}

void MainWindow::on_btnClose_clicked()
{
    close();
    //ayarlar.close();
    hakkinda.close();

}

void MainWindow::on_btnMin_clicked()
{
    //setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //    setAttribute(Qt::WA_ShowWithoutActivating);
    auto& traymn = trayMenu;
    this->hide();
    traymn->actions().at(5)->setEnabled(false);
    traymn->actions().at(4)->setEnabled(true);
}
