#ifndef COMPAREDBS_H
#define COMPAREDBS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <multihashofrecords.h>
#include <sizeoofmassiveofnotequalrows.h>

static QVector<int>* compareDbs(QSqlRelationalTableModel* &model1, QSqlRelationalTableModel* &model2){

    MultiHashOfRecords massiveOfRecors1(model1);
    MultiHashOfRecords massiveOfRecors2(model2);
    QString stationName = model1->record(1).field(0).value().toString();
    QAbstractItemView* table1 = new QTableWidget();
    QAbstractItemView* table2 = new QTableWidget();
    QSqlQuery query1("SELECT Stations.NameSt, TS_Name.NameTs, TS.Question, "
                                    "TS.Module, TS.I, TS.J, "
                                    "TS.Lock, TS.Pulse, TS.Inverse, "
                                    "TS.Occupation, TS.SvtfMain, TS.SvtfDiag, "
                                    "TS.SvtfClass, TS.SvtfError, "
                                    "TS.NoRc, TS.NoStrl, "
                                    "TS.NoSvtf, TS.ExtData, "
                                    "TS.StrlZsName, TS.StrlMuName "
                                    "FROM TS, TS_Name, Stations "
                                    "WHERE Stations.NameSt='"+stationName+"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt ORDER BY TS.Module,TS.I, TS.J, TS_Name.NameTs ASC;", (model1)->database());
    QSqlQuery query2("SELECT Stations.NameSt, TS_Name.NameTs, TS.Question, "
                                    "TS.Module, TS.I, TS.J, "
                                    "TS.Lock, TS.Pulse, TS.Inverse, "
                                    "TS.Occupation, TS.SvtfMain, TS.SvtfDiag, "
                                    "TS.SvtfClass, TS.SvtfError, "
                                    "TS.NoRc, TS.NoStrl, "
                                    "TS.NoSvtf, TS.ExtData, "
                                    "TS.StrlZsName, TS.StrlMuName "
                                    "FROM TS, TS_Name, Stations "
                                    "WHERE Stations.NameSt='"+stationName+"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt ORDER BY TS.Module,TS.I, TS.J, TS_Name.NameTs ASC;", (model2)->database());


    int* parameters1 = MultiHashOfRecords::getMaxParameters(model1);
    int* parameters2 = MultiHashOfRecords::getMaxParameters(model2);

    int maxModules = (parameters1[0] > parameters2[0]) ? parameters1[0] : parameters2[0];
    int maxI       = (parameters1[1] > parameters2[1]) ? parameters1[1] : parameters2[1];
    int maxJ       = (parameters1[2] > parameters2[2]) ? parameters1[2] : parameters2[2];

    int sizeoOfMassiveOfNotEqualRows = maxModules*maxI*maxJ*2;

    qDebug() << sizeoOfMassiveOfNotEqualRows << maxModules << maxI << maxJ;
    QVector<int>* vector = new QVector<int>;
    int vectorIndex = 0;

    int delete1 = 1;
//    int ***massiveOfNotEqualRows;
//    int **massiveOfNotEqualRows1;
//    int **massiveOfNotEqualRows2;
//    int **massiveOfNotEqualRows3 = new int* [1];
//    massiveOfNotEqualRows3[0] = new int [1];
//    massiveOfNotEqualRows3[0][0] = 0;
//    massiveOfNotEqualRows1 = new int* [sizeoOfMassiveOfNotEqualRows];
//    massiveOfNotEqualRows2 = new int* [sizeoOfMassiveOfNotEqualRows];
//    for (int i = 0; i < sizeoOfMassiveOfNotEqualRows; i++) {
//        massiveOfNotEqualRows1[i] = new int [4];
//        massiveOfNotEqualRows2[i] = new int [4];
//    }
//    massiveOfNotEqualRows = new int** [3];
//    massiveOfNotEqualRows[0] = massiveOfNotEqualRows1;
//    massiveOfNotEqualRows[1] = massiveOfNotEqualRows2;
//    massiveOfNotEqualRows[2] = massiveOfNotEqualRows3;
//    int index1 = 0;
//    int index2 = 0;
    for (int module = 0; module < maxModules ; module++){
        for(int i = 0; i < maxI; i++){
            for(int j = 0; j < maxJ; j++){
                static QString string[] = { "NameTs","Lock","Pulse",
                                    "Inverse","Occupation",
                                    "SvtfMain","SvtfDiag",
                                    "SvtfClass", "SvtfError",
                                    "NoRc", "NoStrl",
                                    "NoSvtf", "ExtData",
                                    "StrlZsName", "StrlMuName"
                        };
                QString key = QVariant(module).toString() + " " + QVariant(i).toString() + " " + QVariant(j).toString();
                qDebug() << key;
                qDebug() << "Проверка строки";
                QList<QSqlRecord> list1 = massiveOfRecors1.getMassive()->values(key);
                QList<QSqlRecord> list2 = massiveOfRecors2.getMassive()->values(key);
                if (list1.isEmpty() && !list2.isEmpty()){
                    qDebug() << "list1 пуст, а list2 нет";
                    for (int i = 0; i < list2.size(); i++){
                        QSqlRecord record2 = list2[i];
                        record2.field("id").setGenerated(false);
                        qDebug() << table1->insertRecord(-1, QSqlRecord());
                        qDebug() << table2->insertRecord(-1, record2);
                        qDebug() << record2;
                        vector->resize(vector->size() +1);
                        for (int k = 0; k < table1->rowCount(); k++){
                            if (table2->record(k).field("Module").value().toInt() == module &&
                                table2->record(k).field("I")     .value().toInt() == i      &&
                                table2->record(k).field("J")     .value().toInt() == j){
                                qDebug() << module << i << j << k;
                                vector->insert(vectorIndex++, k);
                                break;
                            }
                        }
                    }
//                    massiveOfNotEqualRows2[index2][0] = module;
//                    massiveOfNotEqualRows2[index2][1] = i;
//                    massiveOfNotEqualRows2[index2][2] = j;
//                    massiveOfNotEqualRows2[index2][3] = 1;

//                    massiveOfNotEqualRows1[index1][0] = module;
//                    massiveOfNotEqualRows1[index1][1] = i;
//                    massiveOfNotEqualRows1[index1][2] = j;
//                    massiveOfNotEqualRows1[index1][3] = 0;

//                    massiveOfNotEqualRows3[0][0]++;
//                    index1++;
//                    index2++;
                } else if (list2.isEmpty() && !list1.isEmpty()){
                    qDebug() << "list2 пуст, а list1 нет";
                    for (int i = 0; i < list1.size(); i++){
                        QSqlRecord record1 = list1[i];
                        record1.field("id").setGenerated(false);
                        qDebug() << table2->insertRecord(-1, QSqlRecord());
                        qDebug() << table1->insertRecord(-1, record1);
                        qDebug() << record1;
                        qDebug() << table1->lastError();
                        qDebug() << table2->lastError();
                        vector->resize(vector->size() +1);
                        for (int k = 0; k < table1->rowCount(); k++){
                            if (table1->record(k).field("Module").value().toInt() == module &&
                                table1->record(k).field("I")     .value().toInt() == i      &&
                                table1->record(k).field("J")     .value().toInt() == j){
                                qDebug() << module << i << j << k;
                                vector->insert(vectorIndex++, k);
                                break;
                            }
                        }
                    }
//                    massiveOfNotEqualRows2[index2][0] = module;
//                    massiveOfNotEqualRows2[index2][1] = i;
//                    massiveOfNotEqualRows2[index2][2] = j;
//                    massiveOfNotEqualRows2[index2][3] = 0;

//                    massiveOfNotEqualRows1[index1][0] = module;
//                    massiveOfNotEqualRows1[index1][1] = i;
//                    massiveOfNotEqualRows1[index1][2] = j;
//                    massiveOfNotEqualRows1[index1][3] = 1;

//                    index1++;
//                    index2++;
//                    massiveOfNotEqualRows3[0][0]++;
                } else if (list2.isEmpty() && list1.isEmpty()){
                    qDebug() << "list1 пуст и list2 пуст";
                } else {
                    qDebug() << "list1 не пуст и list2 не пуст";
                    for (int i = 0; i < list1.size(); i++){
                        QSqlRecord record1 = list1[i];
                        qDebug() << "Проход по list1";
                        for (int i = 0; i < list2.size(); i++){
                            QSqlRecord record2 = list2[i];
                            qDebug() << "Проход по list2";
                            if(!record1.isEmpty()){
                                qDebug() << "record1 существует";
                                if(!record2.isEmpty()){
                                    qDebug() << "record2 существует";
                                    int test = 0;
                                    for (QString s : string){
                                        qDebug() << "Проход по массиву строк сравнения";
                                        if (record1.field(s).value() != record2.field(s).value()){

                                            qDebug() << record1.field(s).value();
                                            qDebug() << record2.field(s).value();

//                                            massiveOfNotEqualRows1[index1][0] = module;
//                                            massiveOfNotEqualRows1[index1][1] = i;
//                                            massiveOfNotEqualRows1[index1][2] = j;
//                                            massiveOfNotEqualRows1[index1][3] = 1;

//                                            massiveOfNotEqualRows2[index2][0] = module;
//                                            massiveOfNotEqualRows2[index2][1] = i;
//                                            massiveOfNotEqualRows2[index2][2] = j;
//                                            massiveOfNotEqualRows2[index2][3] = 1;

//                                            index1++;
//                                            index2++;
//                                            massiveOfNotEqualRows3[0][0]++;
                                            test = 1;
                                            break;
                                        }
                                    }
                                    if (test == 0){
                                        record1.field("id").setGenerated(false);
                                        record2.field("id").setGenerated(false);
                                        qDebug() << table1->insertRecord(-1, record1);
                                        qDebug() << table2->insertRecord(-1, record2);
                                        qDebug() << record1;
                                        qDebug() << record2;
                                    } else {
                                        record1.field("id").setGenerated(false);
                                        record2.field("id").setGenerated(false);
                                        qDebug() << table1->insertRecord(-1, record1);
                                        qDebug() << table2->insertRecord(-1, record2);
                                        qDebug() << record1;
                                        qDebug() << record2;
                                        vector->resize(vector->size() +1);
                                        for (int k = 0; k < table1->rowCount(); k++){
                                            if (table1->record(k).field("Module").value().toInt() == module &&
                                                table1->record(k).field("I")     .value().toInt() == i      &&
                                                table1->record(k).field("J")     .value().toInt() == j){
                                                qDebug() << module << i << j << k;
                                                vector->insert(vectorIndex++, k);
                                                break;
                                            }
                                        }
                                        test = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //    table1->setRelation(0, QSqlRelation("Stations", "NoSt", "NameSt"));
    //    table1->setRelation(1, QSqlRelation("TS_Name", "Cod", "NameTS"));
    //    table2->setRelation(0, QSqlRelation("Stations", "NoSt", "NameSt"));
    //    table2->setRelation(1, QSqlRelation("TS_Name", "Cod", "NameTS"));
    model1 = table1;
    model2 = table2;
    qDebug() << vector->size() << "Размер";
    for (int i = 0; i < vector->size(); i++){
        qDebug() << vector->at(i);
    }
    return vector;
}

static int findCell (QSqlRelationalTableModel* model, int module, int i, int j){
    for (int k = 0; k < model->rowCount(); k++){
        if (model->record(k).field("Module").value().toInt() == module &&
            model->record(k).field("I")     .value().toInt() == i      &&
            model->record(k).field("J")     .value().toInt() == j){
            return k;
        }
    }
    return -1;
}


#endif // COMPAREDBS_H
