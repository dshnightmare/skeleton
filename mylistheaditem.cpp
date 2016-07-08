#include "mylistheaditem.h"
#include "ui_mylistheaditem.h"

MyListHeadItem::MyListHeadItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyListHeadItem)
{
    ui->setupUi(this);
}

MyListHeadItem::~MyListHeadItem()
{
    delete ui;
}
