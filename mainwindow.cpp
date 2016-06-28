#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "Kinect.h"
#include "QDateTime"
#include "QDir"
#include "QtConcurrent/QtConcurrent"
#include "QMutex"
#include "QTextCodec"

static QMutex flagLock;
static QMutex flagLock1;
static QMutex testlock;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    w1=NULL;
    w2=NULL;
    takeflag=Kinect_exit;
    seeangleflag=false;
    folder="data";
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QImage mat2qimage(cv::Mat img)
{
    QImage img1;
    if (img.channels()==3)
    {
        cvtColor(img,img,CV_BGR2RGB);
        img1 = QImage((const unsigned char*)(img.data),img.cols,img.rows,
                      img.cols*img.channels(),QImage::Format_RGB888);
    }
    else if(img.channels()==1)
    {
        QVector<QRgb>  colorTable;

            for(int k=0;k<256;++k)
            {

                   colorTable.push_back( qRgb(k,k,k) );

            }


        img1 = QImage((const unsigned char*)(img.data),img.cols,img.rows,
                    img.cols*img.channels(),QImage::Format_Indexed8);
         img1.setColorTable(colorTable);
    }
    else
    {
        img1 = QImage((const unsigned char *)(img.data), img.cols, img.rows,
                        img.cols*img.channels(), QImage::Format_RGB888);
    }
    return img1;
}

QString MainWindow::makedir()
{
    QString tfolder;
    bool mkdirsuccess;
    QDir* dir = new QDir;

    if (!dir->exists(folder))
    {
        mkdirsuccess=dir->mkdir(folder);
    }

    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    int msec=time.currentTime().msec();
    QString currentDate = dt.toString("yyyy-MM-dd-hh-mm-ss");
    currentDate=currentDate+"-"+QString::number(msec);
    tfolder=folder+"//"+currentDate;

    if (!dir->exists(tfolder))
    {
        mkdirsuccess=dir->mkdir(tfolder);
    }
    if (!dir->exists(tfolder+"//color"))
    {
        mkdirsuccess=dir->mkdir(tfolder+"//color");
    }
//    if (!dir->exists(tfolder+"//depth"))
//    {
//        mkdirsuccess=dir->mkdir(tfolder+"//depth");
//    }
    if (!dir->exists(tfolder+"//skeleton"))
    {
        mkdirsuccess=dir->mkdir(tfolder+"//skeleton");
    }
    if (!dir->exists(tfolder+"//2dskeleton"))
    {
        mkdirsuccess=dir->mkdir(tfolder+"//2dskeleton");
    }
    return tfolder;
}

void MainWindow::Kinectrun(QByteArray ba)
{
    char* folder = ba.data();
    CBodyBasics myKinect;
    HRESULT hr = myKinect.InitializeDefaultSensor(folder);
    QImage rgbimg,skeletonimg;
    if (SUCCEEDED(hr)){
        while (1){
            flagLock.lock();
            if (takeflag==Kinect_take)
            {
                flagLock.unlock();
                myKinect.Update();

//                QTime t;
//                t.start();
//                while(t.elapsed()<30)
//                QCoreApplication::processEvents();
                if(myKinect.readok==true)
                {
                    rgbimg=mat2qimage(myKinect.colorImg);
                    skeletonimg=mat2qimage(myKinect.skeletonImg);
                    ui->skeleton->setPixmap(QPixmap::fromImage(skeletonimg.scaled(ui->skeleton->width(),ui->skeleton->height(),Qt::KeepAspectRatio)));
                    ui->skeleton->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
                    ui->rgb->setPixmap(QPixmap::fromImage(rgbimg.scaled(ui->rgb->width(),ui->rgb->height(),Qt::KeepAspectRatio)));
                    ui->rgb->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

                    flagLock1.lock();
                    if (seeangleflag==true)
                    {
                        emit s_angles(myKinect.rec_angle_and_dis);
                    }
                    flagLock1.unlock();
                }

            }
            else if(takeflag==Kinect_pause)
            {
                flagLock.unlock();
                while(takeflag==Kinect_pause)
                {
                    QTime t;
                    t.start();
                    while(t.elapsed()<100)
                    QCoreApplication::processEvents();
                }

            }
            else if(takeflag==Kinect_exit)
            {
                flagLock.unlock();
                QTime t;
                t.start();
                while(t.elapsed()<1000)
                QCoreApplication::processEvents();
                break;
            }

        }
    }

}
void MainWindow::closeEvent(QCloseEvent *e)
{
    flagLock.lock();
    takeflag=Kinect_exit;
    flagLock.unlock();
    QTime t;
    t.start();
    if (w1!=NULL)
    {
        w1->close();
        delete w1;
    }
    if (w2!=NULL)
    {
        w2->close();
        delete w2;
    }

    while(t.elapsed()<1000)
    QCoreApplication::processEvents();
}

