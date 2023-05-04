#include "ges_salles.h"
#include "ui_ges_salles.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#define file_tx "^[A-Za-z ]+$"
#define file_ex "^[0-9]+$"
#include <QSqlRecord>
#include "qrcodegen.hpp"
#include <QtSql/QSqlDatabase>
#include <arduino.h>
#include <QSqlError>

void ges_salles::update_label()
{
    QByteArray data = serialPort.readAll();
    QString dataString(data);

}
ges_salles::ges_salles(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ges_salles)
{



    {

    //*************Arduino*******************************************************************
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
        break;
    case(-1):qDebug() << "arduino is not available";
    }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
    //le slot update_label suite à la reception du signal readyRead (reception des données).
    }




    ui->setupUi(this);
    ui->categorie_ajout->setValidator(new QRegExpValidator( QRegExp(file_tx),this));
    ui->capacite_ajout->setValidator(new QRegExpValidator( QRegExp(file_ex),this));
    ui->numero_ajout->setValidator(new QRegExpValidator( QRegExp(file_ex),this));
    ui->temps_ajout->setValidator(new QRegExpValidator( QRegExp(file_ex),this));
    ui->surface_ajout->setValidator(new QRegExpValidator( QRegExp(file_ex),this));

}


void ges_salles::readSerialData()
{
    connect(this, SIGNAL(readSerialData(QString)), this, SLOT(update_label(QString)));
    QByteArray data = A.read_from_arduino();
    QString dataString(data);

    // Split the data into the RFID, access status, and room number strings
    QStringList dataList = dataString.split(",");
    QString rfid = dataList.at(0).split(":").at(1).trimmed();
    QString accessStatus = dataList.at(1).split(":").at(1).trimmed();
    QString roomNumber = dataList.at(2).split(":").at(1).trimmed();

    // Insert the data into the database
    // TODO: Implement database insertion code here
    QSqlQuery query;
    query.prepare("INSERT INTO RFID (RFID, ACCES, ROOMN) VALUES (:rfid, :accessStatus, :roomNumber)");
    query.bindValue(":rfid", rfid);
    query.bindValue(":accessStatus", accessStatus);
    query.bindValue(":roomnumber", roomNumber);
    bool success = query.exec();

    if (!success) {
    qDebug() << "Failed to insert data into database:";
    }


    {
    // QByteArray data = serialPort.readAll();
     //QString dataString(data);

    QByteArray data =A.read_from_arduino();
    }
}



ges_salles::~ges_salles()
{
    delete ui;
}
bool ges_salles::controlSaisie(){ ;
//controole se saisie sur lesq champs d ajout
    if (



            ui->categorie_ajout->text().isEmpty() ||
            ui->disponible_ajout->text() =="" ||

            ui->capacite_ajout->text().isEmpty() ||
            ui->capacite_ajout->text().toInt() ==0||

            ui->numero_ajout->text().isEmpty() ||
            ui->numero_ajout->text().toInt() == 0 ||

            ui->temps_ajout->text().isEmpty() ||
            ui->temps_ajout->text().toInt() == 0 ||

            ui->surface_ajout->text().isEmpty() ||
            ui->surface_ajout->text().toFloat() == 0

           )

        return 0;

    else

        return 1;
                                };
