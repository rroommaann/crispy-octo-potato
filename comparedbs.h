#ifndef COMPAREDBS_H
#define COMPAREDBS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTranslator>
#include <multihashofrecords.h>

static QVector<QTableWidget*>* compareDbs(QSqlRelationalTableModel* &model1, QSqlRelationalTableModel* &model2){

    MultiHashOfRecords massiveOfRecors1(model1);
    MultiHashOfRecords massiveOfRecors2(model2);
    QString stationName = model1->record(1).field(0).value().toString();
    qDebug() << stationName;
    QString query = "SELECT Stations.NameSt, TS_Name.NameTs, TS.Question, "
                    "TS.Module, TS.I, TS.J, "
                    "TS.Lock, TS.Pulse, TS.Inverse, "
                    "TS.Occupation, TS.SvtfMain, TS.SvtfDiag, "
                    "TS.SvtfClass, TS.SvtfError, "
                    "TS.NoRc, TS.NoStrl, "
                    "TS.NoSvtf, TS.ExtData, "
                    "TS.StrlZsName, TS.StrlMuName "
                    "FROM TS, TS_Name, Stations "
                    "WHERE Stations.NameSt='"+stationName+"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt ORDER BY TS.Module,TS.I, TS.J, TS_Name.NameTs ASC;";

    QSqlQuery query1(model1->database());
    query1.prepare(query);
    query1.exec();
    qDebug() << query1.isActive();

    QSqlQuery query2(model2->database());
    query2.prepare(query);
    query2.exec();
    qDebug() << query2.isActive();

    int* parameters1 = MultiHashOfRecords::getMaxParameters(model1);
    int* parameters2 = MultiHashOfRecords::getMaxParameters(model2);

    int maxModules = (parameters1[0] > parameters2[0]) ? parameters1[0] : parameters2[0];
    int maxI       = (parameters1[1] > parameters2[1]) ? parameters1[1] : parameters2[1];
    int maxJ       = (parameters1[2] > parameters2[2]) ? parameters1[2] : parameters2[2];

    QTableWidget* table1 = new QTableWidget;
    QTableWidget* table2 = new QTableWidget;
    static QStringList  string = { "NameSt","NameTs",
                        "Question", "Module", "I", "J",
                        "Lock","Pulse",
                        "Inverse","Occupation",
                        "SvtfMain","SvtfDiag",
                        "SvtfClass", "SvtfError",
                        "NoRc", "NoStrl",
                        "NoSvtf", "ExtData",
                        "StrlZsName", "StrlMuName"
    };
    table1->setColumnCount(query1.record().count());
    table2->setColumnCount(query2.record().count());
    for (int i = 0; i < string.size(); i++){
        table1->setHorizontalHeaderItem(i, new QTableWidgetItem);
        table2->setHorizontalHeaderItem(i, new QTableWidgetItem);
    }
    table1->setHorizontalHeaderLabels(string);
    table2->setHorizontalHeaderLabels(string);
    QVector<QTableWidget*>* vector = new QVector<QTableWidget*>(2);
    vector->insert(0,table1);
    vector->insert(1,table2);
    int rows = 0;
    table1->setRowCount(rows);
    table2->setRowCount(rows);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    QColor color = "#FF9BAA";
    brush.setColor(color);
    for (int module = 0; module < maxModules ; module++){
        for(int i = 0; i < maxI; i++){
            for(int j = 0; j < maxJ; j++){
                QString key = QVariant(module).toString() + " " + QVariant(i).toString() + " " + QVariant(j).toString();
                qDebug() << key;
                qDebug() << "Проверка строки";
                QList<QSqlRecord> list1 = massiveOfRecors1.getMassive()->values(key);
                QList<QSqlRecord> list2 = massiveOfRecors2.getMassive()->values(key);
                if (list1.isEmpty() && !list2.isEmpty()){
                    qDebug() << "list1 пуст, а list2 нет";
                    for (int i = 0; i < list2.size(); i++){
                        QSqlRecord record = list2[i];
                        rows++;
                        table1->setRowCount(rows);
                        table2->setRowCount(rows);
                        for(int col = 0; col < table1->columnCount(); col++){
                            QString fieldName = string[col];
                            QTableWidgetItem *newItem1 = new QTableWidgetItem("*",0);
                            QTableWidgetItem *newItem2 = new QTableWidgetItem(record.field(fieldName).value().toString(),0);
                            newItem1->setBackground(brush);
                            newItem2->setBackground(brush);
                            table1->setItem(rows-1,col, newItem1);
                            table2->setItem(rows-1,col, newItem2);
                        }
                    }
                } else if (list2.isEmpty() && !list1.isEmpty()){
                    qDebug() << "list2 пуст, а list1 нет";
                    for (int i = 0; i < list1.size(); i++){
                        QSqlRecord record = list1[i];
                        rows++;
                        table1->setRowCount(rows);
                        table2->setRowCount(rows);
                        for(int col = 0; col < table1->columnCount(); col++){
                            QString fieldName = string[col];
                            QTableWidgetItem *newItem1 = new QTableWidgetItem(record.field(fieldName).value().toString(),0);
                            QTableWidgetItem *newItem2 = new QTableWidgetItem("*", 0);
                            newItem1->setBackground(brush);
                            newItem2->setBackground(brush);
                            table1->setItem(rows-1,col, newItem1);
                            table2->setItem(rows-1,col, newItem2);
                        }
                    }
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
                                        if (record1.field(s).value() != record2.field(s).value().toString()){
                                            test = 1;
                                            break;
                                        }
                                    }
                                    if (test == 0){
                                        rows++;
                                        table1->setRowCount(rows);
                                        table2->setRowCount(rows);
                                        for(int col = 0; col < table1->columnCount(); col++){
                                            QString fieldName = string[col];
                                            QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(),0);
                                            QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                                            table1->setItem(rows-1,col, newItem1);
                                            table2->setItem(rows-1,col, newItem2);
                                        }
                                    } else {
                                        rows++;
                                        table1->setRowCount(rows);
                                        table2->setRowCount(rows);
                                        for(int col = 0; col < table1->columnCount(); col++){
                                            QString fieldName = string[col];
                                            QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(),0);
                                            QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                                            newItem1->setBackground(brush);
                                            newItem2->setBackground(brush);
                                            table1->setItem(rows-1,col, newItem1);
                                            table2->setItem(rows-1,col, newItem2);
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
