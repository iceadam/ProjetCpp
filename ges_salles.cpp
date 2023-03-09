#include "ges_salles.h"
#include "ui_ges_salles.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#define file_tx "^[A-Za-z ]+$"
#define file_ex "^[0-9]+$"
ges_salles::ges_salles(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ges_salles)
{
    ui->setupUi(this);


}

ges_salles::~ges_salles()
{
    delete ui;
}
