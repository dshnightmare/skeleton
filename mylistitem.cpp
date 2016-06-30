#include "mylistitem.h"
#include "ui_mylistitem.h"


MyListItem::MyListItem(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::MyListItem)
{
    ui->setupUi(this);
}

MyListItem::~MyListItem()
{
    delete ui;
}

QString MyListItem::getName(){
    return ui->name->text();
}

void MyListItem::initItem(QString name, QListWidget *list, QListWidgetItem *item){
    ui->name->setText(name);
    ui->angle->setPalette(Qt::black);
    ui->angle->display(0);
    //ui->legend->setStyleSheet("background-color:rgb(255, 0, 0)");
    connect(ui->del, SIGNAL(clicked(bool)), this, SLOT(deleteItem()));
    plist = list;
    pitem = item;
}

void MyListItem::setAngle(double angle){
    ui->angle->display(angle);
}

void MyListItem::deleteItem(){
    emit deleteListItem(pitem);
}
