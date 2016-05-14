/********************************************************************************
** Form generated from reading UI file 'layersviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERSVIEWER_H
#define UI_LAYERSVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LayersViewer
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *Add;
    QPushButton *Remove;
    QPushButton *SavePngButton;

    void setupUi(QWidget *LayersViewer)
    {
        if (LayersViewer->objectName().isEmpty())
            LayersViewer->setObjectName(QStringLiteral("LayersViewer"));
        LayersViewer->resize(198, 165);
        verticalLayout = new QVBoxLayout(LayersViewer);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(LayersViewer);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Add = new QPushButton(LayersViewer);
        Add->setObjectName(QStringLiteral("Add"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/mapAppMain/resources/icons/add.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Add->setIcon(icon);

        horizontalLayout->addWidget(Add);

        Remove = new QPushButton(LayersViewer);
        Remove->setObjectName(QStringLiteral("Remove"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/mapAppMain/resources/icons/remove.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Remove->setIcon(icon1);

        horizontalLayout->addWidget(Remove);

        SavePngButton = new QPushButton(LayersViewer);
        SavePngButton->setObjectName(QStringLiteral("SavePngButton"));

        horizontalLayout->addWidget(SavePngButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LayersViewer);

        QMetaObject::connectSlotsByName(LayersViewer);
    } // setupUi

    void retranslateUi(QWidget *LayersViewer)
    {
        LayersViewer->setWindowTitle(QApplication::translate("LayersViewer", "Form", 0));
        Add->setText(QString());
        Remove->setText(QString());
        SavePngButton->setText(QApplication::translate("LayersViewer", "Save as PNG", 0));
    } // retranslateUi

};

namespace Ui {
    class LayersViewer: public Ui_LayersViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERSVIEWER_H
