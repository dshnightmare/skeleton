#include "infodetailview.h"
#include "ui_infodetailview.h"

InfoDetailView::InfoDetailView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDetailView)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("±¨¸æµ¥"));
}

InfoDetailView::~InfoDetailView()
{
    delete ui;
}


void InfoDetailView::setDate(QString date){
    ui->date->setText(date);
}

void InfoDetailView::setName(QString name){
    ui->name->setText(name);
}

void InfoDetailView::setAge(QString age){
    ui->age->setText(age);
}

void InfoDetailView::setSex(QString sex){
    ui->sex->setText(sex);
}

void InfoDetailView::setCardid(QString cardid){
    ui->cardid->setText(cardid);
}


void InfoDetailView::setComplain(QString complain){
    ui->complain->setText(complain);
}

void InfoDetailView::setScore(QString score){
    ui->score->setText(score);
}

void InfoDetailView::setDiagnose(QString diagnose){
    ui->diagnose->setText(diagnose);
}

void InfoDetailView::setPicture1(QString src){
    ui->picture1->setText("");
}

void InfoDetailView::setPicture2(QString src){
    ui->picture2->setText("");
}
