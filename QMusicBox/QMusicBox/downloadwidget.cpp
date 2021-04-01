#include "downloadwidget.h"
#include "ui_downloadwidget.h"
#include"musicwidget.h"
#include "ui_musicwidget.h"
#include<QFile>
#include<QMenu>
#include<QEventLoop>
#include<QCryptographicHash>
#include<QTime>
#include<QDir>

DownloadWidget::DownloadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("在线音乐");
    m_cook = new QNetworkCookieJar;


    QTime time = QTime::currentTime();
    qsrand( time.msec()+time.second()*1000);

    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息

    network_manager3 = new QNetworkAccessManager();
    network_request3 = new QNetworkRequest();			//发送请求三下载歌曲

    network_manager4 = new QNetworkAccessManager();
    network_request4 = new QNetworkRequest();			//发送请求四下载img

    connect(network_manager2, &QNetworkAccessManager::finished, this, &DownloadWidget::replyFinished2);
    connect(network_manager, &QNetworkAccessManager::finished, this, &DownloadWidget::replyFinished);

    connect(network_manager3, &QNetworkAccessManager::finished, this, &DownloadWidget::replyFinished3);

    connect(network_manager4, &QNetworkAccessManager::finished, this, &DownloadWidget::replyFinished4);

    connect(this,SIGNAL(mediaAdd(QString )),this,SLOT(slot_addMedia(QString)));
    connect(this,SIGNAL(nameAdd(QString  )),this,SLOT(slot_addName(QString)));
    connect(this,SIGNAL(lrcAdd(QString )),this,SLOT(slot_addLrc(QString)));
    connect(this,SIGNAL(imgAdd(QString )),this,SLOT(slot_addImg(QString)));
}

DownloadWidget::~DownloadWidget()
{
    delete ui;
}

void DownloadWidget::search(QString str)
{
    if( m_key.compare(str) == 0 ) return ;
    ui->listWidget->clear();
    m_key = str;
    page = 0;

    m_LrcListCount = 0;
    m_MediaListCount = 0;
    m_imgListCount = 0;
    //发送歌曲搜索请求
    QString KGAPISTR1 = QString("http://songsearch.kugou.com/song_search_v2?keyword="
                        "%1&page=&pagesize=&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0").arg(str);
//    QString KGAPISTR1 ="https://v1.itooi.cn/kugou/url?id=ac9b095dcf364f751bced2e148638afa&quality=128";

    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(*network_request);

}


void DownloadWidget::replyFinished(QNetworkReply *reply)        //发送搜索请求完成，接受到信息，然后进行解析
{
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
//        qDebug()<<bytes;
        QString result(bytes);  //转化为字符串
//         QString strUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
//        qDebug()<<strUrl;
        parseJson_getAlbumID(result);  //自定义方法，解析歌曲数据

    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}
