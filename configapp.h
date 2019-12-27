#ifndef CONFIGAPP_H
#define CONFIGAPP_H

#include <QMainWindow>
#include <QSerialPortInfo>
//#include "serialport.h"
//#include "WinBase.h"
//#include "Windows.h"
#include "uwbmodule.h"

namespace Ui {
class configApp;
}

class configApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit configApp(QWidget *parent = nullptr);
    ~configApp();

private slots:
    void on_connectBtn_clicked();

    void on_refreshToolBtn_clicked();

    void on_readSettingsBtn_clicked();

    void on_uploadSettingsBtn_clicked();

private:
    UwbModule module;
    Ui::configApp *ui;
};

#endif // CONFIGAPP_H
