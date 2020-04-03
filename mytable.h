#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>
#include <QtWidgets>
#include <QSql>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QMouseEvent>
#include <QSqlRelationalDelegate>

#include "multihashofrecords.h"
#include "comparedbs.h"

class MyTable : public QWidget
{
    Q_OBJECT

public:
    explicit MyTable(QString dbName, QWidget *parent = nullptr);
    MyTable(QWidget *parent = nullptr);
    void setStation(QString q);
    ~MyTable();

    MultiHashOfRecords getHash() const;

    void setTable(QTableWidget *value);
    QTableWidget *getTable() const;

    bool isReady() const;

    QString getNameDB() const;
    void createNewHashData(QStringList listOfColumns, tableType type);

    void resetTable();

private:
    void setViewReady();
    QString m_nameDB;
    MultiHashOfRecords hash;
    QTableWidget *table = nullptr;

    bool ready = false;

    QString choosedStation = "нет";
};

#endif