void ges_salles::CLEAR()
{//vider les champs
    ui->numero_ajout->clear();
    ui->categorie_ajout->clear();
    ui->disponible_ajout->clear();
    ui->temps_ajout->clear();
    ui->surface_ajout->clear();
    ui->capacite_ajout->clear();
}
//fonction ajout
void ges_salles::on_bouton_ajouter_clicked()
{
    QMessageBox msgBox;
        if(controlSaisie()==1)//verfication de controle de saisie
    {

            bool test1=ac.employexiste(ui->numero_ajout->text());//verifaction de l existance de l employe
            if(test1==true)
    {            msgBox.setText("!!ID existe !!");//afficher existe
                msgBox.exec();}

            else{//si il n existe pas alors on recupere les donnes du champs
    int NUMERO=ui->numero_ajout->text().toInt();
    QString CATEGORIE=ui->categorie_ajout->text();
    QString DISPONIBILITE=ui->disponible_ajout->text();
    int CAPACITE=ui->capacite_ajout->text().toInt();
    int TEMPS=ui->temps_ajout->text().toInt();
    int SURFACE=ui->surface_ajout->text().toInt();

    salle acc(NUMERO,CATEGORIE,DISPONIBILITE,SURFACE,CAPACITE,TEMPS);

    bool test=acc.ADD();//ajout de l employe avec verfication de l ajout

     if(test==true)
     {
     CLEAR();
     msgBox.setText("Ajout avec succes.");//si l ajout ppasse bien affichage du texte
     }
     else
       {msgBox.setText("!!Echec d'ajout!!");}//sinon affichage de texte erreur


        msgBox.exec();   }}

    else if(controlSaisie()==0)//s il ya erreue de controle de saise un champ vide exemple alors on affice l erreur
    {
    QMessageBox::critical(nullptr, QObject::tr("not ok"),
    QObject::tr("veuiller remplir tous les champs.\n !!!Il existe des champs VIDE ou INCORRECT!!!\n"
    "Click cancel to exit."), QMessageBox::Cancel);
    }

        ui->table_salle->setModel(ac.GETALL());//mise ajout du table view pour l affiche de nouvel salle
}


//fonction modifer
void ges_salles::on_pb_modifier_clicked()
{
    if(controlSaisie()==1)//verification controle de saisie
    {
        int NUMERO=ui->numero_ajout->text().toInt();//recupperation des donnes depuis les champs
        QString CATEGORIE=ui->categorie_ajout->text();
        QString DISPONIBILITE=ui->disponible_ajout->text();
        int CAPACITE=ui->capacite_ajout->text().toInt();
        int TEMPS=ui->temps_ajout->text().toInt();
        int SURFACE=ui->surface_ajout->text().toInt();

        salle acc(NUMERO,CATEGORIE,DISPONIBILITE,SURFACE,CAPACITE,TEMPS);

bool test1=ac.employexiste(ui->numero_ajout->text());//veriication de l existance de l employe
            if(test1==true)// si existe
 {
    bool test=acc.UPDATE();//mise a jour de labse avec la commande update
            QMessageBox msgBox;
            if(test)//si l update passe bien affichage du text
            {ui->table_salle->setModel(ac.GETALL());
                 CLEAR();
                msgBox.setText("modification avec succes.");
                msgBox.exec(); }

            else
            {//sinon  affichage du text erreur
            msgBox.setText("!!Echec de modification!!");
            msgBox.exec();
             }
    }

            else
            {//s il n existe pas affichage de l erreur
                QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                 QObject::tr("salles n'existe pas pour modifier !\n"
                 "Click Cancel to exit."), QMessageBox::Cancel);

            }


    }
  else if(controlSaisie()==0)//s il ya erreue de controle de saise un champ vide exemple alors on affice l erreur
  {
       // qDebug() <<"veuiller remplir tous les champs!!! Il existe des champs VIDE ou INCORRECT!!!\n";
       QMessageBox::critical(nullptr, QObject::tr("not ok"),

                          QObject::tr("veuiller remplir tous les champs.\n !!!Il existe des champs VIDE ou INCORRECT!!!\n"

                                      "Click cancel to EXIT."), QMessageBox::Cancel);
    }


}

void ges_salles::on_tabWidget_currentChanged(int)//fonction pour l affichge des donne de labes sur le tableaux
{ui->table_salle->setModel(ac.GETALL());

}

