#include "csqlite.h"


CSqlite::CSqlite()  //目录
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE");

}

void  CSqlite::ConnectSql(QString db)
{
    m_db.setDatabaseName(db);
}

void  CSqlite::DisConnect()
{
    m_db.close();
}

bool CSqlite::SelectSql(QString sqlstr ,int nColumn,QStringList &list )
{
    bool success;
    m_mutex.lock();
    m_db.open();
    QSqlQuery query ;
    query.prepare(sqlstr);
    success  = query.exec() ;

    if(!success)
    {
        qDebug()<<sqlstr<<QString(QObject::tr(":SelectData error"));
        m_db.close();
        m_mutex.unlock();
        return false;
    }
    else
    {
        for(  ; query.next(); )
        {
            for(int i = 0; i < nColumn ; i++)
            {
                list.append( query.value(i).toString() );
            }
        }
    }
    m_db.close();
    m_mutex.unlock();

    return true;
}

//更新：删除、插入、修改
bool  CSqlite::UpdateSql(QString sqlstr)
{
    m_mutex.lock();

    m_db.open();

    QSqlQuery query;

    query.prepare( sqlstr );

    bool success = query.exec() ;

    if(!success)
    {
        qDebug()<<sqlstr<<QString(QObject::tr(":UpdateData fail!"));
        m_db.close();
        m_mutex.unlock();
        return false;
    }
    m_db.close();
    m_mutex.unlock();
    return true;
}

