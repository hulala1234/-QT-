#include "csqlctrl.h"
#include<QDir>

CSqlCtrl*  CSqlCtrl::m_pSqlCtrl =  new CSqlCtrl;
CSqlCtrl::CSqlCtrl()  //目录
{
    m_db = new CSqlite();
}
CSqlCtrl::~CSqlCtrl()
{
    if(m_db)
        delete m_db;
}

CSqlCtrl* CSqlCtrl::GetSqlInstance()
{
    if( m_pSqlCtrl ) return m_pSqlCtrl;
    else return nullptr;
}
////需要先调用createIndexDBPath 之后才能调用这个  (m_dateTime)
void CSqlCtrl::InitDB( QString DBDir ,QString HostName)
{
    m_db->ConnectSql(DBDir+"User.db");

    QDir tempDir;
    tempDir.setPath(DBDir);
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(DBDir))
    {
        qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(DBDir);
    }
    QFile *tempFile = new QFile;
       //将程序的执行路径设置到filePath下
    quint64 FileSize = 0;
    if(tempFile->exists(DBDir+"User.db"))
    {
        qDebug()<<QObject::tr("文件存在");
        QFileInfo fi(DBDir+"User.db");
        FileSize = fi.size();
    }else
    {
        tempFile->setFileName(DBDir+"User.db");
        if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug()<<QObject::tr("打开失败");
        }
        FileSize = tempFile->size();
        tempFile->close();
    }

    do
    {
        if( FileSize > 10 )  break;

        //未打开证明没有, 需要创建表
        //表单1 : 用户信息 t_userInfo: 字段依次是id 用户名 主机名 头像 (个性签名) 电话号码 邮箱
        //表单2 : 好友信息 t_FriendInfo: 字段依次是 Mac地址 用户名 主机名 IP 头像 电话号码 邮箱 MAC地址是主键
        //表单3 : 历史记录 t_History: 发送方mac地址,姓名  ip , 时间, /*fromorto 0给用户 1给好友,*/ 内容(聊天,文件保存路径) 记录的问题是 什么时间与谁聊天, 聊了什么 群组怎么办
        //表单4 : 系统设置 :t_SystemSet: 字段依次是 isAutoRun int(1/0)
        //表单5 : 共享文件设置 t_ShareFile: 文件名, 类型, 路径 ,时间, 是否需要密码, 密码
        //表单6 : 群组成员信息 t_Group: 字段依次是 群号 组成员名字,ip, mac , icon
        //表单7 : 群组信息    t_GroupInfo: 字段依次是  组名 组Icon 群号

        QString SqlStr = "create table t_userInfo (id int, UserName varchar(50) , cpuName varchar(50), IconId varchar(50),Mark varchar(50), tel varchar(20) )";

        if( !m_db->UpdateSql(SqlStr ) )
        {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_userInfo create fail\n"));
            break;
        }
        qsrand(0);
        QString img = QString(":/images/tx/%1.png").arg( qrand()%9+1 );
        SqlStr = QString("insert into t_userInfo values('1','%1','%2','%3','','')")
                .arg(HostName).arg(HostName).arg(img);

        if( !m_db->UpdateSql(SqlStr ) )
        {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_userInfo insert fail\n"));
            break;
        }

        SqlStr = "create table t_FriendInfo ( MacAddr varchar(20) , FriendName varchar(50), cpuName varchar(50), IpAddr varchar(20), IconId int,tel varchar(20), email varchar(20) , PRIMARY KEY (MacAddr))";

        if( !m_db->UpdateSql(SqlStr ) ) {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_FriendInfo create fail\n"));
            break;
        }

        SqlStr = "create table t_History ( MacAddr varchar(20),Name varchar(50), IpAddr varchar(20), time datetime, content TEXT )";

        if( !m_db->UpdateSql(SqlStr ) ) {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_History create fail\n"));
            break;
        }

        SqlStr = "create table t_ShareFile ( FileName varchar(255), type varchar(20), FilePath varchar(255),time datetime, NeedCode int ,password varchar(20) )";
        if( !m_db->UpdateSql(SqlStr ) ){
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_ShareFile create fail\n"));
            break;
        }
        SqlStr = "create table t_SystemSet ( isAutoRun int )";
        if( !m_db->UpdateSql(SqlStr ) ){
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_SystemSet create fail\n"));
            break;
        }

        SqlStr = "insert into t_SystemSet values( 1 )";
        if( !m_db->UpdateSql(SqlStr ) ){
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_SystemSet insert fail\n"));
            break;
        }

        SqlStr = "create table t_Group ( GroupNum varchar(20), UserName varchar(50),ip varchar(20) , Mac varchar(20) ,Icon varchar(50) )";
        if( !m_db->UpdateSql(SqlStr ) ) {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_Group create fail\n"));
            break;
        }
        //t_GroupInfo: 字段依次是 组名 组Icon 群号

        SqlStr = "create table t_GroupInfo ( GroupName varchar(50), Icon varchar(50) ,GroupNum varchar(20) )";
        if( !m_db->UpdateSql(SqlStr ) ) {
            qDebug()/*<<query.lastError()*/<<QString(QObject::tr(" t_Group create fail\n"));
            break;
        }
    }while(0);

    return ;
}

