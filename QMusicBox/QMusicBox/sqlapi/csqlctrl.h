#ifndef CSQLCTRL_H
#define CSQLCTRL_H

#include"csqlite.h"


class CSqlCtrl
{
public:
    CSqlCtrl();
    ~CSqlCtrl();

    static CSqlCtrl* GetSqlInstance();
    void InitDB(QString DBDir , QString HostName);

    bool SelectUserInfo(QStringList& list);
    bool SelectShareFileInfo(QStringList& list);
    bool SelectShareFileInfo(QString FileName ,QString time , QStringList& list);

    bool SelectGroupInfo(QStringList& list);
    bool SelectGroup(QString GroupNum,QStringList& list);

    bool SelectHistoryItem(QStringList& list);
    bool SelectHistoryInfo(QString Mac , int nPage,QStringList& list);//分页 一页20
    bool SelectIsAutoRun(QStringList &list);
    bool SelectHistoryCount( QString Mac ,QStringList &list);

    bool InsertHistory( QString Name , QString Mac , QString IP , QString time ,QString content);
    bool InsertGroupInfo(QString GroupName,QString GroupIcon,QString GroupNum  );

    bool InsertData(QString strSql);
    bool DeleteData(QString strSql);
    bool UpdateData(QString strSql);

    bool UpdateIsAutoRun( bool isAuto);
    bool UpdateGroupInfo(QString GroupName , QString GroupIcon , QString GroupNum);
    bool UpdateUserInfo(QString UserName, QString UserIcon,QString m_Mark );

    bool DeleteOnePersonHistory(QString Mac);
    bool DeletePersonFromGroup( QString GroupNum , QString UserMac);
private :
    CSqlite *m_db;
    static CSqlCtrl* m_pSqlCtrl;
};

#endif // CSQLCTRL_H
