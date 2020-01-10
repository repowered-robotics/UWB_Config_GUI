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

void configApp::updateAnchorsCallback(){
//    ui->statusBar->showMessage("Updating anchors ... " + QString::number(this->update_count++, 10));
    this->module.readAnchors();
    ui->anchorInfoTextEdit->appendPlainText(this->module.anchorsToString());
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
    //ui->statusBar->showMessage("Channel: " + QString::number(this->module.channel, 10));
    ui->channelBox->setCurrentText(QString::number(this->module.channel, 10));
    ui->numberOfAnchorsLine->setText(QString::number(this->module.numberOfAnchors & 0xFF, 10));
    ui->samplesPerRangeLine->setText(QString::number(this->module.samplesPerRange & 0xFF, 10));

    // display coordinates if this is an anchor
    if(this->module.mode == ANCHOR_MODE){
        ui->xCoord_line->setText(QString().setNum(this->module.x, 'f' , 3));
        ui->yCoord_line->setText(QString().setNum(this->module.y, 'f' , 3));
        ui->zCoord_line->setText(QString().setNum(this->module.z, 'f' , 3));
    }
}

void configApp::on_uploadSettingsBtn_clicked()
{
    bool ok = false;
    this->module.id = ui->deviceIdLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.id = 0xDF; // default ID

    this->module.setMode(ui->modeBox->currentText());

    // update coordinates if this is an anchor
    if(this->module.mode == ANCHOR_MODE){
        this->module.x = ui->xCoord_line->text().toFloat(&ok);
        this->module.y = ui->yCoord_line->text().toFloat(&ok);
        this->module.z = ui->zCoord_line->text().toFloat(&ok);
    }

    this->module.channel = ui->channelBox->currentText().toUInt(&ok, 10);
    if(!ok){
        qDebug() << "Couldn't parse the channel number";
        this->module.channel = 2; //
    }

    this->module.numberOfAnchors = ui->numberOfAnchorsLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.numberOfAnchors = 5;

    this->module.samplesPerRange = ui->samplesPerRangeLine->text().toUInt(&ok, 10);
    if(!ok)
        this->module.samplesPerRange = 1;

    this->module.writeAllFields();
}

void configApp::on_modeBox_currentTextChanged(const QString &arg1)
{
    if(arg1.compare("anchor") == 0){
        ui->xCoord_line->setText("0.0");
        ui->yCoord_line->setText("0.0");
        ui->zCoord_line->setText("0.0");
    }else{
        ui->xCoord_line->setText("---");
        ui->yCoord_line->setText("---");
        ui->zCoord_line->setText("---");
    }
}

void configApp::on_saveConfig_btn_clicked()
{
    this->module.saveConfig();
}

void configApp::on_readAnchorsBtn_clicked()
{
    this->module.readAnchors();
    //const QString newText = this->module.anchorsToString();
    //qDebug() << newText;
    ui->anchorInfoTextEdit->appendPlainText(this->module.anchorsToString());
}
