#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Kinect.h"
#include "QDateTime"
#include "QDir"
#include "qbitmap.h"
#include "qlineedit.h"
#include "QtConcurrent/QtConcurrent"
#include "QMutex"
#include "QTextCodec"
#include "QValidator"
#include "mylistitem.h"
#include "boost/iostreams/stream.hpp"
#include "boost/iostreams/device/array.hpp"

static QMutex flagLock;
static QMutex testlock;


static const int x_div=10;
static const int y_div=10;
static const int z_div=10;
static const int radious_div=3;
static const float taken_rate=0.9f;

static const int LocateTimes=300;
static const int LocateFrames=400;
static const int FrameStart=50;
static const int FrameInterval=5;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    takeflag=Kinect_exit;
    folder="data";
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->select_side->addItem(QString::fromLocal8Bit("左"));
    ui->select_side->addItem(QString::fromLocal8Bit("右"));
    ui->select_side->setCurrentIndex(-1);
    ui->select_position->addItem(QString::fromLocal8Bit("膝"));
    ui->select_position->addItem(QString::fromLocal8Bit("踝"));
    ui->select_position->addItem(QString::fromLocal8Bit("髋"));
    ui->select_position->addItem(QString::fromLocal8Bit("肘"));
    ui->select_position->addItem(QString::fromLocal8Bit("肩"));

    ui->select_position->setCurrentIndex(-1);
    ui->centralWidget->setPalette(QColor(55, 55, 55));
    ui->display->setPalette(QColor(225, 225, 225));
    ui->StackedWidget->setPalette(QColor(242, 242, 242));
    ui->stacklist->setPalette(QColor(242, 242, 242));
    tabChoose = new QButtonGroup();
    tabChoose->addButton(ui->choose1, 0);
    tabChoose->addButton(ui->choose2, 1);
    tabChoose->addButton(ui->choose3, 2);
    tabChoose->setExclusive(true);
    connect(tabChoose, SIGNAL(buttonClicked(int)), ui->StackedWidget, SLOT(setCurrentIndex(int)));
    ui->choose1->setChecked(true);
    anglemanager = new AngleManager(ui->qwtPlot);
    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags);
    ui->selected_angles->addItem(item);

    MyListHeadItem* myItem = new MyListHeadItem();
    ui->selected_angles->setItemWidget(item, myItem);
    item->setSizeHint(QSize(myItem->rect().width(), myItem->rect().height()));
    infoPage();


    connect(this,SIGNAL(s_v1(int)),this,SLOT(r_v1(int)));
    connect(this,SIGNAL(s_v2(int)),this,SLOT(r_v2(int)));

    left_hip_div.x=0.0;
    left_hip_div.y=0.0;
    left_hip_div.z=0.0;

    right_hip_div.x=0.0;
    right_hip_div.y=0.0;
    right_hip_div.z=0.0;

    hip_score_left = -1;
    hip_score_right = -1;
    knee_score_left = -1;
    knee_score_right = -1;

    //database
    queryWidget = new QueryWidget;
    queryWidget->setWindowTitle(QString::fromLocal8Bit("查询数据库"));

    qRegisterMetaType<bodyangle>("bodyangle");
}

MainWindow::~MainWindow()
{
    if(NULL != queryWidget)
        delete queryWidget;
    delete tabChoose;
    delete anglemanager;
    delete ui;
}

void MainWindow::reshapeButtons(){
    ui->start->setFixedSize(ui->start->width(), ui->start->width() / 2);
    ui->locate->setFixedSize(ui->locate->width(),ui->locate->width()/2);
    ui->screenshot->setFixedSize(ui->screenshot->width(),ui->screenshot->width()/2);
    ui->pause->setFixedSize(ui->pause->width(), ui->pause->width() / 2);
    ui->stop->setFixedSize(ui->stop->width(), ui->stop->width() / 2);
    ui->video->setFixedSize(ui->video->width(), ui->video->width() / 2);
    ui->add_angle->setFixedSize(ui->add_angle->width(), ui->add_angle->width() * 7 / 20);
    ui->choose1->setFixedSize(ui->choose1->width(), ui->choose1->width() / 3);
    ui->choose2->setFixedSize(ui->choose2->width(), ui->choose2->width() / 3);
    ui->choose3->setFixedSize(ui->choose3->width(), ui->choose3->width() / 3);
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
//    if (!dir->exists(tfolder+"//2dskeleton"))
//    {
//        mkdirsuccess=dir->mkdir(tfolder+"//2dskeleton");
//    }
    return tfolder;
}

void MainWindow::Kinectrun(QByteArray ba)
{
    char* folder = ba.data();
    CBodyBasics myKinect;
    HRESULT hr = myKinect.InitializeDefaultSensor(folder);
    myKinect.setoffset(left_hip_div,right_hip_div,1);
    QImage rgbimg,skeletonimg;
    int cnt=0;
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
                    cnt++;
                    if (ifrecrgbimg)
                    {
                        char ss[100];
                        sprintf(ss, "%s\\color\\%d.jpg", folder, cnt);
                        imwrite(ss,myKinect.colorImg);
                        ifrecrgbimg=false;
                    }
                    rgbimg=mat2qimage(myKinect.colorImg);
                    skeletonimg=mat2qimage(myKinect.skeletonImg);
                    ui->skeleton->setPixmap(QPixmap::fromImage(skeletonimg.scaled(ui->skeleton->width(),ui->skeleton->height(),Qt::KeepAspectRatio)));
                    ui->skeleton->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
                    ui->rgb->setPixmap(QPixmap::fromImage(rgbimg.scaled(ui->rgb->width(),ui->rgb->height(),Qt::KeepAspectRatio)));
                    ui->rgb->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

                    emit s_angles(myKinect.rec_angle_and_dis);
                    emit s_joints(myKinect.recjoint);
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

                //恢复初始状态
                ui->skeleton->clear();
                ui->rgb->clear();
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

    while(t.elapsed()<1000)
    QCoreApplication::processEvents();
}



