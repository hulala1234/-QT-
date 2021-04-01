/********************************************************************************
** Form generated from reading UI file 'downloadwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADWIDGET_H
#define UI_DOWNLOADWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DownloadWidget
{
public:
    QListWidget *listWidget;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *DownloadWidget)
    {
        if (DownloadWidget->objectName().isEmpty())
            DownloadWidget->setObjectName(QStringLiteral("DownloadWidget"));
        DownloadWidget->resize(421, 530);
        DownloadWidget->setMinimumSize(QSize(421, 530));
        DownloadWidget->setMaximumSize(QSize(421, 530));
        listWidget = new QListWidget(DownloadWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(11, 11, 401, 471));
        label = new QLabel(DownloadWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(170, 490, 72, 30));
        label->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(DownloadWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 490, 93, 30));
        pushButton_2 = new QPushButton(DownloadWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(270, 490, 93, 30));

        retranslateUi(DownloadWidget);

        QMetaObject::connectSlotsByName(DownloadWidget);
    } // setupUi

    void retranslateUi(QWidget *DownloadWidget)
    {
        DownloadWidget->setWindowTitle(QApplication::translate("DownloadWidget", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("DownloadWidget", "\347\254\2541\351\241\265", Q_NULLPTR));
        pushButton->setText(QApplication::translate("DownloadWidget", "\344\270\212\344\270\200\351\241\265", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("DownloadWidget", "\344\270\213\344\270\200\351\241\265", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DownloadWidget: public Ui_DownloadWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADWIDGET_H
