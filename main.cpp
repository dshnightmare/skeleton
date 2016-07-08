#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>


int main(int argc, char *argv[])
{
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
