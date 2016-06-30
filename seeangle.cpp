#include "seeangle.h"
#include "ui_seeangle.h"

static QCheckBox* boxset[22];
static double showangleplot[22][30];
static QPen* temppen[22];
static QString nameset[22]={QString::fromLocal8Bit("Ï¥ÇüÇú×ó") , QString::fromLocal8Bit("Ï¥ÇüÇúÓÒ") , QString::fromLocal8Bit("Ï¥ÉìÖ±×ó") , QString::fromLocal8Bit("Ï¥ÉìÖ±ÓÒ") , QString::fromLocal8Bit("Ï¥¹ÉëÖ½Ç×ó") ,
                    QString::fromLocal8Bit("Ï¥¹ÉëÖ½ÇÓÒ") , QString::fromLocal8Bit("Ï¥¼ä¾à×ó") , QString::fromLocal8Bit("Ï¥¼ä¾àÓÒ") , QString::fromLocal8Bit("õ×¼ä¾à×ó") , QString::fromLocal8Bit("õ×¼ä¾àÓÒ") ,
                    QString::fromLocal8Bit("÷ÅÇüÇú×ó") , QString::fromLocal8Bit("÷ÅÇüÇúÓÒ") , QString::fromLocal8Bit("÷ÅÉìÖ±×ó") , QString::fromLocal8Bit("÷ÅÉìÖ±ÓÒ") , QString::fromLocal8Bit("÷ÅÍâÕ¹×ó") ,
                    QString::fromLocal8Bit("÷ÅÍâÕ¹ÓÒ") , QString::fromLocal8Bit("÷ÅÄÚÊÕ×ó") , QString::fromLocal8Bit("÷ÅÄÚÊÕÓÒ") , QString::fromLocal8Bit("÷ÅÄÚĞı×ó") , QString::fromLocal8Bit("÷ÅÄÚĞıÓÒ") ,
                    QString::fromLocal8Bit("÷ÅÍâĞı×ó") , QString::fromLocal8Bit("÷ÅÍâĞıÓÒ")};

void makepen()
{

    temppen[0] = new QPen(QColor(255,0,0),2,Qt::SolidLine);
    temppen[1] = new QPen(QColor(255,0,0),2,Qt::DotLine);
    temppen[2] = new QPen(QColor(0,255,0),2,Qt::SolidLine);
    temppen[3] = new QPen(QColor(0,255,0),2,Qt::DotLine);
    temppen[4] = new QPen(QColor(0,0,255),2,Qt::SolidLine);
    temppen[5] = new QPen(QColor(0,0,255),2,Qt::DotLine);
    temppen[6] = new QPen(QColor(200,200,0),2,Qt::SolidLine);
    temppen[7] = new QPen(QColor(200,200,0),2,Qt::DotLine);
    temppen[8] = new QPen(QColor(255,0,255),2,Qt::SolidLine);
    temppen[9] = new QPen(QColor(255,0,255),2,Qt::DotLine);
    temppen[10] = new QPen(QColor(0,255,255),2,Qt::SolidLine);
    temppen[11] = new QPen(QColor(0,255,255),2,Qt::DotLine);
    temppen[12] = new QPen(QColor(0,0,0),2,Qt::SolidLine);
    temppen[13] = new QPen(QColor(0,0,0),2,Qt::DotLine);
    temppen[14] = new QPen(QColor(255,128,0),2,Qt::SolidLine);
    temppen[15] = new QPen(QColor(255,128,0),2,Qt::DotLine);
    temppen[16] = new QPen(QColor(0,128,255),2,Qt::SolidLine);
    temppen[17] = new QPen(QColor(0,128,255),2,Qt::DotLine);
    temppen[18] = new QPen(QColor(128,128,128),2,Qt::SolidLine);
    temppen[19] = new QPen(QColor(128,128,128),2,Qt::DotLine);
    temppen[20] = new QPen(QColor(239,143,189),2,Qt::SolidLine);
    temppen[21] = new QPen(QColor(239,143,189),2,Qt::DotLine);
}

