#ifndef CSQLITE_H
#define CSQLITE_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include<QMutex>
#include<QStringList>
#include<QTextCodec>
#include<QDebug>

class CSqlite
{
public:
    CSqlite();
    ~CSqlite(){}

    void  ConnectSql(QString db);
    void  DisConnect();
    bool  SelectSql(QString sqlstr ,int nColumn,QStringList &list );

    //更新：删除、插入、修改
    bool  UpdateSql(QString sqlstr);
private:
    QSqlDatabase m_db;
    QMutex m_mutex;
};

#endif // CSQLITE_H
