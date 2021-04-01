#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QJsonObject>
#include<QJsonArray>
#include<QNetworkCookieJar>
#include<QNetworkCookie>
#include<QJsonParseError>

namespace Ui {
class DownloadWidget;
}
#define MAX_ONE_PAGE (20)
class DownloadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWidget(QWidget *parent = 0);
    ~DownloadWidget();
signals:
    void mediaAdd(QString song);
    void nameAdd(QString  name);
    void lrcAdd(QString lrcStr);
    void imgAdd(QString imgStr);
    void SIG_PlayOnline(QString strUrl , QString musicName);
    void SIG_musicDownloaded(QString path );
    void SIG_refreshImg();
public slots:
    void search(QString str);
    void replyFinished(QNetworkReply *reply);        //发送搜索请求完成，接受到信息，然后进行解析
    void replyFinished2(QNetworkReply *reply);       //发送搜索请求完成，接受到信息，然后进行解析         通过歌曲ID搜索
    void replyFinished3(QNetworkReply *reply);
    void replyFinished4(QNetworkReply *reply);
    void parseJson_getAlbumID(QString json);     //解析接收到的歌曲信息，得到歌曲ID
    void parseJson_getplay_url(QString json);        //解析得到歌曲

    void slot_addMedia(QString strMedia);
    void slot_addName(QString strName);
    void slot_addLrc(QString strLrc);
    void slot_addImg(QString strImg);
    QString GetMD5();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_doubleClicked(const QModelIndex &index);
    void slot_MenuAction(QAction*action);
private:
    Ui::DownloadWidget *ui;

    QNetworkAccessManager *network_manager  ;
    QNetworkRequest *network_request  ;				//发送请求一得到AlbumID和FileHash
    QNetworkAccessManager* network_manager2  ;
    QNetworkRequest *network_request2  ;	//发送请求二得到url和歌词等信息
    QNetworkAccessManager* network_manager3  ;
    QNetworkRequest *network_request3  ;	//发送请求三下载歌曲
    QNetworkAccessManager* network_manager4  ;
    QNetworkRequest *network_request4  ;	//发送请求四下载img
    QString album_idStr;
    QString hashStr;
    QNetworkCookieJar * m_cook;

    int page;
    QString m_key;
    QString m_LrcList[MAX_ONE_PAGE];
    int m_LrcListCount;
    QString m_mediaList[MAX_ONE_PAGE];
    int m_MediaListCount;
    QString m_imgList[MAX_ONE_PAGE];
    int m_imgListCount;

};

#endif // DOWNLOADWIDGET_H
