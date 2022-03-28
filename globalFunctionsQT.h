#ifndef GLOBALFUNCTIONSQT_H
#define GLOBALFUNCTIONSQT_H

#include <QSqlDatabase>
#include <QRegularExpression>
#include <QDebug>

static QSqlDatabase getDb(const QString &dbPath, const QString &passwordForMdb = "64465")
{
    if (QSqlDatabase::contains(dbPath))
        return QSqlDatabase::database(dbPath);

    QSqlDatabase dbSql;
    if(dbPath.contains(QRegularExpression(R"(.+\.mdb)")))
    {
        dbSql = QSqlDatabase::addDatabase("QODBC", dbPath);
        dbSql.setDatabaseName("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + dbPath);
        //        dbSql.setHostName("localhost");
        //        dbSql.setUserName("admin");
        dbSql.setPassword(passwordForMdb);
    }
    else if(dbPath.contains(QRegularExpression(R"(.+\.db)")))
    {
        dbSql = QSqlDatabase::addDatabase("QSQLITE", dbPath);
        dbSql.setDatabaseName(dbPath);
    }
    dbSql.open();
    return dbSql;
}

#endif // GLOBALFUNCTIONSQT_H
