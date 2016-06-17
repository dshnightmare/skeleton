#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    w.setWindowTitle(QString::fromLocal8Bit("关节测量"));
    w.show();
    return a.exec();
}
