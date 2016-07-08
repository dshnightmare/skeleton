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
    QPalette pal = ui->angle->palette();
    pal.setColor(QPalette::Text, Qt::red);
    ui->angle->setPalette(pal);
    ui->angle->setText("000");
    //ui->legend->setStyleSheet("background-color:rgb(255, 0, 0)");
    ui->del->setFixedSize(ui->del->height(), ui->del->height());
    connect(ui->del, SIGNAL(clicked(bool)), this, SLOT(deleteItem()));
    plist = list;
    pitem = item;
}

void MyListItem::setAngle(int angle){
    //ui->angle->display(angle);
    QString s;
    s.sprintf("%03d", angle);
    ui->angle->setText(s);
}

void MyListItem::deleteItem(){
    emit deleteListItem(pitem);
}
