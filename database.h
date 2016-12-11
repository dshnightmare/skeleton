#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QStringList>

using namespace std;

class infoitem{
public:
    QString name;
    QString age;
    QString sex;
    QString height;
    QString weight;
    QString BMI;
    QString medicinetype;
    QString cardid;
    QString complain;
    QString score;
    QString diagnosis;
    bool exist;
};

class Database
{
public:
    enum {Info_Id, Info_Name, Info_Age, Info_Sex, Info_Height, Info_Weight, Info_BMI,
          Info_Medicinetype, Info_Cardid, Info_Complain, Info_Score, Info_Diagnosis, Info_Date};
    static Database* getInstance(){
        if (NULL != pInstance)
            return pInstance;
        pInstance = new Database;
        pInstance->db = QSqlDatabase::addDatabase("QSQLITE","info_sql");
        pInstance->db.setDatabaseName("medicinedata.db");
        if (!pInstance->db.open())
        {
            cerr<<"databse create failed!"<<endl;
            delete pInstance;
            pInstance = NULL;
            return pInstance;
        }

        QSqlQuery query(pInstance->db);
        bool success = query.exec("create table if not exists infolist(id integer primary key asc, name text not null, age integer, sex text, "
                                  "height real, weight real, BMI real, medicinetype text, cardid text, complain text not null, score integer, "
                                  "diagnosis text not null, date timestamp not null default (datetime('now', 'localtime')))");
        if (!success)
        {
            cerr<<"create table failed!"<<endl;
            qDebug()<<query.lastError()<<endl;
            delete pInstance;
            pInstance = NULL;
        }
        return pInstance;
    }
    bool insert_infoitem(infoitem item);
    bool update_infoitem(infoitem item);
    bool query_infoitem(QString cardid);

    QSqlDatabase db;
private:
    Database();
    ~Database();
    static Database* pInstance;
};

#endif // DATABASE_H
