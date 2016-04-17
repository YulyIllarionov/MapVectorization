/********************************************************************************
** Form generated from reading UI file 'layerconfiguredialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERCONFIGUREDIALOG_H
#define UI_LAYERCONFIGUREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LayerConfigureDialog
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QPushButton *Pipette;
    QSpacerItem *verticalSpacer;
    QFrame *SapleFrame;
    QPushButton *AddColor;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QFrame *LeftSample;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QFrame *RightSample;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *LayerConfigureDialog)
    {
        if (LayerConfigureDialog->objectName().isEmpty())
            LayerConfigureDialog->setObjectName(QStringLiteral("LayerConfigureDialog"));
        LayerConfigureDialog->resize(204, 306);
        verticalLayout_4 = new QVBoxLayout(LayerConfigureDialog);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Pipette = new QPushButton(LayerConfigureDialog);
        Pipette->setObjectName(QStringLiteral("Pipette"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Pipette->sizePolicy().hasHeightForWidth());
        Pipette->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/icons/pipette.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Pipette->setIcon(icon);
        Pipette->setCheckable(true);

        verticalLayout->addWidget(Pipette);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout);

        SapleFrame = new QFrame(LayerConfigureDialog);
        SapleFrame->setObjectName(QStringLiteral("SapleFrame"));
        SapleFrame->setAutoFillBackground(true);
        SapleFrame->setFrameShape(QFrame::StyledPanel);
        SapleFrame->setFrameShadow(QFrame::Plain);

        horizontalLayout_3->addWidget(SapleFrame);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 10);

        verticalLayout_4->addLayout(horizontalLayout_3);

        AddColor = new QPushButton(LayerConfigureDialog);
        AddColor->setObjectName(QStringLiteral("AddColor"));

        verticalLayout_4->addWidget(AddColor);

        groupBox = new QGroupBox(LayerConfigureDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label);

        LeftSample = new QFrame(groupBox);
        LeftSample->setObjectName(QStringLiteral("LeftSample"));
        LeftSample->setAutoFillBackground(true);
        LeftSample->setFrameShape(QFrame::StyledPanel);
        LeftSample->setFrameShadow(QFrame::Plain);

        verticalLayout_3->addWidget(LeftSample);


        horizontalLayout_4->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(label_2);

        RightSample = new QFrame(groupBox);
        RightSample->setObjectName(QStringLiteral("RightSample"));
        RightSample->setAutoFillBackground(true);
        RightSample->setFrameShape(QFrame::StyledPanel);
        RightSample->setFrameShadow(QFrame::Plain);

        verticalLayout_2->addWidget(RightSample);


        horizontalLayout_4->addLayout(verticalLayout_2);


        verticalLayout_4->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(LayerConfigureDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label_3);

        lineEdit = new QLineEdit(LayerConfigureDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(LayerConfigureDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(label_4);

        comboBox = new QComboBox(LayerConfigureDialog);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);


        verticalLayout_4->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(LayerConfigureDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_4->addWidget(buttonBox);

        verticalLayout_4->setStretch(0, 3);
        verticalLayout_4->setStretch(1, 1);
        verticalLayout_4->setStretch(2, 5);
        verticalLayout_4->setStretch(3, 1);
        verticalLayout_4->setStretch(4, 1);
        verticalLayout_4->setStretch(5, 1);

        retranslateUi(LayerConfigureDialog);

        QMetaObject::connectSlotsByName(LayerConfigureDialog);
    } // setupUi

    void retranslateUi(QWidget *LayerConfigureDialog)
    {
        LayerConfigureDialog->setWindowTitle(QApplication::translate("LayerConfigureDialog", "Dialog", 0));
        Pipette->setText(QString());
        AddColor->setText(QApplication::translate("LayerConfigureDialog", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\206\320\262\320\265\321\202", 0));
        groupBox->setTitle(QApplication::translate("LayerConfigureDialog", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \321\206\320\262\320\265\321\202\320\276\320\262", 0));
        label->setText(QApplication::translate("LayerConfigureDialog", "\320\236\321\202:", 0));
        label_2->setText(QApplication::translate("LayerConfigureDialog", "\320\224\320\276:", 0));
        label_3->setText(QApplication::translate("LayerConfigureDialog", "\320\230\320\274\321\217 \321\201\320\273\320\276\321\217:", 0));
        label_4->setText(QApplication::translate("LayerConfigureDialog", "\320\242\320\270\320\277 \321\201\320\273\320\276\321\217:", 0));
    } // retranslateUi

};

namespace Ui {
    class LayerConfigureDialog: public Ui_LayerConfigureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERCONFIGUREDIALOG_H
