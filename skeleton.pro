#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T20:30:13
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skeleton
TEMPLATE = app

INCLUDEPATH += $$quote(D:\opencv\build\include)
INCLUDEPATH += $$quote(D:\opencv\build\include\opencv)
INCLUDEPATH += $$quote(D:\opencv\build\include\opencv2)
INCLUDEPATH += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\inc)
INCLUDEPATH += D:\Qwt-6.1.2\include
INCLUDEPATH += D:\boost_1_61_0


#LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_core2410d.lib)
#LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_highgui2410d.lib)
#LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_imgproc2410d.lib)
#LIBS += D:\Qwt-6.1.2\lib\qwtd.lib
#LIBS += D:\boost_1_61_0\stage\lib\libboost_serialization-vc140-mt-gd-1_61.lib


LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_core2410.lib)
LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_highgui2410.lib)
LIBS += $$quote(D:\opencv\build\x64\vc12\lib\opencv_imgproc2410.lib)
LIBS += D:\Qwt-6.1.2\lib\qwt.lib
LIBS += D:\boost_1_61_0\stage\lib\libboost_serialization-vc140-mt-1_61.lib


LIBS += $$quote(C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x64\Kinect20.lib)


SOURCES += main.cpp\
        mainwindow.cpp \
    myKinect.cpp \
    mylistitem.cpp \
    anglemanager.cpp \
    bodyangle.cpp \
    mylistheaditem.cpp \
    database.cpp \
    kneescorewizard.cpp \
    hipscorewizard.cpp \
    querywidget.cpp \
    mysqltablemodel.cpp \
    infodetailview.cpp

HEADERS  += mainwindow.h \
    myKinect.h \
    mylistitem.h \
    anglemanager.h \
    bodyangle.h \
    mylistheaditem.h \
    database.h \
    kneescorewizard.h \
    hipscorewizard.h \
    querywidget.h \
    mysqltablemodel.h \
    infodetailview.h

FORMS    += mainwindow.ui \
    mylistitem.ui \
    mylistheaditem.ui \
    querywidget.ui \
    infodetailview.ui

include(QRCode/QRCode.pri)

RESOURCES += \
    source.qrc