void MainWindow::locate_hip()
{

    short lhipdiv_cnt[2*x_div+1][2*y_div+1][2*z_div+1];
    memset(lhipdiv_cnt,0,sizeof(lhipdiv_cnt));
    short rhipdiv_cnt[2*x_div+1][2*y_div+1][2*z_div+1];
    memset(rhipdiv_cnt,0,sizeof(rhipdiv_cnt));

    left_hip_div.x=0.0;
    left_hip_div.y=0.0;
    left_hip_div.z=0.0;

    right_hip_div.x=0.0;
    right_hip_div.y=0.0;
    right_hip_div.z=0.0;

    QString inputfolder=makedir();
    QByteArray ba = inputfolder.toLatin1();
    char* folder = ba.data();
    CBodyBasics myKinect;

    QImage rgbimg,skeletonimg;
    int times=LocateFrames;
    Joint knees[LocateFrames];
    Joint knees1[LocateFrames];
    int cnt=0;
    HRESULT hr = myKinect.InitializeDefaultSensor(folder);


    if (SUCCEEDED(hr))
    {
        emit(s_v1(-1));
        QTime t;
        t.start();
        while(t.elapsed()<100);
        while(1)
        {
            if (cnt>=times) break;

            flagLock.lock();
            if (takeflag==Kinect_exit)
            {
                emit(s_v1(LocateFrames));
                flagLock.unlock();

                //恢复初始状态
                ui->skeleton->clear();
                ui->rgb->clear();
                return;
            }
            flagLock.unlock();

            myKinect.Update();
            if(myKinect.readok==true && myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeLeft]==TrackingState_Tracked
                                     && myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipLeft]==TrackingState_Tracked
                                     && myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeRight]==TrackingState_Tracked
                                     && myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipRight]==TrackingState_Tracked)
            {
                knees[cnt]=myKinect.knee1;
                knees[cnt].Position.X-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipLeft][0];
                knees[cnt].Position.Y-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipLeft][1];
                knees[cnt].Position.Z-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipLeft][2];

                knees1[cnt]=myKinect.knee2;
                knees1[cnt].Position.X-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipRight][0];
                knees1[cnt].Position.Y-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipRight][1];
                knees1[cnt].Position.Z-=myKinect.recjoint.tjoint_coordinate_3d.plot3d[JointType_HipRight][2];

                cnt++;
            }

            rgbimg=mat2qimage(myKinect.colorImg);
            skeletonimg=mat2qimage(myKinect.skeletonImg);

            ui->skeleton->setPixmap(QPixmap::fromImage(skeletonimg.scaled(ui->skeleton->width(),ui->skeleton->height(),Qt::KeepAspectRatio)));
            ui->skeleton->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            ui->rgb->setPixmap(QPixmap::fromImage(rgbimg.scaled(ui->rgb->width(),ui->rgb->height(),Qt::KeepAspectRatio)));
            ui->rgb->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            emit(s_v1(cnt));
            if(qpd1->wasCanceled())
            {
                flagLock.lock();
                takeflag=Kinect_exit;
                flagLock.unlock();
                return;
            }

        }

        emit(s_v1(cnt));
    }

    emit(s_v2(-1));
    QTime t;
    t.start();
    while(t.elapsed()<500);

    for (int i=0;i<LocateFrames;i++)
    {
        cv::Point3i tmpldiv;
        tmpldiv.x=(int)(knees[i].Position.X*100);
        tmpldiv.y=(int)(knees[i].Position.Y*100);
        tmpldiv.z=(int)(knees[i].Position.Z*100);

        int tmpr = (int) pow( pow(tmpldiv.x,2.0) + pow(tmpldiv.y,2.0) + pow(tmpldiv.z,2.0) , 0.5 );

        for (int i=-x_div;i<=x_div;i++)
        {
            for (int j=-y_div;j<=y_div;j++)
            {
                for (int k=-z_div;k<=z_div;k++)
                {
                    int tmpr1=(int) pow( pow( tmpldiv.x - i, 2.0 ) + pow( tmpldiv.y-j,2.0 ) + pow(tmpldiv.z-k,2.0) , 0.5 );
                    if (abs(tmpr-tmpr1)<radious_div) lhipdiv_cnt[i+x_div][j+y_div][k+z_div]+=1;
                }
            }
        }

        cv::Point3i tmprdiv;
        tmprdiv.x=(int)(knees1[i].Position.X*100);
        tmprdiv.y=(int)(knees1[i].Position.Y*100);
        tmprdiv.z=(int)(knees1[i].Position.Z*100);

        tmpr = (int) pow( pow(tmprdiv.x,2.0) + pow(tmprdiv.y,2.0) + pow(tmprdiv.z,2.0) , 0.5 );

        for (int i=-x_div;i<=x_div;i++)
        {
            for (int j=-y_div;j<=y_div;j++)
            {
                for (int k=-z_div;k<=z_div;k++)
                {
                    int tmpr1=(int) pow( pow( tmprdiv.x - i, 2.0 ) + pow( tmprdiv.y-j,2.0 ) + pow(tmprdiv.z-k,2.0) , 0.5 );
                    if (abs(tmpr-tmpr1)<radious_div) rhipdiv_cnt[i+x_div][j+y_div][k+z_div]++;
                }
            }
        }
        emit(s_v2(i));
    }

    short tmax=0;
    for (int i=0;i<=2*x_div;i++)
    {
        for (int j=0;j<=2*y_div;j++)
        {
            for (int k=0;k<=2*z_div;k++)
            {
                if (lhipdiv_cnt[i][j][k]>tmax)
                {
                    tmax=lhipdiv_cnt[i][j][k];
                }
            }
        }
    }
    float lsumx=0.0, lsumy=0.0, lsumz=0.0; int lsumcnt=0;
    for (int i=0;i<=2*x_div;i++)
    {
        for (int j=0;j<=2*y_div;j++)
        {
            for (int k=0;k<=2*z_div;k++)
            {
                if (lhipdiv_cnt[i][j][k]>(int) (tmax*taken_rate) )
                {
                    lsumx+=i-x_div;
                    lsumy+=j-y_div;
                    lsumz+=k-z_div;
                    lsumcnt++;
                }
            }
        }
    }
    lsumx/=(lsumcnt*100);
    lsumy/=(lsumcnt*100);
    lsumz/=(lsumcnt*100);

    tmax=0;
    for (int i=0;i<=2*x_div;i++)
    {
        for (int j=0;j<=2*y_div;j++)
        {
            for (int k=0;k<=2*z_div;k++)
            {
                if (rhipdiv_cnt[i][j][k]>tmax)
                {
                    tmax=rhipdiv_cnt[i][j][k];
                }
            }
        }
    }

    float rsumx=0.0, rsumy=0.0, rsumz=0.0; int rsumcnt=0;
    for (int i=0;i<=2*x_div;i++)
    {
        for (int j=0;j<=2*y_div;j++)
        {
            for (int k=0;k<=2*z_div;k++)
            {
                if (rhipdiv_cnt[i][j][k]>(int) (tmax*taken_rate) )
                {
                    rsumx+=i-x_div;
                    rsumy+=j-y_div;
                    rsumz+=k-z_div;
                    rsumcnt++;
                }
            }
        }
    }
    rsumx/=(rsumcnt*100);
    rsumy/=(rsumcnt*100);
    rsumz/=(rsumcnt*100);



    left_hip_div.x=lsumx;
    left_hip_div.y=lsumy;
    left_hip_div.z=lsumz;

    right_hip_div.x=rsumx;
    right_hip_div.y=rsumy;
    right_hip_div.z=rsumz;

    //qDebug("%d\n %f %f %f\n %f %f %f\n",rsumcnt,lsumx, lsumy, lsumz,rsumx,rsumy,rsumz);

    flagLock.lock();
    takeflag=Kinect_exit;
    flagLock.unlock();
    emit(s_v2(LocateFrames));

}