bool CSqlCtrl::SelectUserInfo(QStringList& list)
{

    QString sqlstr=  QString("select  UserName  , cpuName , IconId ,mark , tel  from t_userInfo where id = 1;")  ;

    if( !m_db->SelectSql(sqlstr,5,list ) )
    {
        qDebug()<<  QString(QObject::tr( "SelectUserInfo error"));
        return false;
    }
        return true;

}
//文件名, 类型, 路径 ,时间, 是否需要密码, 密码
//create table t_ShareFile ( FileName varchar(255), type varchar(20), FilePath varchar(255),time datetime, NeedCode int ,password varchar(20) )
bool CSqlCtrl::SelectShareFileInfo(QStringList& list)
{
    QString sql =  QString("select  FileName  , type , FilePath ,time from t_ShareFile;")  ;


    if(!m_db->SelectSql(sql , 4, list ))
    {
        qDebug()<<  QString(QObject::tr( "SelectShareFileInfo error"));
        return false;
    }
    return true;
}

bool CSqlCtrl::SelectShareFileInfo(QString FileName ,QString time , QStringList& list)
{
    QString sql =  QString("select  FileName  , type , FilePath ,time from t_ShareFile where FileName = '%1' and time = '%2'").arg(FileName).arg(time)  ;

    if(!m_db->SelectSql(sql , 4, list ))
    {
        qDebug()<<  QString(QObject::tr( "SelectShareFileInfo error"));
        return false;
    }
    return true;
}

bool  CSqlCtrl::SelectGroupInfo(QStringList& list)
{

    QString Sql =  QString("select  groupName  , Icon , groupNum from t_groupInfo;")  ;


    if(!m_db->SelectSql(Sql , 3, list))
    {
        qDebug()<<  QString(QObject::tr( "ViewGroupInfo error"));
        return false;
    }
    return true;
}

bool CSqlCtrl::SelectGroup(QString GroupNum,QStringList& list)
{

    QString Sql = QString("select  userName  , ip , mac ,icon from t_group where groupNum = '%1';").arg(GroupNum)  ;


    if(!m_db->SelectSql(Sql, 4, list))
    {
        qDebug()<<  QString(QObject::tr( "ViewGroup error"));
        return false;
    }
    return true;
}
bool CSqlCtrl::SelectHistoryItem(QStringList& list)
{

    QString Sql = QString("select  Name  , ipAddr , macAddr  from t_History group by MacAddr;")  ;

    if(!m_db->SelectSql(Sql , 3, list))
    {
        qDebug()<<  QString(QObject::tr( "GetHistoryItem error"));
        return false;
    }
     return true;
}

bool CSqlCtrl::SelectHistoryInfo(QString Mac , int nPage,QStringList& list)  //分页 一页20
{

    QString str = QString("%1").arg( 20*(nPage-1) );
    QString SqlStr = QString("select content  from t_History where MacAddr = '%1' limit 20 offset %2 ;").arg(Mac).arg( str ) ;


    if(!m_db->SelectSql(SqlStr , 1, list))
    {
        qDebug()<<  QString(QObject::tr( "GetHistoryInfo error"));
        return false;
    }

    return true;
}

bool CSqlCtrl::InsertHistory( QString Name , QString Mac , QString IP , QString time ,QString content)
{
    //MacAddr varchar(20),Name varchar(50), IpAddr varchar(20), time datetime, content TEXT
    QString sql = QString("insert into t_History values( '%1','%2','%3','%4','%5');").arg(Mac).arg(Name).arg(IP).arg(time).arg(content);
    if( !m_db->UpdateSql( sql))
    {
        qDebug()<<"InsertHistory error";
        return false;
    }
        return true;

}

