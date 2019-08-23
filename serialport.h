#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

#include "Windows.h"

class serialPort : public QObject
{
    Q_OBJECT
public:
    explicit serialPort(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERIALPORT_H