void MainWindow::cal_valgus(){
    QString inputfolder=makedir();
    QByteArray ba = inputfolder.toLatin1();
    char* folder = ba.data();
    CBodyBasics myKinect;

    QImage rgbimg,skeletonimg;
    HRESULT hr = myKinect.InitializeDefaultSensor(folder);

    if (SUCCEEDED(hr)){
        QTime t;
        t.start();
        while(t.elapsed()<100);
        while(1){
            flagLock.lock();
            if (takeflag==Kinect_exit)
            {
                flagLock.unlock();
                //恢复初始状态
                ui->skeleton->clear();
                ui->rgb->clear();
                return;
            }
            flagLock.unlock();

            myKinect.Update();
            bool checkleft = myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeLeft] == TrackingState_Tracked &&
                             myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipLeft] == TrackingState_Tracked &&
                             myKinect.recjoint.tjoint_coordinate_3d.status[JointType_AnkleLeft == TrackingState_Tracked];
            bool checkright = myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeRight] == TrackingState_Tracked &&
                              myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipRight] == TrackingState_Tracked &&
                              myKinect.recjoint.tjoint_coordinate_3d.status[JointType_AnkleRight] == TrackingState_Tracked;
            bool checkboth = checkleft && checkright;
            if (myKinect.readok ==true && (ui->side->currentIndex() != 0 || checkleft) &&
                                          (ui->side->currentIndex() != 1 || checkright) &&
                                          (ui->side->currentIndex() != 2 || checkboth))
            {
                if(ui->side->currentIndex() == 0)
                    emit s_valgus(180 - (int)myKinect.rec_angle_and_dis[4]);
                else if(ui->side->currentIndex() == 1)
                    emit s_valgus(180 - (int)myKinect.rec_angle_and_dis[5]);
                else{
                    //任意一侧内翻取该侧
                    if (myKinect.rec_angle_and_dis[4] > 180)
                        emit s_valgus(180 - (int)myKinect.rec_angle_and_dis[4]);
                    else if (myKinect.rec_angle_and_dis[5] > 180)
                        emit s_valgus(180 - (int)myKinect.rec_angle_and_dis[5]);
                    //否则取大值
                    else
                        emit s_valgus(std::max(180 - (int)myKinect.rec_angle_and_dis[4], 180 - (int)myKinect.rec_angle_and_dis[5]));
                }
            }

            rgbimg=mat2qimage(myKinect.colorImg);
            skeletonimg=mat2qimage(myKinect.skeletonImg);

            ui->skeleton->setPixmap(QPixmap::fromImage(skeletonimg.scaled(ui->skeleton->width(),ui->skeleton->height(),Qt::KeepAspectRatio)));
            ui->skeleton->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            ui->rgb->setPixmap(QPixmap::fromImage(rgbimg.scaled(ui->rgb->width(),ui->rgb->height(),Qt::KeepAspectRatio)));
            ui->rgb->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        }
    }
}

//使用外旋+内收联合判断是否能系鞋带
void MainWindow::cal_shoes(){
    QString inputfolder=makedir();
    QByteArray ba = inputfolder.toLatin1();
    char* folder = ba.data();
    CBodyBasics myKinect;

    QImage rgbimg,skeletonimg;
    HRESULT hr = myKinect.InitializeDefaultSensor(folder);

    if (SUCCEEDED(hr)){
        QTime t;
        t.start();
        while(t.elapsed()<100);
        while(1){
            flagLock.lock();
            if (takeflag==Kinect_exit)
            {
                flagLock.unlock();
                //恢复初始状态
                ui->skeleton->clear();
                ui->rgb->clear();
                return;
            }
            flagLock.unlock();

            myKinect.Update();
            bool checkleft = myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeLeft] != TrackingState_NotTracked &&
                             myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipLeft] != TrackingState_NotTracked &&
                             myKinect.recjoint.tjoint_coordinate_3d.status[JointType_AnkleLeft != TrackingState_NotTracked];
            bool checkright = myKinect.recjoint.tjoint_coordinate_3d.status[JointType_KneeRight] != TrackingState_NotTracked &&
                              myKinect.recjoint.tjoint_coordinate_3d.status[JointType_HipRight] != TrackingState_NotTracked &&
                              myKinect.recjoint.tjoint_coordinate_3d.status[JointType_AnkleRight] != TrackingState_NotTracked;
            bool checkboth = checkleft && checkright;
            if (myKinect.readok ==true && (ui->side->currentIndex() != 0 || checkleft) &&
                                          (ui->side->currentIndex() != 1 || checkright) &&
                                          (ui->side->currentIndex() != 2 || checkboth))
            {
                if(ui->side->currentIndex() == 0){
                    emit s_shoes(myKinect.rec_angle_and_dis[16] + myKinect.rec_angle_and_dis[20]);
                } else if(ui->side->currentIndex() == 1){
                    emit s_shoes(myKinect.rec_angle_and_dis[17] + myKinect.rec_angle_and_dis[21]);
                } else{
                    emit s_shoes(std::min(myKinect.rec_angle_and_dis[16] + myKinect.rec_angle_and_dis[20], myKinect.rec_angle_and_dis[17] + myKinect.rec_angle_and_dis[21]));
                }
            }

            rgbimg=mat2qimage(myKinect.colorImg);
            skeletonimg=mat2qimage(myKinect.skeletonImg);

            ui->skeleton->setPixmap(QPixmap::fromImage(skeletonimg.scaled(ui->skeleton->width(),ui->skeleton->height(),Qt::KeepAspectRatio)));
            ui->skeleton->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            ui->rgb->setPixmap(QPixmap::fromImage(rgbimg.scaled(ui->rgb->width(),ui->rgb->height(),Qt::KeepAspectRatio)));
            ui->rgb->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        }
    }
}

//cv::Point3f MainWindow::findpoint(cv::Point3f point1,cv::Point3f point2,cv::Point3f point3)
//{
//    cv::Point3f line1,line2,nline,dline1,dline2,inline1,inline2;

//    inline1.x=0.5*(point1.x+point2.x);inline1.y=0.5*(point1.y+point2.y);inline1.z=0.5*(point1.z+point2.z);
//    inline2.x=0.5*(point2.x+point3.x);inline2.y=0.5*(point2.y+point3.y);inline2.z=0.5*(point2.z+point3.z);

//    line1.x=point1.x-point2.x;line1.y=point1.y-point2.y;line1.z=point1.z-point2.z;
//    line2.x=point3.x-point2.x;line2.y=point3.y-point2.y;line2.z=point3.z-point2.z;
//    nline = calcnormal(line1,line2);
//    dline1 = calcnormal(line1,nline);
//    dline2 = calcnormal(line2,nline);
//    if (findokflag==false) return point1;
//    return findinter(dline1,inline1,dline2,inline2);
//}

//cv::Point3f MainWindow::findinter(cv::Point3f d1, cv::Point3f p1, cv::Point3f d2, cv::Point3f p2)
//{
//    float x1=p1.x,x2=p2.x,y1=p1.y,y2=p2.y,z1=p1.z,z2=p2.z,u1=d1.x,u2=d2.x,v1=d1.y,v2=d2.y,w1=d1.z,w2=d2.z;
//    if ((v2*u1-v1*u2)<0.0000001)
//    {
//        findokflag=false;
//        return d1;
//    }
//    float t2 = (v1*(x2-x1)-u1*(y2-y1))/(v2*u1-v1*u2);
//    float x = x2+u2*t2, y = y2+v2*t2, z = z2+w2*t2;
//    cv::Point3f res;
//    res.x=x;
//    res.y=y;
//    res.z=z;
//    return res;
//}

//cv::Point3f MainWindow::calcnormal(cv::Point3f line1,cv::Point3f line2)
//{
//    cv::Point3f normal;
//    normal.x=line1.y*line2.z-line1.z*line2.y;
//    normal.y=line1.z*line2.x-line1.x*line2.z;
//    normal.z=line1.x*line2.y-line1.y*line2.x;
//    float tt = pow(pow(normal.x,2.0)+pow(normal.y,2.0)+pow(normal.z,2.0), 0.5 );
//    if (tt<0.000000001)
//    {
//        findokflag=false;
//        return normal;
//    }
//    normal.x/=tt; normal.y/=tt; normal.z/=tt;
//    return normal;

