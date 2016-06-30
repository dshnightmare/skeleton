#include "parsermodel.h"

parserModel::parserModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int parserModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return stringList.count();
}

QVariant parserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= stringList.size())
        return QVariant();

    // FIXME: Implement me!
    if (role == Qt::UserRole){
        return stringList.at(index.row());
    }
    else if (role == Qt::UserRole + 1){
        //图例的颜色
    }
    return QVariant();
}
