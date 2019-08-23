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
