#include "arduino.h"
#include "rfid.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QObject>

RFID::RFID(QObject *parent) : QObject(parent)
{
    //*************Arduino*******************************************************************
    int ret = A.connect_arduino(); // lancer la connexion à arduino
    switch (ret) {
    case(0): qDebug() << "arduino is available and connected to : " << A.getarduino_port_name();
        break;
    case(1): qDebug() << "arduino is available but not connected to :" << A.getarduino_port_name();
        break;
    case(-1): qDebug() << "arduino is not available";
    }

    QObject::connect(A.getserial(), &QSerialPort::readyRead, this, &RFID::update_label); // permet de lancer
    //le slot update_label suite à la reception du signal readyRead (reception des données).

    while (true) {
        if (A.getserial()->waitForReadyRead()) {
            QByteArray data = A.getserial()->readAll();
            if (!data.isEmpty()) {
                QString strData = QString::fromUtf8(data);
                qDebug() << "Received data from Arduino: " << strData;

                // insert the data into the SQL database
                QSqlQuery query;
                query.prepare("INSERT INTO RFID (RFID , ACCESSSTATUS , ROOMNUMBER) VALUES (:RFID , :accessStatus , :roomNumber)");
                query.bindValue(":data", strData);
                if (!query.exec()) {
                    qWarning() << "Failed to insert data into database";
                }
            }
        }
    }
}

RFID::~RFID()
{

}

