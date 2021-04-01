/********************************************************************************
** Form generated from reading UI file 'musicwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSICWIDGET_H
#define UI_MUSICWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MusicWidget
{
public:
    QPushButton *pb_AddMusic;
    QPushButton *pb_DeleteMusic;
    QListWidget *lw_musicList;
    QGroupBox *gb_container;
    QLabel *lb_musicTime;
    QSlider *slider_MusicProcess;
    QPushButton *pb_volumn;
    QSlider *slider_volumn;
    QPushButton *pb_next;
    QPushButton *pb_play;
    QPushButton *pb_pre;
    QLabel *lb_volumn;
    QLabel *lb_CurrentMusicName;
    QListWidget *lw_Lyric;
    QPushButton *pb_search;
    QLineEdit *le_search;
    QLabel *label;
    QPushButton *pb_close;
    QPushButton *pb_min;

    void setupUi(QWidget *MusicWidget)
    {
        if (MusicWidget->objectName().isEmpty())
            MusicWidget->setObjectName(QStringLiteral("MusicWidget"));
        MusicWidget->resize(800, 600);
        MusicWidget->setMinimumSize(QSize(800, 530));
        MusicWidget->setMaximumSize(QSize(800, 600));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        MusicWidget->setFont(font);
        pb_AddMusic = new QPushButton(MusicWidget);
        pb_AddMusic->setObjectName(QStringLiteral("pb_AddMusic"));
        pb_AddMusic->setGeometry(QRect(20, 30, 111, 28));
        pb_DeleteMusic = new QPushButton(MusicWidget);
        pb_DeleteMusic->setObjectName(QStringLiteral("pb_DeleteMusic"));
        pb_DeleteMusic->setGeometry(QRect(130, 30, 111, 28));
        lw_musicList = new QListWidget(MusicWidget);
        lw_musicList->setObjectName(QStringLiteral("lw_musicList"));
        lw_musicList->setGeometry(QRect(20, 60, 221, 441));
        gb_container = new QGroupBox(MusicWidget);
        gb_container->setObjectName(QStringLiteral("gb_container"));
        gb_container->setGeometry(QRect(0, 510, 801, 91));
        lb_musicTime = new QLabel(gb_container);
        lb_musicTime->setObjectName(QStringLiteral("lb_musicTime"));
        lb_musicTime->setGeometry(QRect(20, 10, 121, 21));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        lb_musicTime->setFont(font1);
        lb_musicTime->setAutoFillBackground(false);
        lb_musicTime->setStyleSheet(QStringLiteral(""));
        slider_MusicProcess = new QSlider(gb_container);
        slider_MusicProcess->setObjectName(QStringLiteral("slider_MusicProcess"));
        slider_MusicProcess->setGeometry(QRect(150, 10, 331, 21));
        slider_MusicProcess->setAutoFillBackground(false);
        slider_MusicProcess->setMaximum(100);
        slider_MusicProcess->setOrientation(Qt::Horizontal);
        pb_volumn = new QPushButton(gb_container);
        pb_volumn->setObjectName(QStringLiteral("pb_volumn"));
        pb_volumn->setGeometry(QRect(550, 10, 22, 22));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/voice_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_volumn->setIcon(icon);
        pb_volumn->setFlat(true);
        slider_volumn = new QSlider(gb_container);
        slider_volumn->setObjectName(QStringLiteral("slider_volumn"));
        slider_volumn->setGeometry(QRect(629, 10, 141, 22));
        slider_volumn->setAutoFillBackground(false);
        slider_volumn->setStyleSheet(QStringLiteral(""));
        slider_volumn->setOrientation(Qt::Horizontal);
        pb_next = new QPushButton(gb_container);
        pb_next->setObjectName(QStringLiteral("pb_next"));
        pb_next->setGeometry(QRect(140, 40, 40, 40));
        pb_next->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/bfzn_002.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_next->setIcon(icon1);
        pb_next->setIconSize(QSize(40, 40));
        pb_next->setFlat(true);
        pb_play = new QPushButton(gb_container);
        pb_play->setObjectName(QStringLiteral("pb_play"));
        pb_play->setGeometry(QRect(80, 40, 40, 40));
        pb_play->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/bfzn_004.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_play->setIcon(icon2);
        pb_play->setIconSize(QSize(40, 40));
        pb_play->setFlat(true);
        pb_pre = new QPushButton(gb_container);
        pb_pre->setObjectName(QStringLiteral("pb_pre"));
        pb_pre->setGeometry(QRect(20, 40, 40, 40));
        pb_pre->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/bfzn_001.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_pre->setIcon(icon3);
        pb_pre->setIconSize(QSize(40, 40));
        pb_pre->setFlat(true);
        lb_volumn = new QLabel(gb_container);
        lb_volumn->setObjectName(QStringLiteral("lb_volumn"));
        lb_volumn->setGeometry(QRect(580, 10, 41, 22));
        lb_CurrentMusicName = new QLabel(gb_container);
        lb_CurrentMusicName->setObjectName(QStringLiteral("lb_CurrentMusicName"));
        lb_CurrentMusicName->setGeometry(QRect(230, 40, 541, 40));
        lb_CurrentMusicName->setAlignment(Qt::AlignCenter);
        lw_Lyric = new QListWidget(MusicWidget);
        lw_Lyric->setObjectName(QStringLiteral("lw_Lyric"));
        lw_Lyric->setGeometry(QRect(250, 60, 541, 441));
        pb_search = new QPushButton(MusicWidget);
        pb_search->setObjectName(QStringLiteral("pb_search"));
        pb_search->setGeometry(QRect(250, 30, 30, 28));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_search->setIcon(icon4);
        pb_search->setIconSize(QSize(30, 30));
        pb_search->setFlat(false);
        le_search = new QLineEdit(MusicWidget);
        le_search->setObjectName(QStringLiteral("le_search"));
        le_search->setGeometry(QRect(279, 30, 512, 28));
        le_search->setClearButtonEnabled(true);
        label = new QLabel(MusicWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 30, 30));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label->setScaledContents(true);
        pb_close = new QPushButton(MusicWidget);
        pb_close->setObjectName(QStringLiteral("pb_close"));
        pb_close->setGeometry(QRect(760, 2, 26, 26));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(16);
        pb_close->setFont(font2);
        pb_close->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        pb_close->setFlat(true);
        pb_min = new QPushButton(MusicWidget);
        pb_min->setObjectName(QStringLiteral("pb_min"));
        pb_min->setGeometry(QRect(730, 2, 26, 26));
        QFont font3;
        font3.setPointSize(20);
        font3.setBold(true);
        font3.setWeight(75);
        pb_min->setFont(font3);
        pb_min->setFlat(true);

        retranslateUi(MusicWidget);

        QMetaObject::connectSlotsByName(MusicWidget);
    } // setupUi

    void retranslateUi(QWidget *MusicWidget)
    {
        MusicWidget->setWindowTitle(QApplication::translate("MusicWidget", "MusicWidget", Q_NULLPTR));
        pb_AddMusic->setText(QApplication::translate("MusicWidget", "\346\267\273\345\212\240\346\255\214\346\233\262", Q_NULLPTR));
        pb_DeleteMusic->setText(QApplication::translate("MusicWidget", "\345\210\240\351\231\244\346\255\214\346\233\262", Q_NULLPTR));
        gb_container->setTitle(QString());
        lb_musicTime->setText(QApplication::translate("MusicWidget", "00:00/00:00", Q_NULLPTR));
        pb_volumn->setText(QString());
        pb_next->setText(QString());
        pb_play->setText(QString());
        pb_pre->setText(QString());
        lb_volumn->setText(QApplication::translate("MusicWidget", "30%", Q_NULLPTR));
        lb_CurrentMusicName->setText(QString());
        pb_search->setText(QString());
        le_search->setText(QString());
        label->setText(QString());
        pb_close->setText(QApplication::translate("MusicWidget", "x", Q_NULLPTR));
        pb_min->setText(QApplication::translate("MusicWidget", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MusicWidget: public Ui_MusicWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSICWIDGET_H
