#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include "QStringList"
#include "QPoint"
#include "vector"

using namespace std;

class infoitem{
public:
    QString name;
    QString age;
    QString sexuality;
    QString height;
    QString weight;
    QString BMI;
    QString medicinetype;
    QString cardid;
    QString complain;
    QString diagnosis;
    bool exist;
};

class database
{
public:
    database();
    bool initialize();
    bool insert_infoitem(infoitem item);
    bool update_infoitem(infoitem item);
    bool query_infoitem(QString cardid);

};

#endif // DATABASE_H