//}

void MainWindow::r_v1(int num)
{
    if (num==-1)
    {
        qpd1 = new QProgressDialog(this);
        qpd1->setWindowTitle(QString::fromLocal8Bit("膝关节采集"));
        qpd1->setLabelText(QString::fromLocal8Bit("采集中"));
        qpd1->setRange(0, LocateFrames);
        qpd1->setModal(true);
        qpd1->setCancelButtonText(QString::fromLocal8Bit("取消"));
        qpd1->setMinimumDuration(0);

    }
    else if (num==-2)
    {
        qpd1 = new QProgressDialog(this);
        qpd1->setWindowTitle(QString::fromLocal8Bit("膝关节采集"));
        qpd1->setLabelText(QString::fromLocal8Bit("采集中"));
        qpd1->setRange(0, LocateFrames);
        qpd1->setModal(true);
        qpd1->setCancelButtonText(QString::fromLocal8Bit("取消"));
        qpd1->setMinimumDuration(0);

    }
    else
    {
        qpd1->setValue(num);
    }
}

void MainWindow::r_v2(int num)
{

    if (num==-1)
    {
        qpd2 = new QProgressDialog(this);
        qpd2->setWindowTitle(QString::fromLocal8Bit("旋转中心定位"));
        qpd2->setLabelText(QString::fromLocal8Bit("定位中"));
        qpd2->setRange(0, LocateFrames);
        qpd2->setModal(true);
        qpd2->setCancelButtonText(QString::fromLocal8Bit("取消"));
        qpd2->setMinimumDuration(0);
    }
    else if (num==-2)
    {
        qpd2 = new QProgressDialog(this);
        qpd2->setWindowTitle(QString::fromLocal8Bit("旋转中心定位"));
        qpd2->setLabelText(QString::fromLocal8Bit("定位中"));
        qpd2->setRange(0, LocateTimes+1);
        qpd2->setModal(true);
        qpd2->setCancelButtonText(QString::fromLocal8Bit("取消"));
        qpd2->setMinimumDuration(0);
    }
    else
    {

          qpd2->setValue(num);

    }
}


void MainWindow::on_locate_clicked()
{
    flagLock.lock();
    if (takeflag!=Kinect_exit)
    {
        flagLock.unlock();
        return;
    }
    takeflag=Kinect_locate;
    flagLock.unlock();
    QtConcurrent::run(this,&MainWindow::locate_hip);

}




void MainWindow::on_start_clicked()
{
    ifrecrgbimg=false;
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
    anglemanager->clear();
    for(int i = 1; i < ui->selected_angles->count(); i++){
        ((MyListItem *)ui->selected_angles->itemWidget(ui->selected_angles->item(i)))->setAngle(0);
    }
    connect(this,SIGNAL(s_angles(float*)),anglemanager,SLOT(r_angles(float*)));
    connect(this,SIGNAL(s_joints(bodyangle)),anglemanager,SLOT(r_joints(bodyangle)));
    connect(anglemanager,SIGNAL(s_stable_angle(QString, int)), this, SLOT(r_stable_angle(QString,int)));
    ui->video->setEnabled(false);
    curfolder = ba.data();
    QtConcurrent::run(this,&MainWindow::Kinectrun,ba);
}

void MainWindow::on_pause_clicked()
{

    flagLock.lock();
    if (takeflag==Kinect_take)
    {
        takeflag=Kinect_pause;
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//        ui->pause->setText(QString::fromLocal8Bit("继续"));
        ui->pause->setStyleSheet("QPushButton {border-image: url(:/button_background/images/button_continue.png);}\
                                  QPushButton:hover:!pressed {border-image: url(:/button_background/images/button_continue_hover.png);}\
                                  QPushButton:hover:pressed {border-image: url(:/button_background/images/button_continue_down.png);}");
    }
    else if (takeflag==Kinect_pause)
    {
        takeflag=Kinect_take;
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//        ui->pause->setText(QString::fromLocal8Bit("暂停"));
        ui->pause->setStyleSheet("QPushButton {border-image: url(:/button_background/images/button_pause.png);}\
                                  QPushButton:hover:!pressed {border-image: url(:/button_background/images/button_pause_hover.png);}\
                                  QPushButton:hover:pressed {border-image: url(:/button_background/images/button_pause_down.png);}");
    }
    flagLock.unlock();

}

void MainWindow::on_stop_clicked()
{
    flagLock.lock();
    takeflag=Kinect_exit;
    flagLock.unlock();
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//    ui->pause->setText(QString::fromLocal8Bit("暂停"));
    ui->pause->setStyleSheet("QPushButton {border-image: url(:/button_background/images/button_pause.png);}\
                              QPushButton:hover:!pressed {border-image: url(:/button_background/images/button_pause_hover.png);}\
                              QPushButton:hover:pressed {border-image: url(:/button_background/images/button_pause_down.png);}");
    disconnect(this,SIGNAL(s_angles(float*)),anglemanager,SLOT(r_angles(float*)));
    disconnect(this,SIGNAL(s_joints(bodyangle)),anglemanager,SLOT(r_joints(bodyangle)));
    disconnect(anglemanager,SIGNAL(s_stable_angle(QString, int)), this, SLOT(r_stable_angle(QString,int)));
    ui->video->setEnabled(true);
}

void MainWindow::on_Qrcode_clicked()
{
    QString parsestr;
    //Todo: angles information

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

//void MainWindow::on_seeangle_clicked()
//{
//    if (w2==NULL)
//    {
//        w2=new seeangle;
//    }
//    w2->setWindowTitle(QString::fromLocal8Bit("查看角度"));
//    w2->setWindowFlags(Qt::WindowCloseButtonHint);
//    w2->setGeometry(x()+100,y()+100,1151,350);
//    w2->setFixedWidth(1151);
//    w2->setFixedHeight(350);
//    connect(this,SIGNAL(s_angles(double*)),w2,SLOT(r_angles(double*)));
//    connect(w2,SIGNAL(s_end()),this,SLOT(r_end()));
//    flagLock1.lock();
//    seeangleflag=true;
//    flagLock1.unlock();
//    w2->show();
//}

void MainWindow::on_select_position_currentIndexChanged(int index)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->select_action->clear();
    if (index == 0){
        ui->select_action->addItem(QString::fromLocal8Bit("屈曲"));
        ui->select_action->addItem(QString::fromLocal8Bit("伸直"));
        ui->select_action->addItem(QString::fromLocal8Bit("股胫角"));
        ui->select_action->addItem(QString::fromLocal8Bit("间距"));

    }
    else if (index == 1){
        ui->select_action->addItem(QString::fromLocal8Bit("间距"));
    }
    else if (index == 2){
        ui->select_action->addItem(QString::fromLocal8Bit("屈曲"));
        ui->select_action->addItem(QString::fromLocal8Bit("伸直"));
        ui->select_action->addItem(QString::fromLocal8Bit("外展"));
        ui->select_action->addItem(QString::fromLocal8Bit("内收"));
        ui->select_action->addItem(QString::fromLocal8Bit("内旋"));
        ui->select_action->addItem(QString::fromLocal8Bit("外旋"));
    }
    else if (index == 3){
        ui->select_action->addItem(QString::fromLocal8Bit("屈曲"));
    }
    else if (index == 4){
        ui->select_action->addItem(QString::fromLocal8Bit("外展"));
        ui->select_action->addItem(QString::fromLocal8Bit("后伸"));
    }
    ui->select_action->setCurrentIndex(-1);
}

