#include "multihashofrecords.h"
#include <QSqlQuery>

MultiHashOfRecords::MultiHashOfRecords(QSqlQuery *query, QString key)
{
    QSqlRecord record;
    m_multiHash = new QMap <QString, QSqlRecord>;
    int i = 0;
    while(query->record().fieldName(i) != "")
    {
        columns.append(query->record().fieldName(i));
        i++;
    }

    while (query->next())
    {
        record = query->record();
        QString s = record.field(key).value().toString();
        m_multiHash->insert(s, record);
    }
}

MultiHashOfRecords::MultiHashOfRecords()
{

}

MultiHashOfRecords::~MultiHashOfRecords()
{
}

QMap<QString, QSqlRecord>* MultiHashOfRecords::getMassive()
{
    return this->m_multiHash;
}

QStringList MultiHashOfRecords::getColumns() const
{
    return columns;
}