void DownloadWidget::replyFinished2(QNetworkReply *reply)       //发送搜索请求完成，接受到信息，然后进行解析         通过歌曲ID搜索
{
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
         qDebug()<< result;
        parseJson_getplay_url(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}
void DownloadWidget::replyFinished3(QNetworkReply *reply)       //发送搜索请求完成，接受到信息，然后进行解析         通过歌曲ID搜索
{
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
//        QByteArray bytes = reply->readAll();  //获取字节
//        QString result(bytes);  //转化为字符串
//     //    qDebug()<< result;
//        //保存文件
        QString FileName = ui->listWidget->currentItem()->text()+".mp3";
        QString FilePath = QDir::currentPath()+"/music/"+FileName;
        QFile file( FilePath );
        if(!file.open(QIODevice::WriteOnly))
            return  ;
        file.write(reply->readAll());
        file.close();

        emit SIG_musicDownloaded( FilePath    );
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}
void DownloadWidget::replyFinished4(QNetworkReply *reply)       //发送搜索请求完成，接受到信息，然后进行解析         通过歌曲ID搜索
{
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
//        //保存文件
        QString FileName = ui->listWidget->currentItem()->text()+".jpg";
        QString FilePath = QDir::currentPath()+"/images/"+FileName;
        QFile file( FilePath );
        if(!file.open(QIODevice::WriteOnly))
            return  ;
        file.write(reply->readAll());
        file.close();

        emit SIG_refreshImg();
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}

void DownloadWidget::parseJson_getAlbumID(QString json)     //解析接收到的歌曲信息，得到歌曲ID
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if(rootObj.contains("data"))
            {
                QJsonValue valuedata = rootObj.value("data");
                if(valuedata.isObject())
                {
                    QJsonObject valuedataObject = valuedata.toObject();
                    if(valuedataObject.contains("lists"))
                    {
                        QJsonValue valueArray = valuedataObject.value("lists");
                        if (valueArray.isArray())
                        {
                            QJsonArray array = valueArray.toArray();
                            int size = array.size();
                            for(int i = 0;i < /*1*/size;i++)
                            {
                                QJsonValue value = array.at(i);
                                if(value.isObject())
                                {
                                    QJsonObject object = value.toObject();
                                    if(object.contains("AlbumID"))
                                    {
                                        QJsonValue AlbumID_value = object.take("AlbumID");
                                        if(AlbumID_value.isString())
                                        {
                                            album_idStr = AlbumID_value.toString();             //歌曲ID信息
                                        }
                                    }
                                    if(object.contains("FileHash"))
                                    {
                                        QJsonValue FileHash_value = object.take("FileHash");
                                        if(FileHash_value.isString())
                                        {
                                            hashStr = FileHash_value.toString();                //hash
                                        }
                                    }

                                    //通过歌曲ID发送请求，得到歌曲url和歌词
                                    QString KGAPISTR1 = QString("http://www.kugou.com/yy/index.php?r=play/getdata&hash=%1")
                                            .arg(hashStr)/*.arg(album_idStr)*/;


//                                    qDebug()<<KGAPISTR1;
                                    // kg_mid 的值是 [a-z0-9]{4} 的MD5 并且每次请求都不一样
                                    QNetworkCookie cookie("kg_mid", QString( GetMD5() ).toUtf8());
                                    QList<QNetworkCookie> cookies;
                                    cookies.append(cookie);
                                    m_cook->setCookiesFromUrl(cookies, QUrl("http://www.kugou.com/yy/index.php"));
                                    network_manager2->setCookieJar(m_cook);

                                    network_request2->setUrl(QUrl(KGAPISTR1));
                                    network_manager2->get(*network_request2);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        qDebug()<<json_error.errorString();
    }
}
//todo
void DownloadWidget::parseJson_getplay_url(QString json)        //解析得到歌曲
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if(rootObj.contains("data"))
            {
                QJsonValue valuedata = rootObj.value("data");
                if(valuedata.isObject())
                {
                    QJsonObject valuedataObject = valuedata.toObject();
                    QString play_urlStr("");
                    QString img_urlStr("");
                    if(valuedataObject.contains("play_url"))
                    {
                        QJsonValue play_url_value = valuedataObject.take("play_url");
                        if(play_url_value.isString())
                        {
                            play_urlStr = play_url_value.toString();                    //歌曲的url

                            if(play_urlStr!="")
                            {
                                emit mediaAdd(play_urlStr);
                            }

                        }
                    }
                    if(valuedataObject.contains("img"))
                    {
                        QJsonValue img_url_value = valuedataObject.take("img");
                        if(img_url_value.isString())
                        {
                            img_urlStr = img_url_value.toString();                    //歌曲的url

                            if(img_urlStr!="")
                            {
                                emit imgAdd(img_urlStr);
                            }

                        }
                    }
                    if(valuedataObject.contains("audio_name"))
                    {
                        QJsonValue play_name_value = valuedataObject.take("audio_name");
                        if(play_name_value.isString())
                        {
                            QString play_name = play_name_value.toString();                //歌曲名字
                            if(play_urlStr!="")
                            {
                                emit nameAdd(play_name);
                            }

                        }
                    }
                    if(valuedataObject.contains("lyrics"))                                  //lrc
                    {
                        QJsonValue play_url_value = valuedataObject.take("lyrics");
                        if(play_url_value.isString())
                        {
                            QString play_lrcStr = play_url_value.toString();
                            if(play_urlStr!="")
                            {
                                if(play_lrcStr != "")
                                {
                                    emit lrcAdd(play_lrcStr);
                                }
                                else
                                {
                                    emit lrcAdd("没有歌词");
                                }
                            }

                        }
                    }
                }
            }
        }
    }
}

void DownloadWidget::slot_addMedia(QString strMedia)
{
    qDebug()<<"strMedia"<<strMedia;
    m_mediaList[ m_MediaListCount++ ] = strMedia;
}

void DownloadWidget::slot_addName(QString strName)
{
    qDebug()<<"strName"<<strName;
    ui->listWidget->addItem(strName);
}

void DownloadWidget::slot_addLrc(QString strLrc)
{
   // qDebug()<<"strLrc"<<strLrc;
    m_LrcList[ m_LrcListCount++ ] = strLrc;
}

void DownloadWidget::slot_addImg(QString strImg)
{
    m_imgList[ m_imgListCount++] = strImg;
}

void DownloadWidget::on_listWidget_clicked(const QModelIndex &index)
{
    //添加菜单
    QMenu *menu = new QMenu(this);
    menu->addAction("播放音乐");
    menu->addAction("下载到本地");
    connect(menu,SIGNAL(triggered(QAction*)),this,SLOT(slot_MenuAction(QAction*)));

    menu->exec(QCursor::pos());

}

void DownloadWidget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int idx = ui->listWidget->currentIndex().row();

   qDebug()<< ui->listWidget->currentItem()->text();

    QFile lrcFile("lrc/"+ui->listWidget->currentItem()->text()+".txt");

    if( lrcFile.open(QIODevice::WriteOnly))
    {
        QTextStream lrcFileStream(&lrcFile);
        lrcFileStream.setCodec("UTF-8");
        lrcFileStream << m_LrcList[idx];
        lrcFile.close();
    }
    emit SIG_PlayOnline( m_mediaList[idx]  ,  ui->listWidget->currentItem()->text());
}

void DownloadWidget::slot_MenuAction(QAction* action)
{
    if(  ui->listWidget->currentItem() == NULL ) return;


    if( "播放音乐" == action->text() )
    {
        network_request4->setUrl( QUrl(m_imgList[ ui->listWidget->currentIndex().row()  ]) );
        // 发送请求
        network_manager4->get(*network_request4);
        on_listWidget_doubleClicked(QModelIndex());
    }else if("下载到本地" == action->text())
    {
        qDebug()<<m_mediaList[ ui->listWidget->currentIndex().row()] ;
        network_request4->setUrl( QUrl(m_imgList[ ui->listWidget->currentIndex().row()  ]) );
        // 发送请求
        network_manager4->get(*network_request4);
        // 发送https请求前准备工作;
        QSslConfiguration conf = network_request3->sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        network_request3->setSslConfiguration(conf);

        //构造请求
        network_request3->setUrl( QUrl(m_mediaList[ ui->listWidget->currentIndex().row()  ]) );
        // 发送请求
        network_manager3->get(*network_request3);
    }
}

QString DownloadWidget::GetMD5()
{

    char ch[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    int size = sizeof(ch);

    QString str="";

    int num = 0;
    for (int i = 0; i < 4; ++i)
    {
        num = rand() % (size - i - 1);
        ch[num] = ch[num] + ch[size - i - 1 ];
        ch[size - i - 1 ] = ch[num] - ch[size - i - 1 ];
        ch[num] = ch[num] - ch[size - i - 1 ];
        str += ch[size - i - 1 ];
    }
    qDebug()<<str;
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str.toStdString().c_str());

      // 添加数据到加密哈希值
    QByteArray result = hash.result();  // 返回最终的哈希值
//    qDebug()<<result;
    return result.toHex();
}
