#include "musicwidget.h"
#include "ui_musicwidget.h"
#include<QDebug>
#include<QFileDialog>
#include<QFileInfo>
#include<QPixmap>
#include<QTime>
#include<QMessageBox>

MusicWidget* MusicWidget::m_p = 0;
MusicWidget::MusicWidget(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::MusicWidget),m_MusicListCount(0),m_VolumnLast(30),m_VolumnNow(30),
    m_musicStartFlag(false),m_musicPositionChangeFlag(false),m_musicLrcLineCount(0),
    lrcAddLineCount(0),m_voiceOpenFlag(true)
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint );

    m_Player = new QMediaPlayer();
    connect(m_Player, SIGNAL(positionChanged(qint64)), this, SLOT(slot_MusicPositionChanged(qint64)));

    m_Player->setVolume(30);
    ui->slider_volumn->setValue(30);
    m_p = this;

    m_sql = new CSqlite();
    loadSqlAndSetMusicList();
    //ui->lw_musicList->setStyleSheet("background-color:transparent");
    //ui->lw_Lyric->setStyleSheet("background-color:transparent");
//    ui->lw_musicList->setStyleSheet("background-color: rgba(9, 9, 9,150);color: rgba(255, 255, 255,255)");
//    ui->lw_Lyric->setStyleSheet("background-color: rgba(9, 9, 9,150);color: rgba(255, 255, 255,255)");

   // ui->gb_container->setStyleSheet("");
    //设置默认窗口背景
    QPixmap pixmap = QPixmap(":/images/start.png");
    pixmap = pixmap.scaled(this->width(),this->height());
    QPalette pal(this->palette());
    pal.setBrush(QPalette::Background , QBrush(pixmap));
    this->setPalette( pal );

    m_downloadWidget = new DownloadWidget;
    connect(m_downloadWidget, SIGNAL(SIG_PlayOnline(QString  , QString )),this ,SLOT(slot_PlayOnline(QString , QString)) );
    connect(m_downloadWidget ,SIGNAL(SIG_musicDownloaded(QString )),this,SLOT(slot_musicDownloaded(QString)));
    connect(m_downloadWidget ,SIGNAL(SIG_refreshImg()),this,SLOT(slot_setWindowBackGroundImage() ));

    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon ic = QIcon(":/images/logo.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(ic);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("音乐"));

    //在系统托盘显示此对象
    mSysTrayIcon->show();
}

MusicWidget::~MusicWidget()
{
    delete ui;
}


void MusicWidget::on_pb_close_clicked()
{
    qDebug()<<"on_pb_close_clicked";
    m_downloadWidget->close();

    QString sqlStr = QString("Update t_volumnSet set volumn = %1;").arg(m_VolumnNow);
    m_sql->UpdateSql(sqlStr);
    this->close();
}

void MusicWidget::on_pb_min_clicked()
{
    qDebug()<<"on_pb_min_clicked";
    this->showMinimized();
}

void MusicWidget::on_pb_AddMusic_clicked()
{
    bool hasSameItem = false;
    QStringList path = QFileDialog::getOpenFileNames(this,"选择音乐文件","./music");
    for( int i = 0; i< path.count();i++)
    {
        hasSameItem = false;
        for( int j = 0; j<m_MusicListCount;j++)
        {
            if(m_MusicList[j].compare(path.at(i)) == 0)
            {
                hasSameItem = true;
                break;
            }
        }
        //保存在歌单
        if(hasSameItem == false)
        {
            m_MusicList[m_MusicListCount++] = path.at(i); //保存全路径
            //向控件添加文件名
            QFileInfo info(path.at(i));
            ui->lw_musicList->addItem(info.baseName());
            QString sqlStr = QString("insert into t_musicList ( musicName, musicPath )values ('%1','%2');").arg(info.baseName()).arg( path.at(i));
            m_sql->UpdateSql( sqlStr );
        }
    }
}

//双击音乐播放
void MusicWidget::on_lw_musicList_doubleClicked(const QModelIndex &index)
{
//    player->setMedia(QUrl::fromLocalFile("/****/test.mp3"));
//    player->setVolume(50);
//    player->play();

    m_Player->setMedia(QUrl::fromLocalFile(m_MusicList[ ui->lw_musicList->currentIndex().row()] ));

    m_Player->play();

    //切换播放按键显示
    ui->pb_play->setIcon(QIcon(":/images/bfzn_003.png"));
    //存储当前的歌曲名字
    m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
    ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
    //设置背景
    slot_setWindowBackGroundImage();
    //标记播放状态
    m_musicStartFlag = true;
}

