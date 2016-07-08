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
#include "mylistitem.h"
#include "boost/iostreams/stream.hpp"
#include "boost/iostreams/device/array.hpp"

static QMutex flagLock;
static QMutex testlock;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    w1=NULL;
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
}

MainWindow::~MainWindow()
{
    delete tabChoose;
    delete anglemanager;
    delete ui;
}

void MainWindow::reshapeButtons(){
    ui->start->setFixedSize(ui->start->width(), ui->start->width() / 2);
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

//                    flagLock1.lock();
//                    if (seeangleflag==true)
//                    {
//                        emit s_angles(myKinect.rec_angle_and_dis);
//                    }
//                    flagLock1.unlock();
                    emit s_angles(myKinect.rec_angle_and_dis);
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
    if (w1!=NULL)
    {
        w1->close();
        delete w1;
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
    anglemanager->clear();
    for(int i = 1; i < ui->selected_angles->count(); i++){
        ((MyListItem *)ui->selected_angles->itemWidget(ui->selected_angles->item(i)))->setAngle(0);
    }
    connect(this,SIGNAL(s_angles(float*)),anglemanager,SLOT(r_angles(float*)));
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
    disconnect(anglemanager,SIGNAL(s_stable_angle(QString, int)), this, SLOT(r_stable_angle(QString,int)));
    ui->video->setEnabled(true);
    delete w1;
    w1=NULL;
}

void MainWindow::on_Qrcode_clicked()
{
    if (w1==NULL) return;
    QString parsestr;
    parsestr=w1->name+'$'+w1->gender+'$'+w1->age+'$'+w1->height+'$'+w1->weight+'$'+w1->medicaltype
            +'$'+w1->diagonasis+'$'+w1->kneediagnosis+'$'+w1->hipdiagnosis+'$'+w1->otherdiagnosis+'$'
            +w1->side+'$'+w1->year+'$'+w1->month+'$'+w1->day;
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
