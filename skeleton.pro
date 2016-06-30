#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T20:30:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skeleton
TEMPLATE = app

INCLUDEPATH += $$quote(C:\software\coding toools\opencv\build\include)
INCLUDEPATH += $$quote(C:\software\coding toools\opencv\build\include\opencv)
INCLUDEPATH += $$quote(C:\software\coding toools\opencv\build\include\opencv2)
INCLUDEPATH += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\inc)
INCLUDEPATH += C:\software\codingtools\qwt-6.1.2\src
INCLUDEPATH += C:\software\codingtools\boost\boost_1_61_0


#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_core2410d.lib)
#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_highgui2410d.lib)
#LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_imgproc2410d.lib)


LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_core2410.lib)
LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_highgui2410.lib)
LIBS += $$quote(C:\software\coding toools\opencv\build\x86\vc12\lib\opencv_imgproc2410.lib)
LIBS += C:\software\codingtools\qwt-6.1.2\lib\qwt.lib
LIBS += C:\software\codingtools\boost\boost_1_61_0\bin\vc12_x86\lib\libboost_serialization-vc120-mt-1_61.lib
LIBS += C:\software\codingtools\boost\boost_1_61_0\bin\vc12_x86\lib\libboost_serialization-vc120-mt-gd-1_61.lib

LIBS += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x86\Kinect20.lib)


SOURCES += main.cpp\
        mainwindow.cpp \
    myKinect.cpp \
    fillinfo.cpp \
    mylistitem.cpp \
    anglemanager.cpp \
    bodyangle.cpp

HEADERS  += mainwindow.h \
    myKinect.h \
    fillinfo.h \
    mylistitem.h \
    anglemanager.h \
    bodyangle.h

FORMS    += mainwindow.ui \
            fillinfo.ui \
            seeangle.ui \
    mylistitem.ui

include(QRCode/QRCode.pri)
