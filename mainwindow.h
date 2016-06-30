#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <map>
#include <QString>
#include "myKinect.h"
#include "fillinfo.h"
#include "mylistitem.h"
#include "anglemanager.h"
#include "QRController.h"

enum taketype{Kinect_take=0,Kinect_pause=1,Kinect_exit=2};

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
    QString makedir();
    void Kinectrun(QByteArray ba);
    QString showangle[22];
    QString saveangle[22];
private:
    QString fromCategoryToName(int i, int j, int k);

private slots:
    void on_start_clicked();
    
    void on_pause_clicked();
    
    void on_stop_clicked();
    
    void on_Qrcode_clicked();

    void on_fillinfo_clicked();

    //void on_seeangle_clicked();

    void r_end();

    void on_select_position_currentIndexChanged(int index);

    void on_add_angle_clicked();

    void delete_list_item(QListWidgetItem *item);

    void r_stable_angle(QString name, int angle);

signals:
    void s_angles(double*);


protected:
   void closeEvent(QCloseEvent *e);
private:
    Ui::MainWindow *ui;
    taketype takeflag;
    bool seeangleflag;
    fillinfo* w1;
    map<QString, QListWidgetItem*> curListAngle;
    map<QListWidgetItem*, std::tuple<int, int, int>> itemCategory;
    AngleManager* anglemanager;
};

#endif // MAINWINDOW_H
