#include "multihashofrecords.h"

MultiHashOfRecords::MultiHashOfRecords(QSqlRelationalTableModel *model)
{
    QSqlRecord record;
    m_multiHash = new QMultiHash <QString, QSqlRecord>;
    for(int k = 0; k < model->rowCount(); k++){
        QString s = model->record(k).field("Module").value().toString() + " " + model->record(k).field("I").value().toString() + " " + model->record(k).field("J").value().toString();
        m_multiHash->insertMulti(s, model->record(k));
    }
}

MultiHashOfRecords::~MultiHashOfRecords() {
}

QMultiHash<QString, QSqlRecord>* MultiHashOfRecords::getMassive(){
    return this->m_multiHash;
}