void MusicWidget::slot_setWindowBackGroundImage()
{
    QPixmap pixmap = QPixmap("./images/"+m_CurrentMusicName+".png");
    if(pixmap.isNull() == false)
    {
        pixmap = pixmap.scaled(this->width(),this->height());
        QPalette pal(this->palette());
        pal.setBrush(QPalette::Background , QBrush(pixmap));
        this->setPalette( pal );
    }else    
    {
        QPixmap pixmap = QPixmap("./images/"+m_CurrentMusicName+".jpg");
        if(pixmap.isNull() == false)
        {
            pixmap = pixmap.scaled(this->width(),this->height());
            QPalette pal(this->palette());
            pal.setBrush(QPalette::Background , QBrush(pixmap));
            this->setPalette( pal );
        }else
        {
            //不能找到当前歌曲的背景图片设置默认
            QPixmap pixmap = QPixmap(":/images/start.png");
            pixmap = pixmap.scaled(this->width(),this->height());
            QPalette pal(this->palette());
            pal.setBrush(QPalette::Background , QBrush(pixmap));
            this->setPalette( pal );
        }
    }
}
void MusicWidget::slot_MusicPositionChanged(qint64 val)
{
   static int playFlag = 0;
   static bool lrcBeginFlag = false;
   //qDebug()<<"slot_MusicPositionChanged";
   if( m_Player->state() == QMediaPlayer::PlayingState )
   {
//       playFlag++;
       if( m_Player->duration()/*&& playFlag >= 2*/)
       {
           playFlag= 0;
          // qDebug()<<"slot_MusicPositionChanged"<< m_Player->duration();
           ui->slider_MusicProcess->setValue( m_Player->position()*100 / m_Player->duration());
           m_musicPositionChangeFlag = true;
           QTime duration(0,m_Player->position()/60000, qRound(m_Player->position()%60000/1000.0),0);
           QString time = duration.toString("mm:ss");
           QTime duration2(0,m_Player->duration()/60000, qRound(m_Player->duration()%60000/1000.0),0);
           ui->lb_musicTime->setText(time+"/"+ duration2.toString("mm:ss"));
           if( duration == duration2)
           {
               if( m_MusicListCount == 0 )
               {
                   //回到出事状态
                   m_Player->pause();
                   m_musicStartFlag = false;
                   ui->pb_play->setIcon(QIcon(":/images/bfzn_004.png"));
                   return;
               }
               //以下一曲为例
               qDebug()<<"Next Music->"<<m_MusicList[(ui->lw_musicList->currentIndex().row()+1)%m_MusicListCount];
               m_Player->setMedia( QUrl::fromLocalFile(m_MusicList[(ui->lw_musicList->currentIndex().row()+1)%m_MusicListCount]));
               ui->lw_musicList->setCurrentRow((ui->lw_musicList->currentIndex().row()+1)%m_MusicListCount );
               m_Player->play();
               m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
               ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
               slot_setWindowBackGroundImage();
           }

           if( m_musicLyric[0].compare(m_CurrentMusicName)!= 0 )//当前的歌如果不在记录中
           {
               QFile lrcFile("lrc/"+ m_CurrentMusicName +".txt");

               ui->lw_Lyric->clear();
               if( lrcFile.open(QIODevice::ReadOnly))
               {
                   lrcBeginFlag = true;
                   QTextStream lrcFileStream(&lrcFile);

                   lrcFileStream.setCodec("UTF-8");
                   m_musicLrcLineCount = 0;
                   m_musicLyric[m_musicLrcLineCount++] = m_CurrentMusicName;
                   while(!lrcFileStream.atEnd())
                   {
                       QString line = lrcFile.readLine();
                       m_musicLyric[m_musicLrcLineCount++] = line;
                   }
                   lrcFile.close();
               }else // 打开失败, 表示没有该歌词文件
               {
                   int currentMusicRow = 0;
                   ui->lw_Lyric->clear();
                   lrcAddLineCount = 0;
                   for( int i = currentMusicRow -4 ; i < currentMusicRow+5 ; i++)
                   {
                       if(i== currentMusicRow)
                       {
                           ui->lw_Lyric->addItem("当前歌曲无歌词");
                       }else
                       {
                           ui->lw_Lyric->addItem("");
                       }
                       lrcAddLineCount++;
                       ui->lw_Lyric->setCurrentRow( lrcAddLineCount -1 );
                       ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);
                       if( i == currentMusicRow )
                       {
                           QFont font;
                           font.setPointSize(18);

                           ui->lw_Lyric->setCurrentRow(4);
                           ui->lw_Lyric->currentItem()->setFont(font);
                           ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);
                           ui->lw_Lyric->currentItem()->setTextColor(Qt::yellow);

                       }
                   }
               }
           }else //显示歌词
           {
               if(lrcBeginFlag == true)
               {
                   lrcBeginFlag = false;
                   int lrcAddLine = 0;
                   for(int i= 0 ; i< m_musicLrcLineCount && i < 9; ++i)
                   {
                       QStringList lrc = m_musicLyric[i].split("]");
                       if( lrc.count() == 2)
                       {
                           ui->lw_Lyric->addItem(lrc.at(1));
                       }else
                       {
                           ui->lw_Lyric->addItem( m_musicLyric[i] );
                       }
                       lrcAddLine++;
                       ui->lw_Lyric->setCurrentRow(lrcAddLine-1);
                       ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);
                   }

               }
                int currentMusicRow = 0;
                for(currentMusicRow = 0 ; currentMusicRow < m_musicLrcLineCount; currentMusicRow++)
                {
                    QString mm,ss;
                    mm = m_musicLyric[currentMusicRow].mid(1,2);
                    ss = m_musicLyric[currentMusicRow].mid(4,2);
                    QTime duration3(0,mm.toInt() , ss.toInt(),0);
                    if( duration3 == duration)
                    {
                        break;
                    }
                }
                if(currentMusicRow < m_musicLrcLineCount)
                {
                    ui->lw_Lyric->clear();
                    lrcAddLineCount = 0;

                    for( int i = currentMusicRow -4 ; i < currentMusicRow +5 ; i++)
                    {
                        if( i >= 0 && i < m_musicLrcLineCount)
                        {
                            QStringList lrc = m_musicLyric[i].split("]");
                            if( lrc.count() == 2)
                            {
                                ui->lw_Lyric->addItem(lrc.at(1));
                            }else
                            {
                                ui->lw_Lyric->addItem(m_musicLyric[i]);
                            }
                            lrcAddLineCount++;
                            ui->lw_Lyric->setCurrentRow(lrcAddLineCount-1);
                            ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);

                        }else
                        {
                            ui->lw_Lyric->addItem("");
                            lrcAddLineCount++;

                            ui->lw_Lyric->setCurrentRow(lrcAddLineCount-1);
                            ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);
                        }
                        if( i == currentMusicRow)
                        {
                            QFont font;
                            font.setPointSize(18);

                            ui->lw_Lyric->setCurrentRow(4);
                            ui->lw_Lyric->currentItem()->setFont(font);
                            ui->lw_Lyric->currentItem()->setTextAlignment(Qt::AlignCenter);
                            ui->lw_Lyric->currentItem()->setTextColor(Qt::yellow);
                        }
                    }
                }
           }
            ui->lw_Lyric->setCurrentRow(-1);
       }
   }
   else
   {
      playFlag = 0 ;
   }

}