void MainWindow::on_add_angle_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    if(ui->select_side->currentIndex() == -1 || ui->select_position->currentIndex() == -1 || ui->select_action->currentIndex() == -1){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请确认要测量的角度"));
        return;
    }
    QString name = fromCategoryToName(ui->select_side->currentIndex(), ui->select_position->currentIndex(), ui->select_action->currentIndex());
    if(curListAngle.find(name) != curListAngle.end()){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("该角度已经添加"));
        return;
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags);
    curListAngle[name] = item;
    itemCategory[item] = std::make_tuple(ui->select_side->currentIndex(), ui->select_position->currentIndex(), ui->select_action->currentIndex());
    ui->selected_angles->addItem(item);

    MyListItem *myItem = new MyListItem(ui->selected_angles, Qt::Window);
    myItem->initItem(name, ui->selected_angles, item);
    connect(myItem, SIGNAL(deleteListItem(QListWidgetItem*)), this, SLOT(delete_list_item(QListWidgetItem*)));
    //myItem->show();
    ui->selected_angles->setItemWidget(item, myItem);

    item->setSizeHint(QSize(myItem->rect().width(), myItem->rect().height()));

    anglemanager->setcheck(ui->select_side->currentIndex(), ui->select_position->currentIndex(), ui->select_action->currentIndex(), true);
}

void MainWindow::delete_list_item(QListWidgetItem *item){
    //std::cout << ((MyListItem *)ui->selected_angles->itemWidget(item))->getName().toStdString() << endl;
    curListAngle.erase(((MyListItem *)ui->selected_angles->itemWidget(item))->getName());
    anglemanager->setcheck(std::get<0>(itemCategory[item]), std::get<1>(itemCategory[item]), std::get<2>(itemCategory[item]), false);
    itemCategory.erase(item);
    ui->selected_angles->removeItemWidget(item);
    ui->selected_angles->takeItem(ui->selected_angles->row(item));
    if (item != NULL)
        delete item;
}

void MainWindow::delete_diagnose_list_item(QListWidgetItem *item){
    ui->diagnosesList->removeItemWidget(item);
    ui->diagnosesList->takeItem(ui->diagnosesList->row(item));
    if (item != NULL)
        delete item;
}

QString MainWindow::fromCategoryToName(int i, int j, int k){
    QString name;
    QTextStream &qin = QTextStream(&name);
    if (i == 0)
        qin << QString::fromLocal8Bit("左");
    else if (i == 1)
        qin << QString::fromLocal8Bit("右");
    if(j == 0){
        qin << QString::fromLocal8Bit("膝");
        if (k == 0)
            qin << QString::fromLocal8Bit("屈曲");
        else if (k == 1)
            qin << QString::fromLocal8Bit("伸直");
        else if (k == 2)
            qin << QString::fromLocal8Bit("股胫角");
        else if (k == 3)
            qin << QString::fromLocal8Bit("间距");
    }
    else if (j == 1){
        qin << QString::fromLocal8Bit("踝");
        if (k == 0)
            qin << QString::fromLocal8Bit("间距");
    }
    else if (j == 2){
        qin << QString::fromLocal8Bit("髋");
        if (k == 0)
            qin << QString::fromLocal8Bit("屈曲");
        else if (k == 1)
            qin << QString::fromLocal8Bit("伸直");
        else if (k == 2)
            qin << QString::fromLocal8Bit("外展");
        else if (k == 3)
            qin << QString::fromLocal8Bit("内收");
        else if (k == 4)
            qin << QString::fromLocal8Bit("内旋");
        else if (k == 5)
            qin << QString::fromLocal8Bit("外旋");
    }
    else if (j == 3){
        qin << QString::fromLocal8Bit("肘");
        if (k == 0)
            qin << QString::fromLocal8Bit("屈曲");
    }
    else if(j == 4){
        qin << QString::fromLocal8Bit("肩");
        if (k == 0)
            qin << QString::fromLocal8Bit("外展");
        else if (k == 1)
            qin << QString::fromLocal8Bit("后伸");
    }
    return name;
}

void MainWindow::r_stable_angle(QString name, int angle){
    ((MyListItem *)ui->selected_angles->itemWidget(curListAngle[name]))->setAngle(angle);
}

void MainWindow::on_video_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //检查路径
    if(curfolder.isNull()){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无数据，请点击开始按钮开始检测"));
        return;
    }
    //读入数据
    char s[1000];
    sprintf(s, "%s\\skeleton\\save", curfolder.toLatin1().data());

    ifstream load;
    load.open(s);
    if(!load){
        curfolder = QString::null;
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("读入数据失败，请重新检测"));
        load.close();
        return;
    }

    vector<bodyangle> alldata;
    bodyangle tmp;
    char t[10000];
    while(load.getline(t, sizeof(t))){
        boost::iostreams::basic_array<char> sr(t, strlen(t));
        boost::iostreams::stream<boost::iostreams::basic_array<char> > source(sr);
        boost::archive::text_iarchive ia(source);
        ia >> tmp;
        alldata.emplace_back(tmp);
    }

    if(alldata.size() == 0){
        curfolder = QString::null;
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据为空，无法生成视频"));
        load.close();
        return;
    }
    //显式进度条
    QProgressDialog process(this);
    process.setWindowTitle(QString::fromLocal8Bit("提示"));
    process.setLabelText(QString::fromLocal8Bit("处理中"));
    process.setRange(0, alldata.size() - 1);
    process.setModal(true);
    process.setCancelButtonText(QString::fromLocal8Bit("取消"));
    process.setMinimumDuration(0);


    //生成视频并更新进度条
    int fourcc = CV_FOURCC('M', 'J', 'P', 'G');
    double fps = 30;
    bool iscolor = true;
    cv::Mat frame;
    cv::VideoWriter Writer = cv::VideoWriter(curfolder.toStdString() + "\\video.avi", fourcc, fps, cv::Size(bodyangle::cDepthWidth, bodyangle::cDepthHeight), iscolor);
    for(int i = 0; i < alldata.size(); i++){
        frame = alldata[i].draw();
        if(i > 0){
            cv::Mat none;
            none.create(bodyangle::cDepthHeight, bodyangle::cDepthWidth, CV_8UC3);
            none.setTo(0);
            for(int j = alldata[i - 1].frame + 1; j < alldata[i].frame; j++){
                Writer.write(none);
            }
        }
        Writer.write(frame);
        process.setValue(i);
        if(process.wasCanceled())
            break;
    }
    Writer.release();
    load.close();
    curfolder = QString::null;
}

