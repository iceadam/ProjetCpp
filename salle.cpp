#include "salle.h"
#include <QSqlQuery>
#include<QDebug>
#include <QSqlQueryModel>
#include <QObject>
#include <QSqlError>
salle::salle()
{
      NUMERO=0;
      CATEGORIE="";
      DISPONIBILITE="";
      SURFACE=0;
      CAPACITE=0;
      TEMPS=0;
}
salle::salle(int NUMERO,QString CATEGORIE,QString DISPONIBILITE,int SURFACE,int CAPACITE,int TEMPS){
    this->NUMERO=NUMERO;
    this->CATEGORIE=CATEGORIE;
    this->DISPONIBILITE=DISPONIBILITE;
    this->SURFACE=SURFACE;
    this->CAPACITE=CAPACITE;
    this->TEMPS=TEMPS;};

void salle::set_NUMERO(int NUMERO){this->NUMERO=NUMERO;};
void salle::set_CATEGORIE(QString CATEGORIE){this->CATEGORIE=CATEGORIE;};
void salle::set_DISPONIBILITE(QString DISPONIBILITE){this->DISPONIBILITE=DISPONIBILITE;};
void salle::set_SURFACE(int SURFACE){this->SURFACE=SURFACE;};
void salle::set_CAPACITE(int CAPACITE){this->CAPACITE=CAPACITE;};
void salle::set_TEMPS(int TEMPS){this->TEMPS=TEMPS;};

int salle::get_NUMERO(){return NUMERO;};
QString salle::get_CATEGORIE(){return CATEGORIE;};
QString salle::get_DISPONIBILITE(){return DISPONIBILITE;};
int salle::get_SURFACE(){return SURFACE;};
int salle::get_CAPACITE(){return CAPACITE;};
int salle::get_TEMPS(){return TEMPS;};

QSqlQueryModel* salle::GETALL()
{
    QSqlQueryModel* model=new QSqlQueryModel() ;

    model->setQuery("SELECT * FROM GES_SALLE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("NUMERO"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CATEGORIE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DISPONIBILITE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TEMPS"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SURFACE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CAPACITE"));

    return model;
}

bool salle::DELETEE(int NUMERO)
{
    QSqlQuery query;
    query.prepare("Delete from GES_SALLE where NUMERO=:NUMERO");
     query.bindValue(":NUMERO",NUMERO);
     return  query.exec();
}

QSqlQueryModel* salle::FINDBYNUMERO(int NUMERO){
    QSqlQueryModel *modal=new QSqlQueryModel();
    modal->setQuery("select * from GES_SALLE where NUMERO like '%"+QString::number(NUMERO)+"%'");
    return modal;};

bool salle::ADD(){
     QSqlQuery query;
    query.prepare("INSERT INTO GES_SALLE (NUMERO,CATEGORIE,DISPONIBILITE,SURFACE,CAPACITE,TEMPS) "
                   "VALUES (:NUMERO,:CATEGORIE,:DISPONIBILITE,:SURFACE,:CAPACITE,:TEMPS)");
     query.bindValue(":NUMERO",NUMERO);
     query.bindValue(":CATEGORIE", CATEGORIE);
     query.bindValue(":DISPONIBILITE", DISPONIBILITE);
     query.bindValue(":SURFACE", SURFACE);
     query.bindValue(":CAPACITE", CAPACITE);
     query.bindValue(":TEMPS", TEMPS);
    return  query.exec();};

bool salle:: employexiste(QString NUMERO)
{
   bool exists = false;
   QSqlQuery checkQuery;
   checkQuery.prepare("SELECT NUMERO FROM GES_SALLE WHERE NUMERO=:NUMERO");
   checkQuery.bindValue(":NUMERO", NUMERO);
   if (checkQuery.exec())
   { if (checkQuery.next())
       { exists = true;}
   }
   else
   {qDebug() << "salle not found:" << checkQuery.lastError();}
return exists;
}

bool salle::UPDATE(){
    QSqlQuery query;
    query.prepare("update GES_SALLE set CATEGORIE=:CATEGORIE,DISPONIBILITE=:DISPONIBILITE,SURFACE=:SURFACE,CAPACITE=:CAPACITE,TEMPS=:TEMPS WHERE NUMERO=:NUMERO");
    query.bindValue(":NUMERO",NUMERO);
    query.bindValue(":CATEGORIE", CATEGORIE);
    query.bindValue(":DISPONIBILITE", DISPONIBILITE);
    query.bindValue(":SURFACE", SURFACE);
    query.bindValue(":CAPACITE", CAPACITE);
    query.bindValue(":TEMPS", TEMPS);
    return query.exec();
     };
QSqlQueryModel *salle::trie(QString croissance,QString critere)
{
   QSqlQueryModel *modal=new QSqlQueryModel();

   if(critere == "NUMERO" && croissance == "ASC")
       modal->setQuery("select * from GES_SALLE order by NUMERO ASC ");
   else if(critere == "NUMERO" && croissance == "DESC")
       modal->setQuery("select * from GES_SALLE order by NUMERO DESC ");

   else if(critere == "CAPACITE" && croissance == "ASC")
       modal->setQuery("select * from GES_SALLE order by CAPACITE ASC ");

   else if(critere == "CAPACITE" && croissance == "DESC")
       modal->setQuery("select * from GES_SALLE order by CAPACITE DESC ");

   else if(critere == "TEMPS" && croissance == "ASC")
       modal->setQuery("select * from GES_SALLE order by TEMPS ASC ");
   else if(critere == "TEMPS" && croissance == "DESC")
       modal->setQuery("select * from GES_SALLE order by TEMPS DESC ");

   return  modal;
}
QChartView *salle::stat_capacite()
  {
      int a1=0;
      int a2=0;

      QSqlQuery query;
      query.prepare("select * from GES_SALLE where ((CAPACITE>=10)and(CAPACITE<=40))");
      query.exec();

      while(query.next())
          a1++;

      query.prepare("select * from GES_SALLE where ((CAPACITE>=40)and(CAPACITE<=60))");
      query.exec();

      while(query.next())
          a2++;


      qDebug() << a1<< a2 ;

      QPieSeries *series = new QPieSeries();
          series->append("capacite entre 10 et 40", a1);
          series->append("capacite entre 40 et 60", a2);

          QPieSlice *slice = series->slices().at(0);
          slice->setExploded(true);
          slice->setColor("#f37b78");

          QPieSlice *slice2 = series->slices().at(1);
          slice2->setColor("#663333");


          QChart *chart = new QChart();
          chart->addSeries(series);
          chart->setTitle("Statistique du Capacite");


          series->setLabelsVisible();

          QChartView *chartView = new QChartView(chart);
          chartView->setRenderHint(QPainter::Antialiasing);
          chartView->chart()->setAnimationOptions(QChart::AllAnimations);
          chartView->chart()->legend()->hide();
          chartView->resize(1000,500);
          return chartView;
}
