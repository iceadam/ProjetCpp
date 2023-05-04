#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <QObject>
#include <QSerialPort>
#include <QSqlDatabase>
#include <QSqlQuery>

class RFIDReader : public QObject
{
    Q_OBJECT
public:
    explicit RFIDReader(QObject *parent = nullptr);


signals:

public slots:
    void handleData();

private:
    QSerialPort serialPort;
    QSqlDatabase m_;

    QString RFID;
    QString accessStatus;
    QString roomNumber;


};

class Connectionn
{
public:
    Connectionn();
    bool createconnect();
};


#endif // RFIDREADER_H
