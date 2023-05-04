#ifndef RFID_H
#define RFID_H
#include <QObject>
#include <QString>
#include "arduino.h"

class RFID : public QObject
{
    Q_OBJECT
public:
    explicit RFID();
    virtual ~RFID();

private:
    Arduino A;

private slots:
    void update_label();
};

#endif // RFID_H
