#include "uwbmodule.h"


void UwbModule::setComport(QString port)
{
    this->serial.setPortName(port);
    this->serial.setBaudRate(UWB_BAUD_RATE, QSerialPort::AllDirections);
    this->serial.setDataBits(QSerialPort::Data8);
    this->serial.setFlowControl(QSerialPort::NoFlowControl);
    this->serial.setStopBits(QSerialPort::OneStop);
    this->serial.setParity(QSerialPort::NoParity);
    this->openSuccess = this->serial.open(QIODevice::ReadWrite);
}

/**
 * @brief UwbModule::readAllFields reads all fields from the UWB module and updates public members
 */
int UwbModule::readAllFields(){
    QByteArray fields = QByteArray();
    fields.append(char(SELF_ID));
    fields.append(char(MODE));
    fields.append(char(CHANNEL));
    fields.append(char(NUMBER_OF_ANCHORS));
    fields.append(char(SAMPLES_PER_RANGE));
    fields.append(char(FIELD_X));
    fields.append(char(FIELD_Y));
    fields.append(char(FIELD_Z));

    QByteArray rpy = this->readFields(fields); // formulate packet, send it, and read the response
    if(rpy == nullptr)
        return -1; // something went wrong

    this->updateObject(rpy); // update our field data based on the reply
    return 0;
}

/**
 * @brief UwbModule::writeAllFields update the connected module from instance data
 */
void UwbModule::writeAllFields(){
    QByteArray data = QByteArray();
    int body_size = 0;
    body_size += this->addFieldToBuffer(data, SELF_ID, this->id);
    body_size += this->addFieldToBuffer(data, MODE, this->mode);
    body_size += this->addFieldToBuffer(data, CHANNEL, this->channel);
    body_size += this->addFieldToBuffer(data, NUMBER_OF_ANCHORS, this->numberOfAnchors);
    body_size += this->addFieldToBuffer(data, SAMPLES_PER_RANGE, this->samplesPerRange);
    body_size += this->addFieldToBuffer(data, FIELD_X, this->x);
    body_size += this->addFieldToBuffer(data, FIELD_Y, this->y);
    body_size += this->addFieldToBuffer(data, FIELD_Z, this->z);
    //data[1] = char(body_size);
    //data.append(char(STOP_BYTE));
    int write_status = this->writeFields(data);
    if(write_status != 0){
        qDebug() << "Write error";
    }else{
        qDebug() << "Write success!";
    }
    QByteArray rpy;
    while(this->serial.waitForReadyRead(250))
        rpy.append(this->serial.readAll());

}

/**
 * @brief UwbModule::updateObject updates instance fields from the raw reply from the UWB module
 * @param rpy The QByteArray read from the UWB module
 */
