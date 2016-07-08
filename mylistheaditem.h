#ifndef MYLISTHEADITEM_H
#define MYLISTHEADITEM_H

#include <QWidget>

namespace Ui {
class MyListHeadItem;
}

class MyListHeadItem : public QWidget
{
    Q_OBJECT

public:
    explicit MyListHeadItem(QWidget *parent = 0);
    ~MyListHeadItem();

private:
    Ui::MyListHeadItem *ui;
};

#endif // MYLISTHEADITEM_H
