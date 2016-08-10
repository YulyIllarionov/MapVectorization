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
    QPushButton *SplitButton;
    QPushButton *RecognitionButton;
    QPushButton *SavePngButton;

    void setupUi(QWidget *LayersViewer)
    {
        if (LayersViewer->objectName().isEmpty())
            LayersViewer->setObjectName(QStringLiteral("LayersViewer"));
        LayersViewer->resize(419, 238);
        verticalLayout = new QVBoxLayout(LayersViewer);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(LayersViewer);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Add = new QPushButton(LayersViewer);
        Add->setObjectName(QStringLiteral("Add"));

        horizontalLayout->addWidget(Add);

        Remove = new QPushButton(LayersViewer);
        Remove->setObjectName(QStringLiteral("Remove"));

        horizontalLayout->addWidget(Remove);

        SplitButton = new QPushButton(LayersViewer);
        SplitButton->setObjectName(QStringLiteral("SplitButton"));
        SplitButton->setEnabled(false);

        horizontalLayout->addWidget(SplitButton);

        RecognitionButton = new QPushButton(LayersViewer);
        RecognitionButton->setObjectName(QStringLiteral("RecognitionButton"));
        RecognitionButton->setEnabled(false);

        horizontalLayout->addWidget(RecognitionButton);

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
        Add->setText(QApplication::translate("LayersViewer", "\320\224\320\276\320\261.", 0));
        Remove->setText(QApplication::translate("LayersViewer", "\320\243\320\264\320\260\320\273.", 0));
        SplitButton->setText(QApplication::translate("LayersViewer", "Split", 0));
        RecognitionButton->setText(QApplication::translate("LayersViewer", "Recog Text", 0));
        SavePngButton->setText(QApplication::translate("LayersViewer", "Save as PNG", 0));
    } // retranslateUi

};

namespace Ui {
    class LayersViewer: public Ui_LayersViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERSVIEWER_H
