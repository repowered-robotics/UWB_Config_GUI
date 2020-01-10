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

    void updateAnchorsCallback();

private slots:
    void on_connectBtn_clicked();

    void on_refreshToolBtn_clicked();

    void on_readSettingsBtn_clicked();

    void on_uploadSettingsBtn_clicked();

    void on_modeBox_currentTextChanged(const QString &arg1);

    void on_saveConfig_btn_clicked();

    void on_readAnchorsBtn_clicked();

private:
    UwbModule module;
    Ui::configApp *ui;
    int update_count = 0;
};

#endif // CONFIGAPP_H
