#ifndef MYLISTITEM_H
#define MYLISTITEM_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
class MyListItem;
}

class MyListItem : public QWidget
{
    Q_OBJECT

public:
    explicit MyListItem(QWidget *parent = 0, Qt::WindowFlags f=0);
    ~MyListItem();
    void initItem(QString name, QListWidget *list, QListWidgetItem *item);
    void setAngle(int angle);
    QString getName();

private:
    Ui::MyListItem *ui;
    QListWidget *plist;
    QListWidgetItem *pitem;

public slots:
    void deleteItem();
signals:
    void deleteListItem(QListWidgetItem*);
};

#endif // MYLISTITEM_H