void ges_salles::on_table_salle_activated(const QModelIndex &index)//s il click sur un champ de la table alors on prepere la ligne a etre modifier dans l ajout
{
    CLEAR();
        QString val=ui->table_salle->model()->data(index).toString();
        QSqlQuery qry;
        qry.prepare("select * from GES_SALLE where NUMERO='"+val+"'");//selection de la ligne dans la base qui contient le numero introduit
        if(qry.exec())
        {
         while(qry.next())
         {


          ui->numero_ajout->setText(qry.value(0).toString());//affichage du valeur recuperer
          ui->categorie_ajout->setText(qry.value(1).toString());
          ui->disponible_ajout->setText(qry.value(2).toString());
          ui->temps_ajout->setText(qry.value(3).toString());
          ui->surface_ajout->setText(qry.value(4).toString());
          ui->capacite_ajout->setText(qry.value(5).toString());




         }
         using namespace qrcodegen;

         // FETCH DATA FROM DATABASE
         QTableView tab_salle;
         QSqlQueryModel * Mod = new QSqlQueryModel();

         QSqlQuery qry;
         qry.prepare("select * from GES_SALLE where NUMERO='"+val+"'");
         qry.exec();
         Mod->setQuery(qry);
         tab_salle.setModel(Mod);

         // SAVING DATA
         int NUMERO=tab_salle.model()->data(tab_salle.model()->index(0, 0)).toInt();
         QString CATEGORIE = tab_salle.model()->data(tab_salle.model()->index(0, 1)).toString().simplified();;
         QString DISPONIBILITE = tab_salle.model()->data(tab_salle.model()->index(0, 2)).toString().simplified();;
         int TEMPS= tab_salle.model()->data(tab_salle.model()->index(0,3)).toInt();
         int SURFACE= tab_salle.model()->data(tab_salle.model()->index(0, 4)).toInt();
         int CAPACITE= tab_salle.model()->data(tab_salle.model()->index(0,5)).toInt();

         QString text = "Number: " + QString::number(NUMERO) + "\n"
                        "Category: " + CATEGORIE + "\n"
                        "Availability: " + DISPONIBILITE + "\n"
                        "Capacity: " + QString::number(CAPACITE) + "\n"
                        "Time: " + QString::number(TEMPS) + "\n"
                        "Surface: " + QString::number(SURFACE) + "\n";
         QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

         qint32 sz = qr.getSize();
         QImage im(sz, sz, QImage::Format_RGB32);
         QRgb black = qRgb(191, 112, 105);
         QRgb white = qRgb(255, 255, 255);

         for (int y = 0; y < sz; y++)
         {
             for (int x = 0; x < sz; x++)
             {
                 im.setPixel(x, y, qr.getModule(x, y) ? black : white);
             }
         }
          ui->qrcode->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation), Qt::MonoOnly));

         int i = 0;
         for (i = 0; i < 100; i = i + 0.001)
         {
             i++;
             ui->progressBar->setValue(i);
         }




        }
        else
        {
      qDebug()<<"erreur";
        }

}
//supprimer salles
void ges_salles::on_pb_supprimer_clicked()
{
    salle acc;
    acc.set_NUMERO(ui->id_supp->text().toInt());
    bool test1=acc.employexiste(ui->id_supp->text());//verification de l existance



                if(test1==true)//s il existe on le supprime
     {
       bool test=acc.DELETEE(acc.get_NUMERO());
       QMessageBox msgBox;
       if(test)
       {
      msgBox.setText("Suppression avec succes.");
        ui->table_salle->setModel(acc.GETALL());
      CLEAR();
       }
      else
       msgBox.setText("!!Echec de suppression!!");
      msgBox.exec();

     }
                else
                {
                    //qDebug() <<"!!!salle n'existe pas !!\n";
                   QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                QObject::tr("salle n'existe pas !\n"
                                            "salle Cancel to exit."), QMessageBox::Cancel);

                }
}

void ges_salles::on_radioButton_a_clicked()
{
    CLEAR();
          salle c;

  if(ui->comboBox_trie->currentText() != "Choisir")
ui->table_salle->setModel(c.trie("ASC",ui->comboBox_trie->currentText()));
else  ui->table_salle->setModel(c.GETALL());
}

