#include "configapp.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    configApp w;
    w.show();
    QTimer *timer = new QTimer(&w);
    QMainWindow::connect(timer, &QTimer::timeout, &w, QOverload<>::of(&configApp::updateAnchorsCallback));
    timer->start(1000);

    return a.exec();
}
