#include "database.h"


database::database()
{

}
bool database::initialize()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","info_sql");
    db.setDatabaseName("medicinedata.db");
    if (!db.open())
    {
        cerr<<"databse create failed!"<<endl;
        return false;
    }

    QSqlQuery query(db);
    bool success = query.exec("create table if not exists infolist(name varchar,age varchar,sexuality varchar,"
                              "height varchar,weight varchar,BMI varchar,medicinetype varchar,cardid varchar,complain varchar,diagnosis varchar)");
    if (!success)
    {
        cerr<<"create table failed!"<<endl;
        qDebug()<<query.lastError()<<endl;
        return false;
    }

    db.close();
    return true;
}


bool database::query_infoitem(QString cardid)
{

    QSqlDatabase db = QSqlDatabase::database("info_sql");
    db.setDatabaseName("medicinedata.db");
    if (!db.open())
    {
        cerr<<"open failed!"<<endl;
        return false;
    }
    QSqlQuery query(db);

    query.prepare("select name, age, sexuality, height, weight, BMI, medicinetype, complain, diagnosis from infolist where cardid = :cardid");
    query.bindValue(":cardid",QString::fromLocal8Bit(cardid.toLocal8Bit().data()));

    bool success = query.exec();
    if (!success || !query.first())
    {

        return false;
    }

    db.close();
    return true;

}

bool database::insert_infoitem(infoitem item)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","info_sql");
    db.setDatabaseName("medicinedata.db");
    if (!db.open())
    {
        cerr<<"open failed!"<<endl;
        return false;
    }
    QSqlQuery query(db);
    query.prepare("insert into infolist (name, age, sexuality, height, weight, BMI, medicinetype, cardid, complain, diagnosis)"
                  " values (:name, :age, :sexuality, :height, :weight, :BMI, :medicinetype, :cardid, :complain, :diagnosis)");

    query.bindValue(":name",QString::fromLocal8Bit(item.name.toLocal8Bit().data()));

    query.bindValue(":age",QString::fromLocal8Bit(item.age.toLocal8Bit().data()));

    query.bindValue(":sexuality",QString::fromLocal8Bit(item.sexuality.toLocal8Bit().data()));

    query.bindValue(":height",QString::fromLocal8Bit(item.height.toLocal8Bit().data()));

    query.bindValue(":weight",QString::fromLocal8Bit(item.weight.toLocal8Bit().data()));

    query.bindValue(":BMI",QString::fromLocal8Bit(item.BMI.toLocal8Bit().data()));

    query.bindValue(":medicinetype",QString::fromLocal8Bit(item.medicinetype.toLocal8Bit().data()));

    query.bindValue(":cardid",QString::fromLocal8Bit(item.cardid.toLocal8Bit().data()));

    query.bindValue(":complain",QString::fromLocal8Bit(item.complain.toLocal8Bit().data()));

    query.bindValue(":diagnosis",QString::fromLocal8Bit(item.diagnosis.toLocal8Bit().data()));

    bool success = query.exec();
    if (!success)
    {
        cerr<<"insert failed!"<<endl;
        qDebug()<<query.lastError()<<endl;
    }


    db.close();
    return true;
}

bool database::update_infoitem(infoitem item)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","info_sql");
    db.setDatabaseName("medicinedata.db");
    if (!db.open())
    {
        cerr<<"open failed!"<<endl;
        return false;
    }
    QSqlQuery query(db);



    query.prepare("update infolist set name = ?, age = ?, sexuality = ?, height = ?, weight = ?, BMI = ?, medicinetype = ?, "
                  " complain = ?, diagnosis = ? where cardid = ?");
    query.bindValue(0,QString::fromLocal8Bit(item.name.toLocal8Bit().data()));
    query.bindValue(1,QString::fromLocal8Bit(item.age.toLocal8Bit().data()));
    query.bindValue(2,QString::fromLocal8Bit(item.sexuality.toLocal8Bit().data()));
    query.bindValue(3,QString::fromLocal8Bit(item.height.toLocal8Bit().data()));
    query.bindValue(4,QString::fromLocal8Bit(item.weight.toLocal8Bit().data()));
    query.bindValue(5,QString::fromLocal8Bit(item.BMI.toLocal8Bit().data()));
    query.bindValue(6,QString::fromLocal8Bit(item.medicinetype.toLocal8Bit().data()));
    query.bindValue(7,QString::fromLocal8Bit(item.complain.toLocal8Bit().data()));
    query.bindValue(8,QString::fromLocal8Bit(item.diagnosis.toLocal8Bit().data()));
    query.bindValue(9,QString::fromLocal8Bit(item.cardid.toLocal8Bit().data()));

    bool success = query.exec();
    if (!success)
    {
        cerr<<"update failed!"<<endl;
        qDebug()<<query.lastError()<<endl;
        return false;
    }

    db.close();
    return true;
}

