void MainWindow::infoPage(){
    //personal info
    ui->age->setValidator(new QIntValidator(0, 150, this));
    ui->sex->setCurrentIndex(-1);
    QDoubleValidator *dvalidator = new QDoubleValidator(1, 300, 1, this);
    dvalidator->setNotation(QDoubleValidator::StandardNotation);
    ui->height->setValidator(dvalidator);
    ui->weight->setValidator(dvalidator);
    connect(ui->height, SIGNAL(textEdited(QString)), this, SLOT(setBMI(QString)));
    connect(ui->weight, SIGNAL(textEdited(QString)), this, SLOT(setBMI(QString)));
    ui->mtype->setCurrentIndex(-1);
    QRegExp rx("[0-9]+$");
    QRegExpValidator *rvalidator = new QRegExpValidator(rx);
    ui->card->setValidator(rvalidator);

    //symptom
    ui->side->setCurrentIndex(-1);
    ui->pos->setCurrentIndex(-1);
    ui->status->setCurrentIndex(-1);
    connect(ui->aggravate, SIGNAL(toggled(bool)), ui->ayear, SLOT(setEnabled(bool)));
    connect(ui->aggravate, SIGNAL(toggled(bool)), ui->amonth, SLOT(setEnabled(bool)));
    connect(ui->aggravate, SIGNAL(toggled(bool)), ui->aday, SLOT(setEnabled(bool)));

    //diagnose
    diagnoseChoose = new QButtonGroup(this);
    diagnoseChoose->addButton(ui->knee, 0);
    diagnoseChoose->addButton(ui->hip, 1);
    connect(ui->knee, SIGNAL(clicked()), this, SLOT(onRadioClickKnee()));
    connect(ui->hip, SIGNAL(clicked()), this, SLOT(onRadioClickHip()));

}

void MainWindow::setBMI(QString s){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    if(!ui->height->text().isEmpty() && !ui->weight->text().isEmpty()){
        double h = ui->height->text().toDouble() / 100;
        double w = ui->weight->text().toDouble();
        double result = w / (h * h);
        QString eval;
        if(result < 18.5)
            eval = QString::fromLocal8Bit("（过轻）");
        else if(result < 25)
            eval = QString::fromLocal8Bit("（正常）");
        else if(result < 28)
            eval = QString::fromLocal8Bit("（过重）");
        else if(result < 32)
            eval = QString::fromLocal8Bit("（肥胖）");
        else
            eval = QString::fromLocal8Bit("（非常肥胖）");
        ui->BMI->setText(QString::number(result, 'f', 1) + eval);
    }
    else
        ui->BMI->setText(QString::null);
}


void  MainWindow::onRadioClickKnee(){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->diagnose->clear();
    ui->diagnosesList->clear();
    ui->diagnose->addItems(QStringList() << QString::fromLocal8Bit("膝内翻") <<
                           QString::fromLocal8Bit("膝关节骨性关节炎（三间室退变型）") <<
                           QString::fromLocal8Bit("膝关节骨性关节炎（创伤后）") <<
                           QString::fromLocal8Bit("膝外翻") <<
                           QString::fromLocal8Bit("膝关节内侧间室骨性关节炎") <<
                           QString::fromLocal8Bit("髌骨间室骨性关节炎") <<
                           QString::fromLocal8Bit("髌骨软化") <<
                           QString::fromLocal8Bit("膝关节外侧间室骨性关节炎") <<
                           QString::fromLocal8Bit("膝关节滑膜炎") <<
                           QString::fromLocal8Bit("膝关节积液") <<
                           QString::fromLocal8Bit("膝关节骨性关节炎（陈旧术后）") <<
                           QString::fromLocal8Bit("膝关节红肿") <<
                           QString::fromLocal8Bit("下肢畸形（股骨）") <<
                           QString::fromLocal8Bit("下肢畸形（胫骨）") <<
                           QString::fromLocal8Bit("类风湿性关节炎（膝）") <<
                           QString::fromLocal8Bit("强直性脊柱炎（膝）") <<
                           QString::fromLocal8Bit("膝关节感染") <<
                           QString::fromLocal8Bit("膝关节疼痛（不明原因）") <<
                           QString::fromLocal8Bit("胫骨结节骨骺炎") <<
                           QString::fromLocal8Bit("膝关节半月板损伤") <<
                           QString::fromLocal8Bit("膝关节骨梗死") <<
                           QString::fromLocal8Bit("人工膝关节置换术后假体松动") <<
                           QString::fromLocal8Bit("人工膝关节置换术后复查"));
    ui->diagnose->setCurrentIndex(-1);

}

void MainWindow::onRadioClickHip(){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->diagnose->clear();
    ui->diagnosesList->clear();
    ui->diagnose->addItems(QStringList() << QString::fromLocal8Bit("原发性髋关节骨性关节炎") <<
                           QString::fromLocal8Bit("髋臼发育不良") <<
                           QString::fromLocal8Bit("髋臼发育不良继发骨性关节炎") <<
                           QString::fromLocal8Bit("髋关节骨性关节炎（创伤性）") <<
                           QString::fromLocal8Bit("髋关节骨性关节炎（陈旧术后）") <<
                           QString::fromLocal8Bit("股骨头缺血坏死（0期）") <<
                           QString::fromLocal8Bit("股骨头缺血坏死（1期）") <<
                           QString::fromLocal8Bit("股骨头缺血坏死（2期）") <<
                           QString::fromLocal8Bit("股骨头缺血坏死（3期）") <<
                           QString::fromLocal8Bit("股骨头缺血坏死（4期）") <<
                           QString::fromLocal8Bit("髋关节骨性关节炎（陈旧感染）") <<
                           QString::fromLocal8Bit("髋关节疼痛（不明原因）") <<
                           QString::fromLocal8Bit("髋关节积液") <<
                           QString::fromLocal8Bit("髋关节盂唇损伤") <<
                           QString::fromLocal8Bit("髋关节撞击综合征") <<
                           QString::fromLocal8Bit("髋关节感染") <<
                           QString::fromLocal8Bit("强直性脊柱炎（髋）") <<
                           QString::fromLocal8Bit("类风湿性关节炎（髋）") <<
                           QString::fromLocal8Bit("人工髋关节置换术后假体松动") <<
                           QString::fromLocal8Bit("人工髋关节置换术后复查"));
    ui->diagnose->setCurrentIndex(-1);
}

void MainWindow::on_diagnoseAdd_clicked()
{
    if (ui->diagnose->currentIndex() == -1 || ui->side->currentIndex() == -1)
        return;
    QListWidgetItem *item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags);
    ui->diagnosesList->addItem(item);

    MyListItem *myItem = new MyListItem(ui->diagnosesList, Qt::Window);
    myItem->initItem2(ui->diagnose->currentText(), ui->diagnosesList, item);
    connect(myItem, SIGNAL(deleteListItem(QListWidgetItem*)), this, SLOT(delete_diagnose_list_item(QListWidgetItem*)));
    ui->diagnosesList->setItemWidget(item, myItem);
    item->setSizeHint(QSize(myItem->rect().width(), myItem->rect().height()));
}

