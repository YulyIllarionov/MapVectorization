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
    base_types.cpp \
    imageviewer.cpp \
    mypixmapitem.cpp \
    layersviewer.cpp \
    layerconfiguredialog.cpp

HEADERS  += mainwindow.h \
    app/sdk_const.h \
    base_types.h \
    imageviewer.h \
    mypixmapitem.h \
    layersviewer.h \
    layerconfiguredialog.h

FORMS    += mainwindow.ui \
    layersviewer.ui \
    layerconfiguredialog.ui

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

RESOURCES += \
    res.qrc
