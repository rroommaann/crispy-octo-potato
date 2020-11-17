#include "multihashofrecords.h"
#include <QSqlQuery>

MultiHashOfRecords::MultiHashOfRecords(QSqlQuery *query, QString key)
{
    QSqlRecord record;
    m_multiHash = new QMultiMap<QVariant, QSqlRecord>;
    int i = 0;
    while(query->record().fieldName(i) != "")
    {
        columns.append(query->record().fieldName(i));
        i++;
    }

    while (query->next())
    {
        record = query->record();
        if(record.field(key).value().toInt() != 0)
        {
            QVariant s = record.field(key).value().toInt();
            m_multiHash->insert(s, record);
        }
        else
        {
            QString s = record.field(key).value().toString();
            m_multiHash->insert(s, record);
        }
    }
}

MultiHashOfRecords::MultiHashOfRecords()
{

}


MultiHashOfRecords::~MultiHashOfRecords()
{
}

QMultiMap<QVariant, QSqlRecord>* MultiHashOfRecords::getMassive()
{
    return this->m_multiHash;
}

QStringList MultiHashOfRecords::getColumns() const
{
    return columns;
}
