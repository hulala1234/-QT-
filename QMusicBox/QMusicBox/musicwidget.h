#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include<QList>
#include<QMediaPlayer>
#include<downloadwidget.h>
#include"CustomWidget.h"
#include<QSystemTrayIcon>
#include"csqlite.h"
#define DEF_MAX_MUSIC_COUNT  (200)
#define DEF_MAX_LYRIC_COUNT  (500)

namespace Ui {
class MusicWidget;
}

class MusicWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit MusicWidget(QWidget *parent = 0);
    ~MusicWidget();

public slots:

    void on_pb_close_clicked();

    void on_pb_min_clicked();

    void on_pb_AddMusic_clicked();

    void on_lw_musicList_doubleClicked(const QModelIndex &index);

    void slot_setWindowBackGroundImage();
    void slot_MusicPositionChanged(qint64 val);
    void on_pb_play_clicked();

    void on_pb_DeleteMusic_clicked();

    void on_slider_MusicProcess_valueChanged(int value);

    void on_pb_next_clicked();

    void on_pb_pre_clicked();

    void on_pb_search_clicked();

    static MusicWidget* GetMainWnd()
    {
        return m_p;
    }
    void slot_PlayOnline(QString strUrl , QString musicName);
    void slot_musicDownloaded(QString path );
private slots:
    void on_pb_volumn_clicked();

    void on_slider_volumn_valueChanged(int value);
    void loadSqlAndSetMusicList();
    void on_slider_MusicProcess_sliderPressed();

    void on_slider_MusicProcess_actionTriggered(int action);

private:
    Ui::MusicWidget *ui;
    QString m_MusicList[DEF_MAX_MUSIC_COUNT];
    QString m_musicLyric[DEF_MAX_LYRIC_COUNT];
    quint32 m_MusicListCount;
    QMediaPlayer *m_Player;
    quint32 m_VolumnNow;
    quint32 m_VolumnLast;
    QString m_CurrentMusicName;
    bool m_musicStartFlag ;
    bool m_musicPositionChangeFlag;
    int m_musicLrcLineCount;
    int lrcAddLineCount;
    DownloadWidget * m_downloadWidget;

    static MusicWidget* m_p;
    friend class DownloadWidget;
    bool m_voiceOpenFlag;
    QSystemTrayIcon * mSysTrayIcon;
    CSqlite *m_sql;
};

#endif // MUSICWIDGET_H
