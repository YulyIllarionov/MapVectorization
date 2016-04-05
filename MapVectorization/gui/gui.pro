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
INCLUDEPATH += ..\3rdparty\opencv\include
LIBS += ..\3rdparty\opencv\bin\opencv_world310.dll
########################################################
