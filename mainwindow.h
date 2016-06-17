#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QString"
#include "myKinect.h"
#include "fillinfo.h"
#include "seeangle.h"
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

private slots:
    void on_start_clicked();
    
    void on_pause_clicked();
    
    void on_stop_clicked();
    
    void on_Qrcode_clicked();

    void on_fillinfo_clicked();

    void on_seeangle_clicked();

    void r_end();

signals:
    void s_angles(double*);


protected:
   void closeEvent(QCloseEvent *e);
private:
    Ui::MainWindow *ui;
    taketype takeflag;
    bool seeangleflag;
    fillinfo* w1;
    seeangle* w2;
};

#endif // MAINWINDOW_H
