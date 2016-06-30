#ifndef PARSERMODEL_H
#define PARSERMODEL_H

#include <QAbstractListModel>

class parserModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit parserModel(const QStringList &strings, QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    QStringList stringList;
};

#endif // PARSERMODEL_H
