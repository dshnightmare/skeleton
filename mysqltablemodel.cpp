#include "mysqltablemodel.h"

MySqlTableModel::MySqlTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

//QVariant MySqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    // FIXME: Implement me!
//    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
//        return section + m_nStart;
//    return QSqlTableModel::headerData(section, orientation, role);
//}

//int MySqlQueryModel::rowCount(const QModelIndex &parent) const
//{
//    if (parent.isValid())
//        return 0;

//    // FIXME: Implement me!
//}

//int MySqlQueryModel::columnCount(const QModelIndex &parent) const
//{
//    if (parent.isValid())
//        return 0;

//    // FIXME: Implement me!
//}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const

{
    if (index.column() > Database::Info_Date)
        return QSqlQueryModel::data(index, role);
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else
        return QSqlTableModel::data(index, role);
}
