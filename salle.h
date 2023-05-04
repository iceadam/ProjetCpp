#ifndef SALLE_H
#define SALLE_H
#include<QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSlice>
class salle
{
private:
int NUMERO;
QString CATEGORIE,DISPONIBILITE;
int CAPACITE,TEMPS,SURFACE;
public:
    salle();
    salle(int,QString,QString,int,int,int);
    void set_NUMERO(int);
    void set_CATEGORIE(QString);
    void set_DISPONIBILITE(QString);
    void set_CAPACITE(int);
    void set_TEMPS(int);
    void set_SURFACE(int);

    int get_NUMERO();
    QString get_CATEGORIE();
    QString get_DISPONIBILITE();
    int get_CAPACITE();
    int get_TEMPS();
    int get_SURFACE();

    QSqlQueryModel* GETALL();
    QSqlQueryModel* FINDBYNUMERO(int);
    bool ADD();
    bool employexiste(QString NUMERO);
    bool UPDATE();
    QSqlQueryModel* trie(QString croissance,QString critere);
    QChartView *stat_capacite();
    bool DELETEE(int);

};

#endif // SALLE_H