void MusicWidget::on_pb_play_clicked()
{
    if(!m_musicStartFlag)
    {
        if( m_MusicListCount <= 0 ){
            QMessageBox::information(this,"提示","请添加音乐!");
            return;
        }
        ui->pb_play->setIcon(QIcon(":/images/bfzn_003.png"));
        m_musicStartFlag = true;
        m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
        ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
        if(  m_Player->state() == QMediaPlayer::StoppedState  )
        {
            m_Player->setMedia( QUrl::fromLocalFile(m_MusicList[ui->lw_musicList->currentIndex().row()]));
            slot_setWindowBackGroundImage();
        }
        m_Player->play();

    }else
    {
        ui->pb_play->setIcon(QIcon(":/images/bfzn_004.png"));
        m_musicStartFlag = false;
        m_Player->pause();
    }
}

void MusicWidget::on_pb_DeleteMusic_clicked()
{
    if( m_MusicListCount <=0 ) return;
    QString sqlStr = QString("Delete from t_musicList where musicPath = '%1';").arg(m_MusicList[ui->lw_musicList->currentIndex().row()]);
    qDebug()<<"delete current:"<< ui->lw_musicList->currentItem()->text();
    m_sql->UpdateSql(sqlStr);
    for( int i= ui->lw_musicList->currentIndex().row();i< m_MusicListCount-1 ; i++)
    {
        m_MusicList[i]= m_MusicList[i+1];
    }
    m_MusicListCount--;
    ui->lw_musicList->takeItem(ui->lw_musicList->currentIndex().row());


}

