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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LayerConfigureDialog
{
public:
    QVBoxLayout *verticalLayout_5;
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
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *PenButton;
    QPushButton *EraseButton;
    QSpinBox *RadiusSpinBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *Name;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *LayerConfigureDialog)
    {
        if (LayerConfigureDialog->objectName().isEmpty())
            LayerConfigureDialog->setObjectName(QStringLiteral("LayerConfigureDialog"));
        LayerConfigureDialog->resize(253, 494);
        verticalLayout_5 = new QVBoxLayout(LayerConfigureDialog);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
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
        icon.addFile(QStringLiteral(":/mapAppMain/resources/icons/pipette.ico"), QSize(), QIcon::Normal, QIcon::Off);
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

        verticalLayout_5->addLayout(horizontalLayout_3);

        AddColor = new QPushButton(LayerConfigureDialog);
        AddColor->setObjectName(QStringLiteral("AddColor"));
        AddColor->setEnabled(false);

        verticalLayout_5->addWidget(AddColor);

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


        verticalLayout_5->addWidget(groupBox);

        groupBox_3 = new QGroupBox(LayerConfigureDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_3);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        PenButton = new QPushButton(groupBox_3);
        PenButton->setObjectName(QStringLiteral("PenButton"));
        PenButton->setCheckable(true);

        horizontalLayout_2->addWidget(PenButton);

        EraseButton = new QPushButton(groupBox_3);
        EraseButton->setObjectName(QStringLiteral("EraseButton"));
        EraseButton->setCheckable(true);

        horizontalLayout_2->addWidget(EraseButton);

        RadiusSpinBox = new QSpinBox(groupBox_3);
        RadiusSpinBox->setObjectName(QStringLiteral("RadiusSpinBox"));

        horizontalLayout_2->addWidget(RadiusSpinBox);


        verticalLayout_5->addWidget(groupBox_3);

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

        Name = new QLineEdit(LayerConfigureDialog);
        Name->setObjectName(QStringLiteral("Name"));

        horizontalLayout->addWidget(Name);


        verticalLayout_5->addLayout(horizontalLayout);

        groupBox_2 = new QGroupBox(LayerConfigureDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout_4->addWidget(checkBox);

        checkBox_2 = new QCheckBox(groupBox_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout_4->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(groupBox_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        verticalLayout_4->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(groupBox_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        verticalLayout_4->addWidget(checkBox_4);


        verticalLayout_5->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(LayerConfigureDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_5->addWidget(buttonBox);

        verticalLayout_5->setStretch(0, 2);
        verticalLayout_5->setStretch(1, 1);
        verticalLayout_5->setStretch(2, 3);
        verticalLayout_5->setStretch(3, 1);
        verticalLayout_5->setStretch(4, 1);
        verticalLayout_5->setStretch(5, 1);
        verticalLayout_5->setStretch(6, 1);

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
        groupBox_3->setTitle(QApplication::translate("LayerConfigureDialog", "\320\237\320\270\320\272\321\201\320\265\320\273\321\214\320\275\320\276\320\265 \320\264\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265", 0));
        PenButton->setText(QApplication::translate("LayerConfigureDialog", "\320\232\320\260\321\200\320\260\320\275\320\264\320\260\321\210", 0));
        EraseButton->setText(QApplication::translate("LayerConfigureDialog", "\320\233\320\260\321\201\321\202\320\270\320\272", 0));
        label_3->setText(QApplication::translate("LayerConfigureDialog", "\320\230\320\274\321\217 \321\201\320\273\320\276\321\217:", 0));
        groupBox_2->setTitle(QApplication::translate("LayerConfigureDialog", "\320\242\320\270\320\277 \321\201\320\273\320\276\321\217", 0));
        checkBox->setText(QApplication::translate("LayerConfigureDialog", "\320\242\320\265\320\272\321\201\321\202", 0));
        checkBox_2->setText(QApplication::translate("LayerConfigureDialog", "\320\233\320\270\320\275\320\270\320\270", 0));
        checkBox_3->setText(QApplication::translate("LayerConfigureDialog", "\320\236\320\261\320\273\320\260\321\201\321\202\320\270", 0));
        checkBox_4->setText(QApplication::translate("LayerConfigureDialog", "\320\224\321\200\321\203\320\263\320\276\320\265", 0));
    } // retranslateUi

};

namespace Ui {
    class LayerConfigureDialog: public Ui_LayerConfigureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERCONFIGUREDIALOG_H
