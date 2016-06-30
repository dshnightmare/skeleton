#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T20:30:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skeleton
TEMPLATE = app

INCLUDEPATH += $$quote(d:\opencv\build\include)
INCLUDEPATH += $$quote(d:\opencv\build\include\opencv)
INCLUDEPATH += $$quote(d:\opencv\build\include\opencv2)
INCLUDEPATH += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\inc)
INCLUDEPATH += d:\qwt-6.1.2\src


#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_core2410d.lib)
#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_highgui2410d.lib)
#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_imgproc2410d.lib)


LIBS += $$quote(d:\opencv\build\x64\vc12\lib\opencv_core2410.lib)
LIBS += $$quote(d:\opencv\build\x64\vc12\lib\opencv_highgui2410.lib)
LIBS += $$quote(d:\opencv\build\x64\vc12\lib\opencv_imgproc2410.lib)
LIBS += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x64\Kinect20.lib)
LIBS += d:\qwt-6.1.2\lib\qwt.lib


SOURCES += main.cpp\
        mainwindow.cpp \
    myKinect.cpp \
    fillinfo.cpp \
    mylistitem.cpp \
    anglemanager.cpp

HEADERS  += mainwindow.h \
    myKinect.h \
    fillinfo.h \
    mylistitem.h \
    anglemanager.h

FORMS    += mainwindow.ui \
            fillinfo.ui \
            seeangle.ui \
    mylistitem.ui

include(QRCode/QRCode.pri)
