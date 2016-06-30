#include "listdelegate.h"

listDelegate::listDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

void listDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItemV4 opt = setOptions(index, option);

    //prepare
    painter->save();

    //get the data and the rectangle


    painter->dra
}

QSize listDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

}
