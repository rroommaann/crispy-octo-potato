#include "multihashofrecords.h"

MultiHashOfRecords::MultiHashOfRecords(QSqlRelationalTableModel *model)
{
    QSqlRecord record;
    m_multiHash = new QMultiHash <QString, QSqlRecord>;
    for(int k = 0; k < model->rowCount(); k++)
    {
        record = model->record(k);
        QString s = record.field("NameTs").value().toString();
        m_multiHash->insert(s, record);
    }
}

MultiHashOfRecords::~MultiHashOfRecords()
{
}

QMultiHash<QString, QSqlRecord>* MultiHashOfRecords::getMassive()
{
    return this->m_multiHash;
}
