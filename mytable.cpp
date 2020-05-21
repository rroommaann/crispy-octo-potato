#include "mytable.h"
#include "globalFunctionsQT.h"

MyTable::MyTable(QString dbName, QWidget *parent)
    : QWidget(parent)
{
    m_nameDB = dbName;

    QString q = "SELECT * "
                "FROM Stations "
                "ORDER BY NoSt ASC;";
    QSqlQuery query(getDb(dbName, "64465"));
    query.exec(q);
    hash = MultiHashOfRecords(&query, "NoSt");
}

MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
}

void MyTable::setViewReady()
{
    table->setColumnCount(hash.getColumns().count());
    table->setHorizontalHeaderLabels(hash.getColumns());

    for(auto record : hash.getMassive()->values())
    {
        table->setRowCount(table->rowCount() + 1);
        int j = 0;
        while(record.fieldName(j) != "")
        {
            QTableWidgetItem *item = new QTableWidgetItem(record.field(j).value().toString());

            table->setItem(table->rowCount() - 1, j, item);
            j++;
        }
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    ready = true;
}

QString MyTable::getNameDB() const
{
    return m_nameDB;
}

void MyTable::createNewHashData(QStringList listOfColumns, tableType type)
{
    QSqlQuery query(getDb(m_nameDB, "64465"));
    query.exec(prepareQuery(choosedStation, listOfColumns, type));
    QString key;
    if(type == TS)
    {
        key = "NameTs";
    }
    else if (type == TU)
    {
        key = "NameTu";
    }
    else if(type == Stations)
    {
        key = "NoSt";
    }
    else if (type == RouteSrc)
    {
        key = "No";
    }

    hash = MultiHashOfRecords(&query, key);
}

void MyTable::resetTable()
{
    QString q = "SELECT * "
                "FROM Stations "
                "ORDER BY NoSt ASC;";
    QSqlQuery query(getDb(m_nameDB, "64465"));
    query.exec(q);

    hash = MultiHashOfRecords(&query, "NoSt");
    table->setRowCount(0);
    setViewReady();
}

bool MyTable::isReady() const
{
    return ready;
}

QTableWidget *MyTable::getTable() const
{
    return table;
}

void MyTable::setTable(QTableWidget *value)
{
    table = value;
    setViewReady();
}

MultiHashOfRecords MyTable::getHash() const
{
    return hash;
}

void MyTable::setStation(QString q)
{
    choosedStation = q;
}

MyTable::~MyTable()
{
}
