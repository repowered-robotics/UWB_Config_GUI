#ifndef UWBMODULE_H
#define UWBMODULE_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QVector>

#define MAX_CMD_SIZE    512
#define MAX_RPY_SIZE    512
#define HDDR_SIZE       2
#define RPY_WORD_SIZE   5
#define UWB_BAUD_RATE   QSerialPort::Baud115200
#define NUM_FIELDS      8
#define FIELD_DATA_SIZE 4
#define CMD_WORD_SIZE   (1 + FIELD_DATA_SIZE)
#define STOP_BYTE       0xA5

#define ANCHOR_DATA_SIZE 33

#define DFLT_SELF_ID 			0xDF
#define DFLT_MODE 	 			0
#define DFLT_CHANNEL 			(uint8_t)2
#define DFLT_SAMPLES_PER_RANGE 	3
#define DFLT_NUMBER_OF_ANCHORS 	5

typedef enum COMMANDS {
    READ         = 0x11,
    READ_ANCHORS = 0x12,
    CONFIG       = 0x22,
    RANGE        = 0x33,
    RESTART      = 0x44,
    RESET        = 0x55,
    SAVE_CONFIG  = 0x66
}UwbCommand;
/*
FIELD_SELF_ID 	= 0x00,
FIELD_MODE 		= 0x01,
FIELD_CHANNEL 	= 0x02,
FIELD_SAMPLES_PER_RANGE 	= 0x03,
FIELD_NUMBER_OF_ANCHORS 	= 0x04,
FIELD_X = 0x05,
FIELD_Y = 0x06,
FIELD_Z = 0x07
*/
typedef enum FIELDS {
    SELF_ID             = 0x00,
    MODE                = 0x01,
    CHANNEL             = 0x02,
    SAMPLES_PER_RANGE   = 0x03,
    NUMBER_OF_ANCHORS   = 0x04,
    FIELD_X             = 0x05,
    FIELD_Y             = 0x06,
    FIELD_Z             = 0x07
} UwbConfigField;

typedef enum MODES {
    TAG_MODE = 0x00,
    ANCHOR_MODE = 0x01
} UwbMode;

class AnchorData
{
public:
    AnchorData() {}
    AnchorData(const AnchorData &anchor);
    uint32_t id;
    uint32_t timestamp;
    float x;
    float y;
    float z;
    float distance;
    float rx_power;
    float fp_power;
    float fp_snr;
    QString toString();
};

class UwbModule : public QObject
{
    Q_OBJECT
public:
    UwbModule() : QObject(nullptr) {}
    void setComport(QString port);
    bool openSuccess;
    uint32_t id;
    uint32_t mode;
    uint32_t channel;
    uint32_t numberOfAnchors;
    float x;
    float y;
    float z;
    QString modestring;
    uint32_t samplesPerRange;
    QString firmwareVersion;
    int readAllFields();
    void writeAllFields();
    QString getId();
    QString getMode();
    void setMode(QString s);
    void setMode(int n);
    int getChannel();
    int getSamplesPerRange();
    void readAnchors();
    void saveConfig();
    QString anchorsToString();

private:
    QVector<AnchorData> anchors;
    QSerialPort serial;
    QByteArray readFields(QByteArray fields);
    void updateObject(QByteArray& rpy);
    int writeFields(QByteArray fieldData);
    uint32_t readFieldAsInt(int fieldId);
    float readFieldAsFloat(int fieldId);
    int addFieldToBuffer(QByteArray& buffer, UwbConfigField fieldId, uint32_t value);
    int addFieldToBuffer(QByteArray& buffer, UwbConfigField fieldId, float value);
    int sendCmdBuffer();
    QString getFirmwareVersionString(char* data);
};

#endif // UWBMODULE_H