void MusicWidget::on_slider_MusicProcess_valueChanged(int value)
{
    if( m_musicPositionChangeFlag == false)
    {
        if(m_musicStartFlag)
        {
            m_Player->pause();
            m_Player->setPosition(value*m_Player->duration()/100);
            m_Player->play();
//            m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
        }
       else
        {
            m_Player->setPosition(value*m_Player->duration()/100);
        }
        QTime duration(0,m_Player->position()/60000, qRound(m_Player->position()%60000/1000.0),0);
        QString time = duration.toString("mm:ss");
        QTime duration2(0,m_Player->duration()/60000, qRound(m_Player->duration()%60000/1000.0),0);
        ui->lb_musicTime->setText(time+"/"+ duration2.toString("mm:ss"));

    }else
    {
        m_musicPositionChangeFlag = false;
    }
}

void MusicWidget::on_pb_next_clicked()
{
    if( m_MusicListCount == 0 )
    {
        //回到出事状态
        m_Player->pause();
        m_musicStartFlag = false;
        ui->pb_play->setIcon(QIcon(":/images/bfzn_004.png"));
        return;
    }
    //下一曲为例
    m_Player->setMedia(QUrl::fromLocalFile(m_MusicList[ (ui->lw_musicList->currentIndex().row()+1)%m_MusicListCount]));
    ui->lw_musicList->setCurrentRow( (ui->lw_musicList->currentIndex().row()+1)%m_MusicListCount );

    m_Player->play();
    m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
    ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
    if(m_musicStartFlag == false)
    {
        m_musicStartFlag = true;
        ui->pb_play->setIcon(QIcon(":/images/bfzn_003.png"));
    }
    slot_setWindowBackGroundImage();
}

void MusicWidget::on_pb_pre_clicked()
{
    if( m_MusicListCount == 0 )
    {
        //回到出事状态
        m_Player->pause();
        m_musicStartFlag = false;
        ui->pb_play->setIcon(QIcon(":/images/bfzn_004.png"));
        return;
    }
    //上一曲为例
    if(ui->lw_musicList->currentIndex().row()-1>= 0 )
    {
        m_Player->setMedia(QUrl::fromLocalFile(m_MusicList[ (ui->lw_musicList->currentIndex().row()-1)%m_MusicListCount]));
        ui->lw_musicList->setCurrentRow( (ui->lw_musicList->currentIndex().row()-1)%m_MusicListCount );
    }
    else
    {
        m_Player->setMedia(QUrl::fromLocalFile(m_MusicList[ (m_MusicListCount-1) ]));
        ui->lw_musicList->setCurrentRow( m_MusicListCount-1 );
    }
    m_Player->play();
    m_CurrentMusicName = ui->lw_musicList->currentItem()->text();
    ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
    if(m_musicStartFlag == false)
    {
        m_musicStartFlag = true;
        ui->pb_play->setIcon(QIcon(":/images/bfzn_003.png"));
    }
    slot_setWindowBackGroundImage();
}

void MusicWidget::on_pb_search_clicked()
{
    if(!ui->le_search->text().isEmpty())
    {
        qDebug()<< ui->le_search->text();
        m_downloadWidget->search(ui->le_search->text());
        m_downloadWidget->show();
    }

}

void MusicWidget::on_pb_volumn_clicked()
{
    if(m_voiceOpenFlag)
    {
        m_voiceOpenFlag = false;
        ui->pb_volumn->setIcon(QIcon(":/images/voice_close.png"));


//        ui->lb_volumn->setText("0%");
        ui->slider_volumn->setValue(0);
    }else
    {
        ui->slider_volumn->setValue(m_VolumnLast);
        m_voiceOpenFlag = true;
        ui->pb_volumn->setIcon(QIcon(":/images/voice_open.png"));
//        ui->lb_volumn->setText(QString::number(m_VolumnNow)+"%");
    }
}

