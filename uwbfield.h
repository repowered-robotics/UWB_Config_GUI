#ifndef UWBFIELD_H
#define UWBFIELD_H

#include <QObject>

class UwbField : public QObject
{
    Q_OBJECT
public:
    UwbField(int fieldId, QVariant data);
    UwbField(int fieldId);
    void setFieldId(int fieldId);
    int getFieldId();
    QVariant getData();
    const char* fieldDataType;
    const char* getRawData();
    void setRawData(const char* data);

private:
    const char rawData[4];
    int fieldId;
};

#endif // UWBFIELD_H
