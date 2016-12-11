#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlTableModel>
#include "database.h"

class MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit MySqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    // Header:
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    //int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // MYSQLQUERYMODEL_H
