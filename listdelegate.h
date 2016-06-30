#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>

class listDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit listDelegate(QObject *parent = 0);

    //
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // LISTDELEGATE_H
