#include "configapp.h"
#include "ui_configapp.h"


configApp::configApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::configApp)
{
    ui->setupUi(this);
}

configApp::~configApp()
{
    delete ui;
}

void configApp::on_connectBtn_clicked()
{
    // attempt to connect to the com port selected in comPortBox
    int ind = ui->comPortBox->currentIndex();

    if(ind < 0){
        ui->statusBar->showMessage("No COM ports found");
        return;
    }else{
        ui->statusBar->showMessage(ui->comPortBox->currentText());
    }

    ui->statusBar->showMessage(ui->comPortBox->currentText());
    this->module.setComport(ui->comPortBox->currentText());
    if(this->module.openSuccess){
        ui->statusBar->showMessage(ui->comPortBox->currentText() + " opened");
    }else{
        ui->statusBar->showMessage(ui->comPortBox->currentText() + " not opened");
    }

}

void configApp::on_refreshToolBtn_clicked()
{
    // populate the comPortBox with COM port strings

    // first, clear the comPortBox
    ui->comPortBox->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for(QSerialPortInfo port : ports){
        ui->comPortBox->addItem(port.portName(), QVariant(port.portName()));
    }

}

void configApp::on_readSettingsBtn_clicked()
{
    int status = this->module.readAllFields();     // updates the field values
    if(status != 0){
        ui->statusBar->showMessage("Read all fields failed");
    }else{
        ui->statusBar->showMessage("Read all fields success!");
    }
    ui->firmwareVersionLabel->setText(this->module.firmwareVersion);

    ui->deviceIdLine->setText(QString::number(this->module.id & 0xFF, 10));

    ui->modeBox->setCurrentText(this->module.modestring);
    ui->statusBar->showMessage("Channel: " + QString::number(this->module.channel, 10));
    ui->channelBox->setCurrentText(QString::number(this->module.channel, 10));
    ui->numberOfAnchorsLine->setText(QString::number(this->module.numberOfAnchors & 0xFF, 10));
    ui->samplesPerRangeLine->setText(QString::number(this->module.samplesPerRange & 0xFF, 10));
}

void configApp::on_uploadSettingsBtn_clicked()
{
    bool ok = false;
    this->module.id = ui->deviceIdLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.id = 0xDF; // default ID

    this->module.setMode(ui->modeBox->currentText());

    this->module.channel = ui->channelBox->currentText().toUInt(&ok, 10);
    if(!ok)
        this->module.channel = 2; //

    this->module.numberOfAnchors = ui->numberOfAnchorsLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.numberOfAnchors = 5;

    this->module.samplesPerRange = ui->samplesPerRangeLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.samplesPerRange = 1;

    this->module.writeAllFields();
}
