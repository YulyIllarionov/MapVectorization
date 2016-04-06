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
INCLUDEPATH += C:\opencv3.1\release\include
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_core310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_highgui310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_imgproc310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_calib3d310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_objdetect310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_ml310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_features2d310.dll
LIBS += C:\opencv3.1\release\x86\mingw\bin\libopencv_imgcodecs310.dll
########################################################
