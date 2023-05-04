#ifndef GES_SALLES_H
#define GES_SALLES_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMainWindow>
#include"salle.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ges_salles; }
QT_END_NAMESPACE

class ges_salles : public QMainWindow
{
    Q_OBJECT

public:
    ges_salles(QWidget *parent = nullptr);
    ~ges_salles();

public slots:
    void update_label();

private slots:
    void on_bouton_ajouter_clicked();

    void on_pb_modifier_clicked();

    void on_tabWidget_currentChanged(int);

    void on_table_salle_activated(const QModelIndex &index);

    void on_pb_supprimer_clicked();
    bool controlSaisie();
    void  CLEAR();

    void on_radioButton_a_clicked();

    void on_radioButton_d_clicked();

    void on_pb_meilleur_clicked();

    void on_pb_supprimer_2_clicked();

    void on_pb_supprimer_3_clicked();

    void readSerialData();


    void on_pushButton_clicked();

private:
    QSerialPort serialPort;
    Ui::ges_salles *ui;
      salle ac;


    QByteArray data; // variable contenant les données reçues

    Arduino A; // objet temporaire
};
#endif // GES_SALLES_H
