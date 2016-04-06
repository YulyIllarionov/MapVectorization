#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T09:07:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageviewerwidget.cpp

HEADERS  += mainwindow.h \
    imageviewerwidget.h

FORMS    += mainwindow.ui \
    imageviewerwidget.ui

# Подключение openCV библиотеки
INCLUDEPATH += C:\opencv\release\install\include
LIBS += C:\opencv\release\bin\libopencv_core2411.dll
LIBS += C:\opencv\release\bin\libopencv_contrib2411.dll
LIBS += C:\opencv\release\bin\libopencv_highgui2411.dll
LIBS += C:\opencv\release\bin\libopencv_imgproc2411.dll
LIBS += C:\opencv\release\bin\libopencv_calib3d2411.dll
LIBS += C:\opencv\release\bin\libopencv_objdetect2411.dll
LIBS += C:\opencv\release\bin\libopencv_ml2411.dll
########################################################
