#include "configapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    configApp w;
    w.show();

    return a.exec();
}
