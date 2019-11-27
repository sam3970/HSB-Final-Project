#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString systemLang = QLocale::languageToString(QLocale::system().language());
    QString style = "dlg::tile {"" height:24px;""font-weight:bold;""color:##1e1d2f;""background:#1e1d2f;""}";
    QTranslator t;

    a.setStyleSheet(style);


    if(systemLang == "Turkish")
    {

    }

    else if (systemLang == "Korean")
    {
        t.load(":/lang_en.qm");
        a.installTranslator(&t);
    }

    else//한글변환
    {
        t.load(":/lang_ko.qm");
        a.installTranslator(&t);
    }

    MainWindow w(a.arguments());


    QStringList argList = a.arguments();
    QString argument = argList.join(",");

    bool silent = argument.contains("-silent");

    if(!silent)
    {
        w.show();
    }


    return a.exec();
}
