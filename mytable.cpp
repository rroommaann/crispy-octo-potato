#include "mytable.h"
#include "globalFunctionsQT.h"

MyTable::MyTable(const QString &dbName, QWidget *parent)
    : QWidget(parent),
    m_nameDB{dbName}
{
    QString q = QStringLiteral(u"SELECT * "
                "FROM Stations "
                "ORDER BY NoSt ASC;");
    QSqlQuery query(getDb(dbName, "64465"));
    query.exec(q);
    if(hash)
        delete hash;
    hash = new MultiHashOfRecords(query, QStringLiteral("NoSt"));

    query.first();
    do
    {
        listOfStations << query.record().field("NameSt").value().toString();
    } while(query.next());
}

MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
}

void MyTable::setViewReady()
{
    table->setColumnCount(hash->getColumns().count());
    table->setHorizontalHeaderLabels(hash->getColumns());

    auto values = hash->getMassive()->values();
    for(const auto &record : qAsConst(values))
    {
        table->setRowCount(table->rowCount() + 1);
        int j = 0;
        while(record.fieldName(j) != "")
        {
            auto *item = new QTableWidgetItem(record.field(j).value().toString());

            table->setItem(table->rowCount() - 1, j, item);
            j++;
        }
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    ready = true;
}

auto MyTable::getListOfStations() const -> const QStringList &
{
    return listOfStations;
}

auto MyTable::getNameDB() const -> QString
{
    return m_nameDB;
}

void MyTable::createNewHashData(const QStringList &listOfColumns, tableType type)
{
    QSqlQuery query(getDb(m_nameDB, QStringLiteral(u"64465")));
    query.exec(prepareQuery(choosedStation, listOfColumns, type));
    QString key;
    if(type == TS)
        key = QStringLiteral(u"NameTs");
    else if (type == TU)
        key = QStringLiteral(u"NameTu");
    else if(type == Stations)
        key = QStringLiteral(u"NoSt");
    else if (type == RouteSrc)
        key = QStringLiteral(u"No");
    else if (type == TuSpok)
        key = QStringLiteral(u"No");

    if(hash)
        delete hash;
    hash = new MultiHashOfRecords(query, key);
}

void MyTable::resetTable(const QString &dbName)
{
    if(!dbName.isEmpty())
        m_nameDB = dbName;

    QString q = QStringLiteral(u"SELECT * "
                "FROM Stations "
                "ORDER BY NoSt ASC;");
    QSqlQuery query(getDb(m_nameDB, QStringLiteral(u"64465")));
    query.exec(q);

    if(hash)
        delete hash;
    hash = new MultiHashOfRecords(query, QStringLiteral(u"NoSt"));
    table->setRowCount(0);
    setViewReady();
}

auto MyTable::isReady() const -> bool
{
    return ready;
}

auto MyTable::getTable() const -> QTableWidget *
{
    return table;
}

void MyTable::setTable(QTableWidget *value)
{
    table = value;
    setViewReady();
}

auto MyTable::getHash() const -> MultiHashOfRecords *
{
    return hash;
}

void MyTable::setStation(const QString &stationName)
{
    choosedStation = stationName;
}

MyTable::~MyTable()
= default;