void UwbModule::updateObject(QByteArray& rpy_arr){
    const char* rpy = rpy_arr.data(); // get the backbuffer

    // make sure this is a reply to a READ command
    if(rpy[0] != char(READ))
        return;

    int field = 0; // temporary
    int ind;
    for(ind = HDDR_SIZE; ind < rpy[1] + HDDR_SIZE; ind += FIELD_DATA_SIZE){
        field = rpy[ind]; // field ID
        ind++;            // point to the field data
        switch(field){
        case SELF_ID:{
            //memcpy(&this->id, rpy_data + ind, FIELD_DATA_SIZE);
            this->id = (uint32_t)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case MODE:{
            this->mode = (uint32_t)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            if(this->mode == TAG_MODE){
                this->modestring = QString("tag");
            }else if(this->mode == ANCHOR_MODE){
                this->modestring = QString("anchor");
            }else{
                this->modestring = QString("---");
            }
            break;
        }case CHANNEL:{
            this->channel = (uint32_t)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case SAMPLES_PER_RANGE:{
            this->samplesPerRange = (uint32_t)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case NUMBER_OF_ANCHORS:{
            this->numberOfAnchors = (uint32_t)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case FIELD_X:{
            this->x = (float)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case FIELD_Y:{
            this->y = (float)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }case FIELD_Z:{
            this->z = (float)( (rpy[ind]) | (rpy[ind+1] << 8) | (rpy[ind+2] << 16) | (rpy[ind+3] << 24) );
            break;
        }default:
            break;
        }
    }

}

QString UwbModule::getFirmwareVersionString(char* data){
    // [year].[month].[day], Rev [revision]
    QString retval = QString::number(data[0], 10);
    retval.append('.');
    retval.append(QString::number(data[1], 10));
    retval.append('.');
    retval.append(QString::number(data[2], 10));
    retval.append(", Rev ");
    retval.append(QString::number(data[3], 10));
    return retval;
}

/**
 * @brief UwbModule::getId reads the ID from the UWB module and returns it as a QString
 * @return QString base-10 representation of the module ID
 */
QString UwbModule::getId(){
    QByteArray fields = QByteArray(1, '\0');
    fields[0] = SELF_ID;
    QByteArray rpy = this->readFields(fields);
    char* rpy_buf = rpy.data();

    uint32_t id = 0;
    memcpy(&id, rpy_buf + HDDR_SIZE + 1, sizeof(id));
    return QString::number(id, 10);
}


QString UwbModule::getMode(){
    uint32_t mode = this->readFieldAsInt(MODE);

    if(mode == TAG_MODE){
        return "tag";
    }else if(mode == ANCHOR_MODE){
        return "anchor";
    }else{
        return "tag";
    }
}


void UwbModule::setMode(QString s){
    if(s.compare("tag") == 0){
        this->modestring = "tag";
        this->mode = uint32_t(TAG_MODE);
    }else if(s.compare("anchor") == 0){
        this->modestring = "anchor";
        this->mode = uint32_t(ANCHOR_MODE);
    }else{
        this->modestring = "tag";
        this->mode = uint32_t(TAG_MODE);
    }
}

void UwbModule::setMode(int n){
    if(n == TAG_MODE){
        this->modestring = "tag";
        this->mode = uint32_t(TAG_MODE);
    }else if(n == ANCHOR_MODE){
        this->modestring = "anchor";
        this->mode = uint32_t(ANCHOR_MODE);
    }else{
        this->modestring = "tag";
        this->mode = uint32_t(TAG_MODE);
    }
}

int UwbModule::getChannel(){
    return int(this->readFieldAsInt(CHANNEL));
}

int UwbModule::getSamplesPerRange(){
    return int(this->readFieldAsInt(SAMPLES_PER_RANGE));
}


uint32_t UwbModule::readFieldAsInt(int fieldId){
    QByteArray fields = QByteArray(1, char(fieldId));
    QByteArray rpy = this->readFields(fields);
    char* rpy_buf = rpy.data();
    uint32_t retval = 0;
    memcpy(&retval, rpy_buf+HDDR_SIZE+1, sizeof(retval));
    return retval;
}

float UwbModule::readFieldAsFloat(int fieldId){
    QByteArray fields = QByteArray(1, char(fieldId));
    QByteArray rpy = this->readFields(fields);
    char* rpy_buf = rpy.data();
    float retval = 0;
    memcpy(&retval, rpy_buf+HDDR_SIZE+1, sizeof(retval));
    return retval;
}

/**
 * @brief UwbModule::readFields
 * @param fields        array of field ID's to read
 * @param nFields       number of fields to read
 * @retval QByteArray   bytes read from the module
 */
QByteArray UwbModule::readFields(QByteArray fields){
    if(!this->serial.isOpen()){
        return nullptr;
    }
    int nFields = fields.length();

    int size     = 0;
    if(nFields < MAX_CMD_SIZE - (HDDR_SIZE + 1)){
        size = nFields;
    }else{
        size = MAX_CMD_SIZE - (HDDR_SIZE + 1);
    }
    QByteArray sendData = QByteArray(fields);
    sendData.prepend(char(nFields & 0xFF));
    sendData.prepend(char(READ));
    sendData.append(char(STOP_BYTE & 0xFF));
    QString dbg;
    dbg.append("Send data (" + QString::number(sendData.length(), 10) + " bytes): ");
    for(int i = 0; i < sendData.length(); i++){
        dbg.append(QString::number(sendData[i], 16) + " ");
    }
    qDebug() << dbg;
    this->serial.write(sendData);


    QByteArray response;
    // wait for the module to send some bytes
    while(this->serial.waitForReadyRead(250))
        response.append(this->serial.readAll());

    dbg = QString("Reply data (" + QString::number(response.length(), 10) + " bytes): ");
    for(int i = 0; i < response.length(); i++){
        dbg.append(QString::number(response[i], 10) + " ");
    }
    qDebug() << dbg;

//    char hddr[HDDR_SIZE];

//    if(this->serial.read(hddr, HDDR_SIZE) != 0){
//        return nullptr;
//    }
//    int readSize = hddr[1]; // size of the body plus stop byte

//    char* body = (char*)malloc(readSize);
//    if(this->serial.read(body, readSize) != 0)
//        return nullptr;

//    char stop[1];
//    this->serial.read(stop, 1);

//    response.append(hddr);
//    response.append(body);
//    response.append(stop);

    return response;
}

/**
 * @brief UwbModule::writeFields write the value of fieldData to the UWB module
 * @param fieldData QBytesArray of the data to write
 * @return 0 if success, -1 if fieldData is formatted wrong, -2 if something wierd happened
 */
int UwbModule::writeFields(QByteArray fieldData){
    int retval = 0;
    int dataLen = fieldData.length();

    // make sure this makes sense
    if(dataLen % CMD_WORD_SIZE != 0){
        return -1; // fail
    }

    QByteArray sendData = QByteArray(fieldData);

    sendData.prepend(char(dataLen));
    sendData.prepend(char(CONFIG));
    sendData.append(char(STOP_BYTE));

    QString dbg;
    dbg.append("Send data (" + QString::number(sendData.length(), 10) + " bytes): ");
    for(int i = 0; i < sendData.length(); i++){
        dbg.append(QString::number(sendData[i], 10) + " ");
    }
    qDebug() << dbg;

    long long writeSize = this->serial.write(sendData);

    if(writeSize == sendData.length()){
        retval = 0; // success!
    }else{
        retval = -2; // hmm
    }

    return retval;
}


int UwbModule::addFieldToBuffer(QByteArray& buffer, UwbConfigField fieldId, uint32_t value){
    char* temp = new char[FIELD_DATA_SIZE];
    memcpy(temp, &value, FIELD_DATA_SIZE);
    buffer.append(char(fieldId));
    buffer.append(temp[0]); // LSB copied first
    buffer.append(temp[1]);
    buffer.append(temp[2]);
    buffer.append(temp[3]); // MSB copied last

    return 1 + FIELD_DATA_SIZE;
}

int UwbModule::addFieldToBuffer(QByteArray& buffer, UwbConfigField fieldId, float value){
    char* temp = new char[FIELD_DATA_SIZE];
    memcpy(temp, &value, FIELD_DATA_SIZE);
    buffer.append(char(fieldId));
    // convert from big endian to little endian
    buffer.append(temp[0]); // LSB copied first
    buffer.append(temp[1]);
    buffer.append(temp[2]);
    buffer.append(temp[3]); // MSB copied last

    return 1 + FIELD_DATA_SIZE;
}
