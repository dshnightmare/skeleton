#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include "fstream"
#include <Kinect.h>



int main(int argc, char *argv[])
{

//    {
//        //可以在这里写
//        IKinectSensor* sensor;
//        GetDefaultKinectSensor(&sensor);
//        if (sensor->Open())
//        {
//            ICoordinateMapper*      m_pCoordinateMapper;
//            sensor->get_CoordinateMapper(&m_pCoordinateMapper);
//    //        m_pCoordinateMapper->
//            CameraSpacePoint poi1;
//            poi1.X=-1.477638;poi1.Y=0.226764;poi1.Z=2.446001;
//            ColorSpacePoint* cpoi1;
//            m_pCoordinateMapper->MapCameraPointToColorSpace(poi1,cpoi1);
//            cout<<cpoi1->X<<' '<<cpoi1->Y<<endl;
//            sensor->Close();
//        }



//        for (int j = 0; j < _countof(joints); ++j)
//        {
//            //将关节点坐标从摄像机坐标系（-1~1）转到深度坐标系（424*512）
//            m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
//        }










    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    w.setWindowTitle(QString::fromLocal8Bit("关节测量"));
    //w.setWindowState(Qt::WindowFullScreen);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect cur = desktop->availableGeometry(desktop->primaryScreen());
    //w.setGeometry(cur);
    w.setFixedSize(cur.width(), cur.height());
    w.setWindowFlags(w.windowFlags() ^ Qt::WindowMaximizeButtonHint);
    w.show();
    w.reshapeButtons();
    return a.exec();

}