void ges_salles::on_radioButton_d_clicked()
{    CLEAR();
     salle c;


if(ui->comboBox_trie->currentText() != "Choisir")
ui->table_salle->setModel(c.trie("DESC",ui->comboBox_trie->currentText()));
else  ui->table_salle->setModel(c.GETALL());

}

void ges_salles::on_pb_meilleur_clicked()
{QSqlQueryModel *model = ac.FINDBYNUMERO(ui->rechercher->text().toInt());
    if (model->rowCount() > 0) {
        // Get the record of the first row
          QSqlRecord record = model->record(0);
          ui->numero_ajout->setText(record.value(0).toString());//affichage du valeur recuperer
          ui->categorie_ajout->setText(record.value(1).toString());
          ui->disponible_ajout->setText(record.value(2).toString());
          ui->temps_ajout->setText(record.value(3).toString());
          ui->surface_ajout->setText(record.value(4).toString());
          ui->capacite_ajout->setText(record.value(5).toString());;
    }else
    {QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                    QObject::tr("numero du la salle n'existe pas !\n"
                                "numero du la salle Cancel to exit."), QMessageBox::Cancel);

    }

}

void ges_salles::on_pb_supprimer_2_clicked()
{
    QChartView *chartView = ac.stat_capacite();
        chartView->show();
}

void ges_salles::on_pb_supprimer_3_clicked()
{
        QPdfWriter pdf("C:/Users/maste/Desktop/ekher haja/Liste.pdf");

        QPainter painter(&pdf);

        int i = 4000;
        painter.setPen(Qt::black);
        painter.setFont(QFont("PreferVerticalHinting", 30));
        painter.drawPixmap(QRect(100,400,2000,2000),QPixmap("C:/Users/maste/Pictures/Screenshots/Screenshot 2023-04-10 124728.png"));
        painter.drawText(3000,1500,"LISTE DES SALLES");
        painter.setPen(Qt::black);
        painter.setFont(QFont("PreferVerticalHinting", 10));
        painter.setPen(Qt::black);
        painter.setFont(QFont("PreferVerticalHinting", 10));
        painter.drawText(300,3300,"NUMERO");
        painter.drawText(1800,3300,"CATEGORIE");
        painter.drawText(3300,3300,"DISPONIBILITE");
        painter.drawText(4800,3300,"CAPACITE");
        painter.drawText(5300,3300,"TEMPS");
        painter.drawText(6800,3300,"SURFACE");
        painter.drawText(8300,3300,"CODEACTIVITE");
        QSqlQuery query;
        query.prepare("select * from GES_SALLE");
        query.exec();
        while (query.next())
        {
          painter.drawText(300,i,query.value(0).toString());
          painter.drawText(1800,i,query.value(1).toString());
          painter.drawText(3300,i,query.value(2).toString());
          painter.drawText(4800,i,query.value(3).toString());
          painter.drawText(5300,i,query.value(4).toString());
          painter.drawText(6800,i,query.value(5).toString());
          painter.drawText(8300,i,query.value(6).toString());
          i = i +500;
        }

        int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                                                         if (reponse == QMessageBox::Yes)
                                                         {
                                                             QDesktopServices::openUrl(QUrl::fromLocalFile("C:/Users/maste/Desktop/ekher haja/Liste.pdf"));

                                                             painter.end();
                                                         }
                                                         if (reponse == QMessageBox::No)
                                                         {
                                                             painter.end();
                                                         }
}


void ges_salles::on_pushButton_clicked()
{
                                                         QProcess *process = new QProcess(this);
                                                         process->setProgram("C:/Users/maste/Desktop/integration rrrrrrrrrrrrrrtr/build-mainwindowproject-Desktop_Qt_5_15_2_MinGW_32_bit-Debug/debug/mainwindowproject.exe");
                                                         process->start();
}

