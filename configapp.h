#ifndef CONFIGAPP_H
#define CONFIGAPP_H

#include <QMainWindow>

namespace Ui {
class configApp;
}

class configApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit configApp(QWidget *parent = nullptr);
    ~configApp();

private:
    Ui::configApp *ui;
};

#endif // CONFIGAPP_H
