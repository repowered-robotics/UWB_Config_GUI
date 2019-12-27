#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include "Windows.h"


std::vector<QString> getComPorts();

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr, QString port = nullptr);
    QString comPort;
    bool writeData(uint8_t* data, int writeSize);
    bool readData(uint8_t* data, int readSize);
    void close();

signals:

public slots:

private:
    HANDLE hSerial;
};

#endif // SERIALPORT_H
