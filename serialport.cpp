#include "serialport.h"


std::vector<QString> getComPorts(){
    std::vector<QString> ports = {};

    wchar_t lpTargetPath[5000]; // to store the path of the COM ports

    for(int i = 0; i < 256; i++){
        std::string comStr = "COM" + std::to_string(i);
        const wchar_t* str = std::wstring(comStr.begin(), comStr.end()).c_str();
        DWORD test = QueryDosDevice(str, lpTargetPath, 5000);

        if(test != 0){
            ports.push_back(QString(comStr.c_str()));
        }
    }

    return ports;
}


/**
 * @brief SerialPort::SerialPort will open and initialize the com port
 * also see https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-opencommport
 * @param parent
 * @param port the string name of the COM port
 */
SerialPort::SerialPort(QObject *parent, QString port) : QObject(parent), comPort(port)
{
    std::string comStr = comPort.toStdString();
    const wchar_t* comWideStr = std::wstring(comStr.begin(), comStr.end()).c_str();
    hSerial = CreateFile(comWideStr,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         nullptr,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         nullptr);

    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if(!GetCommState(hSerial, &dcbSerialParams)){
        // uh oh
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
        // error setting serial port state
    }

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout        = 50;
    timeouts.ReadTotalTimeoutConstant   = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant  = 50;
    timeouts.WriteTotalTimeoutMultiplier= 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
        // error occurred, uh oh
    }
}

bool SerialPort::writeData(uint8_t* data, int writeSize){
    bool retval;
    DWORD dwBytesWritten = 0;
    retval = WriteFile(this->hSerial, data, (unsigned long)writeSize, &dwBytesWritten, nullptr);
    return retval && dwBytesWritten == (unsigned long)writeSize;
}

bool SerialPort::readData(uint8_t* data, int readSize){
    bool retval;
    DWORD dwBytesRead = 0;
    retval = ReadFile(this->hSerial, data, (unsigned long)readSize, &dwBytesRead, nullptr);
    return retval && dwBytesRead == (unsigned long)readSize;
}

void SerialPort::close(){
    CloseHandle(this->hSerial);
}
