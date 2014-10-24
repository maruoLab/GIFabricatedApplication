#include "masterthread.h"

#include <QSerialPort>

#include <QTime>
#include <QDebug>
#include <QJsonObject>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent),
    waitTime(1000)
{
}

MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void MasterThread::setAxis(const MasterThread::Axis &axis)
{
    this->axis = axis;
}

void MasterThread::read(const QJsonObject &json)
{
    this->portName = json["portName"].toString();
    this->waitTime = json["waitTime"].toInt();
    this->baudrate = json["baudrate"].toInt();
    this->parity   = (QSerialPort::Parity)json["parity"].toInt();
    this->stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
}

void MasterThread::write(QJsonObject &json) const
{
    json["portName"] = this->portName;
    json["waitTime"] = this->waitTime;
    json["baudrate"] = this->baudrate;
    json["parity"]   = this->parity;
    json["stopbits"] = this->stopbits;
}

void MasterThread::transaction(QString &request)
{
    QMutexLocker locker(&mutex);

    if (!isRunning())
    {
        start();
    }else
    {
        cond.wakeOne();
    }
}

void MasterThread::setSerialSettings(const QString &portName, int waitTime, int baudrate, QSerialPort::Parity parity, QSerialPort::StopBits stopbits)
{

    this->portName = portName;
    this->waitTime = waitTime;
    this->baudrate = baudrate;
    this->stopbits = stopbits;
    this->parity = parity;
}

void MasterThread::run()
{

    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName)
    {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTime;
    QString currentRequest = request;
    mutex.unlock();

    QSerialPort serial;

    while(!quit)
    {

        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(this->baudrate);
            serial.setStopBits(this->stopbits);
            serial.setParity(this->parity);
            serial.setDataBits(serial.Data8);

            if (!serial.open(QIODevice::ReadWrite)){
                emit error(tr("Can't open %1, errorcode %2").arg(portName).arg(serial.error()));
                return;
            }
        }


        //write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTime))
        {
            //read response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                {
                    responseData += serial.readAll();
                }

                qDebug() << responseData;
                QString response(responseData);
                emit this->response(response);

            }else{

                emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
            }
        }else{

            emit timeout(tr("Wait write request timeout 1").arg(QTime::currentTime().toString()));
        }

        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName)
        {
            currentPortName = portName;
            currentPortNameChanged = true;
        }else{

            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTime;
        currentRequest = request;
        mutex.unlock();
    }
}
