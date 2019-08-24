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
    }else{
        ui->statusBar->showMessage(ui->comPortBox->currentText());
    }
}

void configApp::on_refreshToolBtn_clicked()
{
    // populate the comPortBox with COM port strings

//    DWORD maxNumPorts = 256;
//    DWORD portNumbers[256];
//    DWORD numPortsFound = 0;
//    DWORD result = GetCommPorts(portNumbers, maxNumPorts, &numPortsFound);

//    if(result == ERROR_SUCCESS){
//        for(DWORD i = 0; i < numPortsFound; i++){
//            QString comStr = "COM" + QString(std::to_string((int)portNumbers[i]).c_str());
//            ui->comPortBox->addItem(comStr, QVariant(comStr));
//        }
//    }

    // first, clear the comPortBox
    ui->comPortBox->clear();

    std::vector<QString> ports = getComPorts();

    for(QString s : ports){
        ui->comPortBox->addItem(s, QVariant(s));
    }

}
