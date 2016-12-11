#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <map>
#include <vector>
#include <QString>
#include <QProgressDialog>
#include "myKinect.h"
#include "mylistitem.h"
#include "anglemanager.h"
#include "QRController.h"
#include "bodyangle.h"
#include "mylistheaditem.h"
#include "database.h"
#include "kneescorewizard.h"
#include "hipscorewizard.h"
#include "querywidget.h"

enum taketype{Kinect_take=0,Kinect_pause=1,Kinect_exit=2,Kinect_locate=3,Kinect_valgus=4, Kinect_shoes=5};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString folder;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void reshapeButtons();
    QString makedir();
    void Kinectrun(QByteArray ba);
    void locate_hip();
    void locate_hip_other();
    void cal_valgus();
    void cal_shoes();
    QString showangle[28];
    QString saveangle[28];
    int hip_score_left;
    int hip_score_right;
    int knee_score_left;
    int knee_score_right;

private:
    QString fromCategoryToName(int i, int j, int k);
    void infoPage();

private slots:
    void on_start_clicked();
    
    void on_pause_clicked();
    
    void on_stop_clicked();
    
    void on_Qrcode_clicked();

    //void on_fillinfo_clicked();

    //void on_seeangle_clicked();

    //void r_end();

    void on_select_position_currentIndexChanged(int index);

    void on_add_angle_clicked();

    void delete_list_item(QListWidgetItem *item);

    void delete_diagnose_list_item(QListWidgetItem *item);

    void r_stable_angle(QString name, int angle);

    void on_video_clicked();

    void setBMI(QString s);

    void onRadioClickKnee();

    void onRadioClickHip();

    void on_diagnoseAdd_clicked();

    void on_diagnoseGenerate_clicked();

    void r_v1(int num);
    void r_v2(int num);

    void on_locate_clicked();

    void on_save_clicked();

    void on_screenshot_clicked();

    void on_mark_clicked();

    void on_reset_clicked();

    void on_cal_valgus_recieve();

    void on_cal_shoes_recieve();

    void on_query_clicked();

signals:
    void s_angles(float*);
    void s_joints(bodyangle);
    void s_v1(int);
    void s_v2(int);
    void s_valgus(int);
    void s_shoes(int);


protected:
   void closeEvent(QCloseEvent *e);
private:
    cv::Point3f left_hip_div;
    cv::Point3f right_hip_div;
    cv::Point3f left_hip;
    cv::Point3f right_hip;
    QProgressDialog* qpd0;
    QProgressDialog* qpd1;
    QProgressDialog* qpd2;
//   cv::Point3f findpoint(cv::Point3f point1,cv::Point3f point2,cv::Point3f point3);
//   cv::Point3f calcnormal(cv::Point3f line1,cv::Point3f line2);
//   cv::Point3f findinter(cv::Point3f d1, cv::Point3f p1, cv::Point3f d2, cv::Point3f p2);
//   bool findokflag;

    //数据库相关
    QueryWidget *queryWidget;

    //截图
    bool ifrecrgbimg;

    Ui::MainWindow *ui;
    taketype takeflag;
    QButtonGroup* tabChoose;
    QButtonGroup* diagnoseChoose;
    map<QString, QListWidgetItem*> curListAngle;
    map<QListWidgetItem*, std::tuple<int, int, int>> itemCategory;
    AngleManager* anglemanager;
    QString curfolder;
};

#endif // MAINWINDOW_H
