#include "multihashofrecords.h"

MultiHashOfRecords::MultiHashOfRecords(QSqlRelationalTableModel *model)
{
    int* temp = getMaxParameters(model);
    QSqlRecord record;
    m_multiHash = new QMultiHash <QString, QSqlRecord>;
    qDebug() << temp[0] << temp[1] << temp[2];
    qDebug() << "PARAMETERS";
//    for (int module = 0; module < temp[0]; module++){
//        for(int i = 0; i < temp[1]; i++){
//            for(int j = 0; j < temp[2]; j++){
//                for(int k = 0; k < model->rowCount(); k++){
//                    if ( model->record(k).field("Module").value().toInt() == module &&
//                        model->record(k).field("I").value().toInt()      == i &&
//                        model->record(k).field("J").value().toInt()      == j){
    for(int k = 0; k < model->rowCount(); k++){
        QString s = model->record(k).field("Module").value().toString() + " " + model->record(k).field("I").value().toString() + " " + model->record(k).field("J").value().toString();
        qDebug() << "Добавление строки по ключу " << s;
        m_multiHash->insertMulti(s, model->record(k));
        qDebug() << "Строка добавлена в m_multiHash";
        qDebug() << model->record(k).field("Module").value().toInt();
        qDebug() << model->record(k).field("I").value().toInt();
        qDebug() << model->record(k).field("J").value().toInt();
    }
}

MultiHashOfRecords::~MultiHashOfRecords() {
}

QMultiHash<QString, QSqlRecord>* MultiHashOfRecords::getMassive(){
    return this->m_multiHash;
}