void MainWindow::on_start_clicked()
{
//    cv::Mat aa=cv::imread("y.jpg");
//    cv::imwrite("ss.jpg",aa);
    flagLock.lock();
    if (takeflag!=Kinect_exit)
    {
        flagLock.unlock();
        return;
    }
    takeflag=Kinect_take;
    flagLock.unlock();
    QString inputfolder=makedir();
    QByteArray ba = inputfolder.toLatin1();
    QtConcurrent::run(this,&MainWindow::Kinectrun,ba);
}

void MainWindow::on_pause_clicked()
{

    flagLock.lock();
    if (takeflag==Kinect_take)
    {
        takeflag=Kinect_pause;
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
        ui->pause->setText(QString::fromLocal8Bit("继续"));
    }
    else if (takeflag==Kinect_pause)
    {
        takeflag=Kinect_take;
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
        ui->pause->setText(QString::fromLocal8Bit("暂停"));
    }
    flagLock.unlock();

}

void MainWindow::on_stop_clicked()
{
    flagLock.lock();
    takeflag=Kinect_exit;
    flagLock.unlock();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->pause->setText(QString::fromLocal8Bit("暂停"));
    delete w1;
    delete w2;
    w1=NULL;
    w2=NULL;
}

void MainWindow::on_Qrcode_clicked()
{
    if (w1==NULL||w2==NULL) return;
    QString parsestr;
    parsestr=w1->name+'$'+w1->gender+'$'+w1->age+'$'+w1->height+'$'+w1->weight+'$'+w1->medicaltype
            +'$'+w1->diagonasis+'$'+w1->kneediagnosis+'$'+w1->hipdiagnosis+'$'+w1->otherdiagnosis+'$'
            +w1->side+'$'+w1->year+'$'+w1->month+'$'+w1->day;
    for (int i=0;i<22;i++)
    {
        parsestr=parsestr+'$';
        if (!w2->saveangle[i].isEmpty())
        {
            parsestr=parsestr+'$'+w2->saveangle[i];
        }

    }
    std::string tstr=parsestr.toStdString();
    QByteArray tempstring = parsestr.toLocal8Bit();
    std::string tt=tempstring.data();

    QRController* controller = new QRController(tempstring.constData(), eclMedium, 250, 250, evAuto);

    vector<char> tempBitmapArray;
    controller->SaveToByteArray(tempBitmapArray);
    QByteArray tempQArray(tempBitmapArray.data(), tempBitmapArray.size());

    QPixmap QRCodeBitmap;
    QRCodeBitmap.loadFromData(tempQArray, "BMP");
    ui->qrcode->setPixmap(QRCodeBitmap.scaled(ui->qrcode->width(),ui->qrcode->height(),Qt::KeepAspectRatio));

}

void MainWindow::on_fillinfo_clicked()
{
    if (w1==NULL)
    {
        w1=new fillinfo;
    }
    w1->setWindowTitle(QString::fromLocal8Bit("填写信息"));
    w1->setWindowFlags(Qt::WindowCloseButtonHint);
    w1->setGeometry(x()+100,y()+100,592,652);
    w1->setFixedWidth(592);
    w1->setFixedHeight(652);
    w1->show();
}

void MainWindow::on_seeangle_clicked()
{
    if (w2==NULL)
    {
        w2=new seeangle;
    }
    w2->setWindowTitle(QString::fromLocal8Bit("查看角度"));
    w2->setWindowFlags(Qt::WindowCloseButtonHint);
    w2->setGeometry(x()+100,y()+100,1151,350);
    w2->setFixedWidth(1151);
    w2->setFixedHeight(350);
    connect(this,SIGNAL(s_angles(float*)),w2,SLOT(r_angles(float*)));
    connect(w2,SIGNAL(s_end()),this,SLOT(r_end()));
    flagLock1.lock();
    seeangleflag=true;
    flagLock1.unlock();
    w2->show();
}

void MainWindow::r_end()
{
    flagLock1.lock();
    seeangleflag=false;
    flagLock1.unlock();
}
