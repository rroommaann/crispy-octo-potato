#include "multihashofrecords.h"
#include <QSqlQuery>

MultiHashOfRecords::MultiHashOfRecords(QSqlQuery &query, const QString &key)
{
    QSqlRecord record;
    m_multiHash = new QMultiMap<QVariant, QSqlRecord>;
    int i = 0;
    while(query.record().fieldName(i) != "")
    {
        columns.append(query.record().fieldName(i));
        i++;
    }

    query.first();
    do
    {
        record = query.record();
        QVariant s = record.field(key).value();
        m_multiHash->insert(s, record);
    } while(query.next());
}

MultiHashOfRecords::~MultiHashOfRecords()
= default;

auto MultiHashOfRecords::getMassive() -> QMultiMap<QVariant, QSqlRecord>*
{
    return this->m_multiHash;
}

auto MultiHashOfRecords::getColumns() const -> QStringList
{
    return columns;
}
