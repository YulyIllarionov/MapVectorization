/********************************************************************************
** Form generated from reading UI file 'mapappmain.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPAPPMAIN_H
#define UI_MAPAPPMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mapAppMainClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mapAppMainClass)
    {
        if (mapAppMainClass->objectName().isEmpty())
            mapAppMainClass->setObjectName(QStringLiteral("mapAppMainClass"));
        mapAppMainClass->resize(600, 400);
        centralWidget = new QWidget(mapAppMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mapAppMainClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mapAppMainClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        mapAppMainClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mapAppMainClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mapAppMainClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mapAppMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mapAppMainClass->setStatusBar(statusBar);

        retranslateUi(mapAppMainClass);

        QMetaObject::connectSlotsByName(mapAppMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *mapAppMainClass)
    {
        mapAppMainClass->setWindowTitle(QApplication::translate("mapAppMainClass", "Map vectorization", 0));
    } // retranslateUi

};

namespace Ui {
    class mapAppMainClass: public Ui_mapAppMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPAPPMAIN_H