void MusicWidget::on_slider_volumn_valueChanged(int value)
{
    m_Player->setVolume(value);

    if( m_VolumnNow != value )
    {
        m_VolumnLast = m_VolumnNow;
        m_VolumnNow = value;
    }

    ui->lb_volumn ->setText(QString::number(value)+"%");
    if( value == 0 )
    {
        if( m_voiceOpenFlag )
        {
            ui->pb_volumn->setIcon(QIcon(":/images/voice_close.png"));
        }
        m_voiceOpenFlag = false;
    }else
    {
        if( !m_voiceOpenFlag )
        {
            ui->pb_volumn->setIcon(QIcon(":/images/voice_open.png"));
        }
        m_voiceOpenFlag = true;
    }

}

void MusicWidget::slot_PlayOnline(QString strUrl , QString musicName)
{
    m_Player->setMedia( QUrl(strUrl) );
    m_Player->play();
    //    //切换播放按键显示
    ui->pb_play->setIcon(QIcon(":/images/bfzn_003.png"));

    //    //标记播放状态
    m_musicStartFlag = true;
    m_CurrentMusicName =  musicName;
    ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
    slot_setWindowBackGroundImage();
}

void MusicWidget::slot_musicDownloaded(QString path  )
{
    m_MusicList[m_MusicListCount++] = path; //保存全路径
    //向控件添加文件名
    QFileInfo info(path );
    ui->lw_musicList->addItem(info.baseName());
    QMessageBox::information(this, "提示",QString("%1 下载完成, 已添加至歌单").arg(info.baseName()));
    QString sqlStr = QString("insert into t_musicList ( musicName, musicPath )values ('%1','%2');").arg(info.baseName()).arg( path );
    m_sql->UpdateSql( sqlStr );
}

void MusicWidget::loadSqlAndSetMusicList()
{
    QString DBDir = QDir::currentPath()+ "/sql/";
    QString FileName = "music.db";
    QDir tempDir;
    tempDir.setPath(DBDir);
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(DBDir))
    {
        qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(DBDir);
    }
    if(!tempDir.exists(QDir::currentPath()+"/music/"))
    {
        qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(QDir::currentPath()+"/music/");
    }
    QFile *tempFile = new QFile;
       //将程序的执行路径设置到filePath下

    if(tempFile->exists(DBDir+FileName))
    {
        qDebug()<<QObject::tr("文件存在");
        //加载 并添加至歌单
        m_sql->ConnectSql(DBDir+FileName);
        QStringList resList;
        QString SqlStr ="select musicName, musicPath from t_musicList;";
        if( !m_sql->SelectSql(SqlStr ,2 ,resList ) )
        {
            return;
        }
        for( int i = 0 ; i< resList.count() ; i+= 2)
        {
            //向控件添加文件名
            ui->lw_musicList->addItem( resList.at(i) );
            m_MusicList[m_MusicListCount++] = resList.at(i+1);; //保存全路径
        }
        resList.clear();
        SqlStr ="select * from t_volumnSet;";
        if( !m_sql->SelectSql(SqlStr ,1 ,resList ) )
        {
            return;
        }
        if( !resList.isEmpty())
        {
            m_VolumnLast = m_VolumnNow = ((QString)resList.at(0)).toInt();
            m_Player->setVolume(m_VolumnNow);
            ui->slider_volumn->setValue(m_VolumnNow);
        }
    }else
    {
        tempFile->setFileName(DBDir+FileName);
        if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug()<<QObject::tr("打开失败");
            QMessageBox::information(this,"提示","数据库加载失败,会导致无法存储歌曲信息!");
        }else
        {
            qDebug()<<QObject::tr("创建数据库文件");
            tempFile->close();
            m_sql->ConnectSql(DBDir+FileName);
            //创建表
            QString SqlStr ="create table t_musicList (musicName varchar(260) , musicPath varchar(260));";
            m_sql->UpdateSql(SqlStr ) ;

            SqlStr = "create table t_volumnSet( volumn int);";
            m_sql->UpdateSql(SqlStr ) ;

            SqlStr = "insert into t_volumnSet (volumn ) values( 30 );";
            m_sql->UpdateSql(SqlStr ) ;

        }
    }
}

void MusicWidget::on_slider_MusicProcess_sliderPressed()
{
//    qDebug()<<ui->slider_MusicProcess->value();
}

void MusicWidget::on_slider_MusicProcess_actionTriggered(int action)
{
//    qDebug()<<action;
}
