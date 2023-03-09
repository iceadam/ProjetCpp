#ifndef GES_SALLES_H
#define GES_SALLES_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class ges_salles; }
QT_END_NAMESPACE

class ges_salles : public QMainWindow
{
    Q_OBJECT

public:
    ges_salles(QWidget *parent = nullptr);
    ~ges_salles();

private slots:


private:
    Ui::ges_salles *ui;

};
#endif // GES_SALLES_H
