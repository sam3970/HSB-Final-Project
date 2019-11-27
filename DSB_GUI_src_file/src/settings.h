#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void ayarKayit();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onCheckedDefaultParam();       //빠른인자 사용
    void onCheckedCustomParam();        //커스텀 인자 사용
    //void onCheckedSystemTray();         //
    //void onCheckedStartup();
    //void onCheckedSchedule();
    //void onCheckedNotification();

    //void onCheckedQuickSettings();

    //void onCheckedP();
    void onCheckedR();
    //void onCheckedS();
    //void onCheckedM();          //HOST Header Case   (host.com -> hOsT.cOm)
    void onCheckedF();          //단편화 값 HTTP
    //void onCheckedK();
    //void onCheckedN();
    void onCheckedE();          //단편화 값 HTTPS
    //void onCheckedA();
    //void onCheckedW();
    //void onCheckedPort();
    //void onCheckedIpId();
    //void onCheckedDnsAddr();
    //void onCheckedDnsPort();
    //void onCheckedDnsv6Addr();
    //void onCheckedDnsv6Port();
    //void onCheckedBlacklist();

    //void onCheckedRadioQuick1();
    //void onCheckedRadioQuick2();
    //void onCheckedRadioQuick3();
    //void onCheckedRadioQuick4();

    void resetSettings();
    void loadSettings();

signals:
    void defaultParamStateChanged(Qt::CheckState state);
    void updateParameters();

private slots:
    void on_btnClose_clicked();

private:
    Ui::Settings *ui;
    QSettings *ayarR;

    int spinFValue;
    int spinKValue;
    int spinEValue;
    int spinPortValue;
    QString lineIpIdValue;

    QString lineDnsAddrValue;
    int spinDnsPortValue;
    QString lineDnsv6AddrValue;
    int spinDnsv6PortValue;

    //widget move
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    bool pressed = false;
};

#endif // SETTINGS_H
