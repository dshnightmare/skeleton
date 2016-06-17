#ifndef FILLINFO_H
#define FILLINFO_H

#include <QDialog>
#include <QString>

namespace Ui {
class fillinfo;
}

class fillinfo : public QDialog
{
    Q_OBJECT

public:
    explicit fillinfo(QWidget *parent = 0);
    ~fillinfo();
    QString name;
    QString gender;
    QString age;
    QString height;
    QString weight;
    QString medicaltype;
    QString diagonasis;
    QString kneediagnosis;
    QString hipdiagnosis;
    QString otherdiagnosis;
    QString side;
    QString year;
    QString month;
    QString day;
    void fillkneediagnosis();
    void fillhipdiagnosis();
//    QString kneeleft1;
//    QString kneeright1;
//    QString kneeleft2;
//    QString kneeright2;
//    QString kneeleft3;
//    QString kneeright3;
//    QString kneeleft4;
//    QString kneeright4;
//    QString wrist1;

private slots:
    void on_cancel_button_clicked();

    void on_ok_button_clicked();

private:
    Ui::fillinfo *ui;
};

#endif // FILLINFO_H