seeangle::seeangle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::seeangle)
{
    Time_cycle=0;
    for (int i=0;i<30;i++)
    {
        xplot[i]=i;
    }
    ui->setupUi(this);

    for (int i=0;i<22;i++)
    {
        for (int j=0;j<30;j++)
        {
            showangleplot[i][j] = 0;
        }
    }
    makepen();
    QwtPlotCanvas *canvas=new QwtPlotCanvas();
    canvas->setPalette(Qt::white);
    canvas->setBorderRadius(10);
    ui->plot->setCanvas( canvas );
    ui->plot->setAxisScale(QwtPlot::xBottom,0,30);
    ui->plot->setAxisScale(QwtPlot::yLeft,0,200);
    boxset[0] = ui->c0;boxset[1] = ui->c1;boxset[2] = ui->c2;boxset[3] = ui->c3;boxset[4] = ui->c4;boxset[5] = ui->c5;boxset[6] = ui->c6;
    boxset[7] = ui->c7;boxset[8] = ui->c8;boxset[9] = ui->c9;boxset[10] = ui->c10;boxset[11] = ui->c11;boxset[12] = ui->c12;boxset[13] = ui->c13;
    boxset[14] = ui->c14;boxset[15] = ui->c15;boxset[16] = ui->c16;boxset[17] = ui->c17;boxset[18] = ui->c18;boxset[19] = ui->c19;
    boxset[20] = ui->c20;boxset[21] = ui->c21;


}

seeangle::~seeangle()
{
    delete ui;
}

void seeangle::on_ok_button_clicked()
{
    if(ui->c0->isChecked()) saveangle[0]=ui->c0->text();
    if(ui->c1->isChecked()) saveangle[1]=ui->c1->text();
    if(ui->c2->isChecked()) saveangle[2]=ui->c2->text();
    if(ui->c3->isChecked()) saveangle[3]=ui->c3->text();
    if(ui->c4->isChecked()) saveangle[4]=ui->c4->text();
    if(ui->c5->isChecked()) saveangle[5]=ui->c5->text();
    if(ui->c6->isChecked()) saveangle[6]=ui->c6->text();
    if(ui->c7->isChecked()) saveangle[7]=ui->c7->text();
    if(ui->c8->isChecked()) saveangle[8]=ui->c8->text();
    if(ui->c9->isChecked()) saveangle[9]=ui->c9->text();
    if(ui->c10->isChecked()) saveangle[10]=ui->c10->text();
    if(ui->c11->isChecked()) saveangle[11]=ui->c11->text();
    if(ui->c12->isChecked()) saveangle[12]=ui->c12->text();
    if(ui->c13->isChecked()) saveangle[13]=ui->c13->text();
    if(ui->c14->isChecked()) saveangle[14]=ui->c14->text();
    if(ui->c15->isChecked()) saveangle[15]=ui->c15->text();
    if(ui->c16->isChecked()) saveangle[16]=ui->c16->text();
    if(ui->c17->isChecked()) saveangle[17]=ui->c17->text();
    if(ui->c18->isChecked()) saveangle[18]=ui->c18->text();
    if(ui->c19->isChecked()) saveangle[19]=ui->c19->text();
    if(ui->c20->isChecked()) saveangle[20]=ui->c20->text();
    if(ui->c21->isChecked()) saveangle[21]=ui->c21->text();
    emit s_end();
    this->close();
}

void seeangle::on_cancel_button_clicked()
{
    emit s_end();
    this->close();
}

void seeangle::r_angles(float *angles)
{
    for (int i=0;i<22;i++)
    {
        if (angles[i]< -180 || angles[i] > 180) angles[i] = 0;
    }

    refresh_angle(angles);


}


void seeangle::refresh_one_angle(float *angles, int ptr)
{
//    double temp = boxset[ptr]->text().toDouble();
//    boxset[ptr]->setText(QString::number(angles[ptr],'f',1));
    if (Time_cycle == 0)
    {

        boxset[ptr]->setText(QString::number(angles[ptr],'f',1));
        for (int i = 0; i<29; i++)
        {
            showangleplot[ptr][i] = showangleplot[ptr][i+1];
        }

        showangleplot[ptr][29] = angles[ptr];
        QwtPlotCurve* curve = new QwtPlotCurve(nameset[ptr]);
        curve->setPen(*temppen[ptr]);
        curve->setSamples(xplot,showangleplot[ptr],30);
        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        ui->plot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
        curve->attach(ui->plot);
    }

}

void seeangle::refresh_angle(float *angles)
{

    Time_cycle++;
    Time_cycle%=30;

    if (Time_cycle == 0) ui->plot->detachItems();

    for (int i=0; i<22; i++)
    {
        if (boxset[i]->isChecked())
        {
            refresh_one_angle(angles, i);
        }
    }

    if (Time_cycle == 0) ui->plot->replot();

}