bool CSqlCtrl::InsertGroupInfo(QString GroupName,QString GroupIcon,QString GroupNum  )
{

    QString sql = QString("insert into t_GroupInfo values('%1','%2','%3');").arg(GroupName).arg(GroupIcon).arg(GroupNum) ;
    if( !m_db->UpdateSql( sql))
    {
        qDebug()<<"InsertGroupInfo error";
        return false;
    }
        return true;
}

bool CSqlCtrl::DeleteOnePersonHistory(QString Mac)
{
    QString sql = QString("delete from t_History where MacAddr = '%1' ;").arg(Mac);
    if( !m_db->UpdateSql( sql))
    {
        qDebug()<<"DeleteOnePersonHistory error";
        return false;
    }
    return true;
}
bool CSqlCtrl::DeletePersonFromGroup( QString GroupNum , QString UserMac)
{
    //this->sql->DeleteData( QString( "delete from t_group where groupnum = %1 and usermac = %2").arg(rq.GroupNum).arg(rq.UserMac));
    QString sql = QString("delete from t_group where groupnum = '%1' and usermac = '%2';").arg(GroupNum).arg(UserMac);
    if( !m_db->UpdateSql( sql))
    {
        qDebug()<<"DeletePersonFromGroup error";
        return false;
    }
    return true;
}

bool  CSqlCtrl::SelectHistoryCount( QString Mac ,QStringList &list)
{
    QString Sql =  QString("select count(MacAddr) from t_History where MacAddr = '%1';").arg(Mac)  ;
    if( m_db->SelectSql(Sql , 1, list))
    {
        qDebug()<<  QString(QObject::tr( "GetHistoryCount error"));
        return false;
    }
    return true;
}

bool CSqlCtrl::SelectIsAutoRun(QStringList &list)
{
    QString Sql=  QString("select * from t_SystemSet ;")  ;


    if(!m_db->SelectSql(Sql, 1 , list))
    {
        qDebug()<<  QString(QObject::tr( "GetHistoryCount error"));
        return false;
    }
     return true;
}

bool CSqlCtrl::UpdateIsAutoRun( bool isAuto)
{
    QString Sql = QString("Update t_SystemSet set IsAutoRun = '%1';").arg(isAuto) ;

    if(!m_db->UpdateSql(Sql))
    {
        qDebug()<<  QString(QObject::tr( "UpdateIsAutoRun error"));
        return false;
    }
    return true;
}
bool CSqlCtrl::UpdateGroupInfo(QString GroupName , QString GroupIcon , QString GroupNum)
{//QString( "update t_groupInfo set groupName = '%1', Icon = '%2' where groupnum = %3").arg(gw->m_GroupName).arg(gw->m_GroupIcon).arg(gw->m_GroupNum)
    QString Sql = QString("update t_groupInfo set groupName = '%1', Icon = '%2' where groupnum = %3;").arg(GroupName).arg(GroupIcon).arg(GroupNum) ;

    if(!m_db->UpdateSql(Sql))
    {
        qDebug()<<  QString(QObject::tr( "UpdateGroupInfo error"));
        return false;
    }
    return true;
}
bool CSqlCtrl::UpdateUserInfo(QString UserName, QString UserIcon,QString Mark )
{
    QString Sql = QString("update t_UserInfo set userName = '%1', IconId = '%2' , mark = '%3' where id = 1;").arg(UserName).arg(UserIcon).arg(Mark) ;

    if(!m_db->UpdateSql(Sql))
    {
        qDebug()<<  QString(QObject::tr( "UpdateGroupInfo error"));
        return false;
    }
    return true;
}

 bool CSqlCtrl::InsertData(QString strSql)
 {
    if(!m_db->UpdateSql(strSql))
     {
         qDebug()<<  QString(QObject::tr( "InsertData error"));
         return false;
     }
     return true;
 }
bool CSqlCtrl::DeleteData(QString strSql)
{
    if(!m_db->UpdateSql(strSql))
     {
         qDebug()<<  QString(QObject::tr( "DeleteData error"));
         return false;
     }
     return true;
}
bool CSqlCtrl::UpdateData(QString strSql)
{
    if(!m_db->UpdateSql(strSql))
     {
         qDebug()<<  QString(QObject::tr( "DeleteData error"));
         return false;
     }
     return true;
}