void MainWindow::on_diagnoseGenerate_clicked()
{
    if(ui->name->text().isEmpty() || ui->age->text().isEmpty() || ui->sex->currentIndex() == -1
            || ui->height->text().isEmpty() || ui->weight->text().isEmpty() || ui->mtype->currentIndex() == -1 || ui->card->text().isEmpty()){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请完整填写个人信息"));
        return;
    }
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->result->clear();
    QTextCursor cursor(ui->result->textCursor());
    QTextCharFormat format;
    cursor.movePosition(QTextCursor::Start);
//    format.setForeground(Qt::red);
//    cursor.mergeCharFormat(format);
    cursor.insertText(QDateTime::currentDateTime().toString(QString::fromLocal8Bit("yyyy年MM月dd日")));
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertText(QString::fromLocal8Bit("个人信息"));
    cursor.insertBlock();
    cursor.insertText(QString::fromLocal8Bit("姓名：") + ui->name->text() + '\t');
    cursor.insertText(QString::fromLocal8Bit("年龄：") + ui->age->text() + '\t');
    cursor.insertText(QString::fromLocal8Bit("性别：") + ui->sex->currentText() + '\t');
    cursor.insertBlock();
    cursor.insertText(QString::fromLocal8Bit("身高：") + ui->height->text() + "cm\t");
    cursor.insertText(QString::fromLocal8Bit("体重：") + ui->weight->text() + "kg\t");
    cursor.insertText(QString::fromLocal8Bit("BMI：") + ui->BMI->text() + '\t');
    cursor.insertBlock();
    cursor.insertText(QString::fromLocal8Bit("医疗类型：") + ui->mtype->currentText() + '\t');
    cursor.insertText(QString::fromLocal8Bit("京医通卡号：") + ui->card->text() + '\t');
    cursor.insertBlock();
    cursor.insertBlock();
    if(ui->side->currentIndex() == -1 || ui->pos->currentIndex() == -1 || ui->status->currentIndex() == -1){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请完整填写症状"));
        return;
    }
    QString status = QString::fromLocal8Bit("主诉：");
    status.append(ui->side->currentText() + ui->pos->currentText() + ui->status->currentText());
    if(ui->lyear->currentIndex() != 0)
        status.append(ui->lyear->currentText() + QString::fromLocal8Bit("年"));
    if(ui->lmonth->currentIndex() != 0)
        status.append(ui->lmonth->currentText() + QString::fromLocal8Bit("月"));
    if(ui->lday->currentIndex() != 0)
        status.append(ui->lday->currentText() + QString::fromLocal8Bit("日"));
    if(ui->aggravate->isChecked()){
        status.append(QString::fromLocal8Bit("加重"));
        if(ui->ayear->currentIndex() != 0)
            status.append(ui->ayear->currentText() + QString::fromLocal8Bit("年"));
        if(ui->amonth->currentIndex() != 0)
            status.append(ui->amonth->currentText() + QString::fromLocal8Bit("月"));
        if(ui->aday->currentIndex() != 0)
            status.append(ui->aday->currentText() + QString::fromLocal8Bit("日"));
    }
    cursor.insertText(status);
    cursor.insertBlock();
    cursor.insertBlock();
    QString grade = QString::fromLocal8Bit("评分：");
    grade.append(ui->score->text());
    cursor.insertText(grade);
    cursor.insertBlock();
    cursor.insertBlock();
    if(ui->diagnosesList->count() == 0){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请完整填写诊断"));
        return;
    }
    cursor.insertText(QString::fromLocal8Bit("诊断："));
    for(int i = 0; i < ui->diagnosesList->count(); i++)
        cursor.insertText(((MyListItem *)ui->diagnosesList->itemWidget(ui->diagnosesList->item(i)))->getName() + '\t');
}

void MainWindow::on_reset_clicked()
{
    hip_score_left = -1;
    hip_score_right = -1;
    knee_score_left = -1;
    knee_score_right = -1;

    ui->name->setText("");ui->age->setText("");ui->sex->setCurrentIndex(-1);ui->BMI->setText("");ui->score->setText("");
    ui->height->setText("");ui->weight->setText("");ui->mtype->setCurrentIndex(-1);ui->card->setText("");
    ui->side->setCurrentIndex(-1); ui->pos->setCurrentIndex(-1); ui->status->setCurrentIndex(-1);
    ui->lyear->setCurrentIndex(0);ui->lmonth->setCurrentIndex(0);ui->lday->setCurrentIndex(0);
    ui->ayear->setCurrentIndex(0);ui->amonth->setCurrentIndex(0);ui->aday->setCurrentIndex(0);
    ui->result->clear();

    while(ui->diagnosesList->count() != 0)
    {
        QListWidgetItem *item = ui->diagnosesList->item(0);
        ui->diagnosesList->takeItem(0);
        ui->diagnosesList->removeItemWidget(item);
        if (item!=NULL) delete item;
    }

}


void MainWindow::on_save_clicked()
{
    infoitem tmpitem;
    if(ui->name->text().isEmpty() || ui->age->text().isEmpty() || ui->sex->currentIndex() == -1
            || ui->height->text().isEmpty() || ui->weight->text().isEmpty() || ui->mtype->currentIndex() == -1 || ui->card->text().isEmpty()){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请完整填写个人信息"));
        return;
    }
    tmpitem.name = ui->name->text();
    tmpitem.age = ui->age->text();
    tmpitem.sex = ui->sex->currentText();
    tmpitem.height = ui->height->text();
    tmpitem.weight = ui->weight->text();
    tmpitem.BMI = ui->BMI->text();
    tmpitem.medicinetype = ui->mtype->currentText();
    tmpitem.cardid = ui->card->text();
    tmpitem.score = ui->score->text();
    if(ui->side->currentIndex() == -1 || ui->pos->currentIndex() == -1 || ui->status->currentIndex() == -1){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请完整填写症状"));
        return;
    }
    QString status = QString::fromLocal8Bit("");
    status.append(ui->side->currentText() + ui->pos->currentText() + ui->status->currentText());
    if(ui->lyear->currentIndex() != 0)
        status.append(ui->lyear->currentText() + QString::fromLocal8Bit("年"));
    if(ui->lmonth->currentIndex() != 0)
        status.append(ui->lmonth->currentText() + QString::fromLocal8Bit("月"));
    if(ui->lday->currentIndex() != 0)
        status.append(ui->lday->currentText() + QString::fromLocal8Bit("日"));
    if(ui->aggravate->isChecked()){
        status.append(QString::fromLocal8Bit("加重"));
        if(ui->ayear->currentIndex() != 0)
            status.append(ui->ayear->currentText() + QString::fromLocal8Bit("年"));
        if(ui->amonth->currentIndex() != 0)
            status.append(ui->amonth->currentText() + QString::fromLocal8Bit("月"));
        if(ui->aday->currentIndex() != 0)
            status.append(ui->aday->currentText() + QString::fromLocal8Bit("日"));
    }
    tmpitem.complain = status;

    QString dia;
    dia = "";
    for(int i = 1; i < ui->diagnosesList->count(); i++)
        dia += (((MyListItem *)ui->diagnosesList->itemWidget(ui->diagnosesList->item(i)))->getName() + "    ");
    tmpitem.diagnosis = dia;

//    if (md_db.query_infoitem(tmpitem.cardid))
//    {
//        md_db.update_infoitem(tmpitem);
//    }
//    else
//    {
//        md_db.insert_infoitem(tmpitem);
//    }
    if (Database::getInstance()->insert_infoitem(tmpitem))
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("录入成功"));
    //on_reset_clicked();
}

void MainWindow::on_screenshot_clicked()
{
    ifrecrgbimg=true;
}

