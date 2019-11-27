#include "settings.h"
#include "ui_settings.h"

#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent,Qt::FramelessWindowHint),
    ui(new Ui::Settings),
    ayarR(new QSettings("HexOpenSource", "DSB_GUI")),
    spinFValue(0),
    spinKValue(0),
    spinEValue(0),
    spinPortValue(0),
    lineIpIdValue(""),
    lineDnsAddrValue("208.67.220.220"),
    spinDnsPortValue(1253),
    lineDnsv6AddrValue("2a02:6b8::feed:0ff"),
    spinDnsv6PortValue(1253)
{
    ui->setupUi(this);

    QPixmap pix1(":/images/images/settings-gears-button.png");
    ui->label_11->setPixmap(pix1);
    ui->label_11->setScaledContents(true);
    ui->label_11->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label_10->setStyleSheet("QLabel {color : white; }");
    /*setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    setWindowIcon(QIcon(":/images/images/settings-gears-button.png"));
    setWindowTitle("설정");*/

    //Reference to QSettings for capturing it from lambda function
    auto& ayar = ayarR;

    //ui->radioQuick1->setChecked(true);

    //Determine if its first run then create or not registry settings
    //TODO: Inherit from QSettings and make efficient save/restore/load mechanism.
    if(!ayarR->contains("Parametre/defaultParam"))
    {
        resetSettings();
    }

    //SIGNAL&SLOTS for parameters and inputs change
    // TODO: Turn all Checkbox::stateChanged signals to CheckBox::stateChanged
    connect(ui->checkDefaultParam, &QCheckBox::stateChanged, this, &Settings::onCheckedDefaultParam);
    connect(ui->checkCustomParam, &QCheckBox::stateChanged, this, &Settings::onCheckedCustomParam);

    if(ui->checkF->checkState() == Qt::Checked){
            //spinFValue = ui->spinF->value();
        ayar->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
    }

    if(ui->checkE->checkState() == Qt::Checked){
            //spinEValue = ui->spinE->value();
        ayar->setValue("Parametre/paramE", "-e " + QString::number(spinEValue));
    }
    connect(ui->checkR, &QCheckBox::stateChanged, this, &Settings::onCheckedR);                     //host header   hoSt
    connect(ui->checkF, &QCheckBox::stateChanged, this, &Settings::onCheckedF);                     //http fragmentation
    connect(ui->checkE, &QCheckBox::stateChanged, this, &Settings::onCheckedE);                     //http fragmentation
    loadSettings();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::closeEvent(QCloseEvent *event)
{
    if(this->isVisible() || this->isTopLevel())
    {
        event->ignore();
        this->hide();
    }
    else
    {
    }

    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedDefaultParam()
{
    if(ui->checkDefaultParam->checkState() == Qt::Checked)
    {
        ui->paramBox->setEnabled(false);
        ui->checkCustomParam->setCheckState(Qt::Unchecked);
        ayarR->setValue("Parametre/defaultParam", true);
        ayarR->setValue("Parametre/customParam", false);
        ayarR->setValue("Parametre/quickSettings", false);
    }
    else
    {

    }
    //Capturing change from MainWindow, Thus we can control ComboBox for Parameter List.
    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}
//직접설정
void Settings::onCheckedCustomParam()
{
    if(ui->checkCustomParam->checkState() == Qt::Checked)
    {
        ui->checkDefaultParam->setChecked(Qt::Unchecked);
        ui->paramBox->setEnabled(true);
        ayarR->setValue("Parametre/customParam", true);
        ayarR->setValue("Parametre/defaultParam", false);
    }
    else
    {
        ui->paramBox->setEnabled(false);
        ayarR->setValue("Parametre/customParam", false);
    }
    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedR()
{
    if(ui->checkR->checkState() == Qt::Checked)
    {
        ayarR->setValue("Parametre/paramR", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramR", false);
    }
}
void Settings::onCheckedF()
{
    if(ui->checkF->checkState() == Qt::Checked)
    {
        ayarR->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramF", false);
    }
}
void Settings::onCheckedE()
{
    if(ui->checkE->checkState() == Qt::Checked)
    {
        ayarR->setValue("Parametre/paramE", "-e " + QString::number(spinEValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramE", false);
    }
}

void Settings::resetSettings()
{
    ayarR->clear();

    ayarR->beginGroup("Parametre");
    ayarR->setValue("defaultParam", true);
    ayarR->setValue("customParam", false);
    ayarR->setValue("quickSettings", false);
    ayarR->setValue("paramR", false);
    ayarR->setValue("paramF", false);
    ayarR->setValue("paramE", false);
    ayarR->endGroup();
}

/**
 * @brief Settings::loadSettings
 *
 * I aggree with you It's not efficient.
 * It's highest priority for me to make it more efficient.
 * I don't like hard coded things either :)
 *
 */
void Settings::loadSettings()
{
    if(ayarR->value("Parametre/defaultParam").toBool())
    {
        ui->checkDefaultParam->setChecked(true);
        ui->paramBox->setEnabled(false);
    }
    else
    {
        ui->checkDefaultParam->setChecked(false);
    }

    if(ayarR->value("Parametre/customParam").toBool())
    {
         ui->checkCustomParam->setChecked(true);
         ui->paramBox->setEnabled(true);
         if(ayarR->value("Parametre/quickSettings").toBool())
         {
             ui->paramBox->setEnabled(false);
         }
    }
    else
    {
        ui->checkCustomParam->setChecked(false);
        ui->paramBox->setEnabled(false);
    }

    if(ayarR->value("Parametre/quickSettings").toBool())
    {
        ui->paramBox->setEnabled(false);
    }
    else
    {
        ui->paramBox->setEnabled(true);
    }


    if(ayarR->value("Parametre/paramR").toBool())
    {
        ui->checkR->setChecked(true);
    }
    else
    {
        ui->checkR->setChecked(false);
    }

    if(ayarR->value("Parametre/paramF").toString() != "false")
    {
        ui->checkF->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramF").toString();
        QStringList list = tmp.split(" ");

        //ui->spinF->setValue(list.at(1).toInt());
        spinFValue = list.at(1).toInt();
    }
    else
    {
        ui->checkF->setChecked(false);
    }

    if(ayarR->value("Parametre/paramE").toString() != "false")
    {
        ui->checkE->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramE").toString();
        QStringList list = tmp.split(" ");

        //ui->spinE->setValue(list.at(1).toInt());
        spinEValue = list.at(1).toInt();
    }
    else
    {
        ui->checkE->setChecked(false);
    }
}
void Settings::mousePressEvent(QMouseEvent *event) {
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

void Settings::mouseMoveEvent(QMouseEvent *event) {
    if(pressed == true){
        move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}
void Settings::on_btnClose_clicked()
{
    this->close();
}
