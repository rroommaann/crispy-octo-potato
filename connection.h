#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>
#include <QSqlDatabase>

static bool createConnection(QString tableName){
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", tableName);
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + tableName);
    db.setUserName("Admin");
    db.setPassword("64465");
    return db.open();
}

#endif // CONNECTION_H