void MainWindow::on_mark_clicked()
{
    flagLock.lock();
    bool ismarkable = takeflag == Kinect_exit;
    flagLock.unlock();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    if (!ismarkable){
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先结束拍摄"));
        return;
    }
//    if (anglemanager->get_max_angles(0) == -360){
//        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先拍摄一段数据"));
//        return;
//    }
    if (ui->side->currentIndex() == -1)
        return;
    if (ui->knee->isChecked()){
        KneeScoreWizard wizard(this, ui->side->currentText());
        connect(&wizard, SIGNAL(scoreChanged(QString)), ui->score, SLOT(setText(QString)));
        //计算自动得分部分
        //活动范围
        int range;
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(0) - anglemanager->get_min_angles(0);
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(1) - anglemanager->get_min_angles(1);
        }else{
            range = std::min(anglemanager->get_max_angles(0) - anglemanager->get_min_angles(0),
                            anglemanager->get_max_angles(1) - anglemanager->get_min_angles(1));
        }
        wizard.setField("second", min(25, max(0, range / 5)));
        //屈曲挛缩畸形
        int angle, s;
        if (ui->side->currentIndex() == 0)
            angle = anglemanager->get_min_angles(0);
        else if(ui->side->currentIndex() == 1)
            angle = anglemanager->get_min_angles(1);
        else
            angle = std::max(anglemanager->get_min_angles(0), anglemanager->get_min_angles(1));
        if (angle < 5)
            s = 0;
        else if(angle < 10)
            s = -2;
        else if(angle < 15)
            s = -5;
        else if(angle < 20)
            s = -10;
        else
            s = -15;
        wizard.setField("fifth", s);
        //外翻力线
        connect((KneeValgusPage*)wizard.page(3), SIGNAL(start_Kinect()), this, SLOT(on_cal_valgus_recieve()));
        connect((KneeValgusPage*)wizard.page(3), SIGNAL(stop_Kinect()), this, SLOT(on_stop_clicked()));
        connect(this,SIGNAL(s_valgus(int)), (KneeValgusPage*)wizard.page(3), SLOT(setValgus(int)));
        wizard.exec();
    }
    else if (ui->hip->isChecked()){
        HipScoreWizard wizard(this, ui->side->currentText());
        connect(&wizard, SIGNAL(scoreChanged(QString)), ui->score, SLOT(setText(QString)));
        //计算自动得到部分
        //todo
        //跛行
        int d = anglemanager->get_limpDiff();
        if (d <= 3)
            wizard.setField("second", 11);
        else if (d <= 5)
            wizard.setField("second", 8);
        else if (d <= 10)
            wizard.setField("second", 5);
        else
            wizard.setField("second", 0);
        //穿袜子
        connect((HipPutOnShoesPage*)wizard.page(4), SIGNAL(start_Kinect()), this, SLOT(on_cal_shoes_recieve()));
        connect((HipPutOnShoesPage*)wizard.page(4), SIGNAL(stop_Kinect()), this, SLOT(on_stop_clicked()));
        connect(this,SIGNAL(s_shoes(int)), (KneeValgusPage*)wizard.page(4), SLOT(setShoes(int)));
        //畸形表现
        int s = 4;
        if (ui->side->currentIndex() == 0){
            //下肢不等长
            if (anglemanager->get_limbDiff() > 3)
                s = 0;
            //固定内收畸形 固定内旋畸形 屈髋挛缩畸形
            if (anglemanager->get_min_angles(16) > 10 || anglemanager->get_min_angles(18) > 10 ||
                    anglemanager->get_min_angles(10) > 30)
                s = 0;

        }else if (ui->side->currentIndex() == 1){
            if (anglemanager->get_min_angles(17) > 10 || anglemanager->get_min_angles(19) > 10 ||
                    anglemanager->get_min_angles(11) > 30)
                s = 0;
        }else{
            if (std::max(anglemanager->get_min_angles(16), anglemanager->get_min_angles(17)) > 10 ||
                    std::max(anglemanager->get_min_angles(18), anglemanager->get_min_angles(19)) > 10 ||
                    std::max(anglemanager->get_min_angles(10), anglemanager->get_min_angles(11)) > 30)
                s = 0;
        }
        wizard.setField("ninth", s);
        //活动范围
        int range, rangescore = 0;
        //屈曲
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(10) - anglemanager->get_min_angles(10);
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(11) - anglemanager->get_min_angles(11);
        }else{
            range = std::min(anglemanager->get_max_angles(10) - anglemanager->get_min_angles(10),
                            anglemanager->get_max_angles(11) - anglemanager->get_min_angles(11));
        }
        if (range >= 90)
            rangescore++;
        //外展，因角度计算不存在负值，只考虑最大值
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(14);
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(15);
        }else{
            range = std::min(anglemanager->get_max_angles(14), anglemanager->get_max_angles(15));
        }
        if (range >= 30)
            rangescore++;
        //内收，最大值减最小值
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(16) - anglemanager->get_min_angles(16);
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(17) - anglemanager->get_min_angles(17);
        }else{
            range = std::min(anglemanager->get_max_angles(16) - anglemanager->get_min_angles(16),
                            anglemanager->get_max_angles(17) - anglemanager->get_min_angles(17));
        }
        if (range >= 20)
            rangescore++;
        //外旋
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(20) - std::max(0, anglemanager->get_min_angles(20));
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(21) - std::max(0, anglemanager->get_min_angles(21));
        }else{
            range = std::min(anglemanager->get_max_angles(20) - std::max(0, anglemanager->get_min_angles(20)),
                            anglemanager->get_max_angles(21) - std::max(0, anglemanager->get_min_angles(21)));
        }
        if (range >= 20)
            rangescore++;
        //内旋
        if (ui->side->currentIndex() == 0){
            range = anglemanager->get_max_angles(18) - std::max(0, anglemanager->get_min_angles(18));
        }else if (ui->side->currentIndex() == 1){
            range = anglemanager->get_max_angles(19) - std::max(0, anglemanager->get_min_angles(19));
        }else{
            range = std::min(anglemanager->get_max_angles(18) - std::max(0, anglemanager->get_min_angles(18)),
                            anglemanager->get_max_angles(19) - std::max(0, anglemanager->get_min_angles(19)));
        }
        if (range >= 15)
            rangescore++;
        wizard.setField("tenth", rangescore);
        wizard.exec();
    }
}

void MainWindow::on_cal_valgus_recieve(){
    flagLock.lock();
    if (takeflag!=Kinect_exit)
    {
        flagLock.unlock();
        return;
    }
    takeflag=Kinect_valgus;
    flagLock.unlock();
    QtConcurrent::run(this,&MainWindow::cal_valgus);
}

void MainWindow::on_cal_shoes_recieve(){
    flagLock.lock();
    if (takeflag!=Kinect_exit)
    {
        flagLock.unlock();
        return;
    }
    takeflag=Kinect_shoes;
    flagLock.unlock();
    QtConcurrent::run(this,&MainWindow::cal_shoes);
}

void MainWindow::on_query_clicked()
{
    if(NULL == queryWidget){
        queryWidget = new QueryWidget;
        queryWidget->setWindowTitle(QString::fromLocal8Bit("数据库查询"));
    }
    if (queryWidget->isVisible()){
        queryWidget->close();
    }
    queryWidget->update_data();
    queryWidget->show();
}
