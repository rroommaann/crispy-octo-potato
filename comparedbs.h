#ifndef COMPAREDBS_H
#define COMPAREDBS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTranslator>
#include <QProgressDialog>
#include <QTimer>
#include <multihashofrecords.h>

enum tableType
{
    def = 0,
    TS = 1,
    TU = 2,
    Stations = 10,
};

static QString prepareQuery(QString stationName, QStringList list, tableType type)
{
    QString query;
    if(type == TS)
    {
        query = "SELECT Stations.NameSt, TS_Name.NameTs";
        for(auto column : list)
        {
            QString temp = ", TS.";
            temp.append(column);
            query.append(temp);
        }
        query.append(" FROM TS, TS_Name, Stations WHERE Stations.NameSt='"+stationName+"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt ORDER BY TS_Name.NameTs ASC;");
    }
    else if (type == TU)
    {
        query = "SELECT Stations.NameSt, TU_Name.NameTu";
        for(auto column : list)
        {
            QString temp = ", TU.";
            temp.append(column);
            query.append(temp);
        }
        query.append(" FROM TU, TU_Name, Stations"
                     " WHERE Stations.NameSt='"+stationName+"' AND TU.Cod=TU_Name.Cod AND TU.NoSt=Stations.NoSt"
                     " ORDER BY TU_Name.NameTu ASC;");
    }
    else if(type == Stations)
    {
        query = "SELECT * "
                "FROM Stations "
                "ORDER BY NoSt ASC;";
    }
    qDebug() << query;
    return query;
}

static QStringList prepareColumnsList (QStringList list, tableType type)
{
    QStringList listOfColumns;
    if(type == TS)
    {
        listOfColumns = QStringList{"NameSt","NameTs"};
    }
    else if (type == TU)
    {
        listOfColumns = QStringList{"NameSt","NameTu"};
    }
    listOfColumns.append(list);
    return listOfColumns;
}

static void compareDbs(MultiHashOfRecords hash1, MultiHashOfRecords hash2, QTableWidget *table1, QTableWidget *table2, tableType type = TS, QStringList list = QStringList())
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    QColor color = "#FF9BAA";
    brush.setColor(color);

    QBrush diffBrush;
    diffBrush.setStyle(Qt::SolidPattern);
    QColor diffColor = "#FF0000";
    diffBrush.setColor(diffColor);
    table1->clear();
    table2->clear();

    int rows = 0;
    table1->setRowCount(rows);
    table2->setRowCount(rows);

    MultiHashOfRecords massiveOfRecors1 = hash1;
    MultiHashOfRecords massiveOfRecors2 = hash2;

    QStringList listOfColumns = prepareColumnsList(list, type);

    int columnCount = listOfColumns.count();
    table1->setColumnCount(columnCount);
    table2->setColumnCount(columnCount);

    for (int i = 0; i < listOfColumns.size(); i++)
    {
        table1->setHorizontalHeaderItem(i, new QTableWidgetItem());
        table2->setHorizontalHeaderItem(i, new QTableWidgetItem());
    }
    table1->setHorizontalHeaderLabels(listOfColumns);
    table2->setHorizontalHeaderLabels(listOfColumns);


    QStringList keys1 = massiveOfRecors1.getMassive()->keys();
    QStringList keys2 = massiveOfRecors2.getMassive()->keys();
    for(auto key : keys1)
    {
        QSqlRecord record1 = massiveOfRecors1.getMassive()->value(key);
        QSqlRecord record2 = massiveOfRecors2.getMassive()->value(key);

        if(!record1.isEmpty())
        {
            if(!record2.isEmpty())
            {
                bool isDiff = 0;
                for (QString s : listOfColumns)
                {
                    if (record1.field(s).value().toString() != record2.field(s).value().toString())
                    {
                        isDiff = true;
                        break;
                    }
                }
                if (!isDiff)
                {
                    rows++;
                    table1->setRowCount(rows);
                    table2->setRowCount(rows);
                    for(int col = 0; col < columnCount; col++)
                    {
                        QString fieldName = listOfColumns[col];
                        QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(),0);
                        QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                        table1->setItem(rows-1,col, newItem1);
                        table2->setItem(rows-1,col, newItem2);
                    }
                }
                else if (isDiff)
                {
                    rows++;
                    table1->setRowCount(rows);
                    table2->setRowCount(rows);
                    for(int col = 0; col < columnCount; col++)
                    {
                        QString fieldName = listOfColumns[col];
                        QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(),0);
                        QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                        if (record1.field(fieldName).value().toString() != record2.field(fieldName).value().toString())
                        {
                            newItem1->setBackground(diffBrush);
                            newItem2->setBackground(diffBrush);
                        }
                        else
                        {
                            newItem1->setBackground(brush);
                            newItem2->setBackground(brush);
                        }
                        table1->setItem(rows-1,col, newItem1);
                        table2->setItem(rows-1,col, newItem2);
                    }
                    isDiff = false;
                }
            }
            else
            {
                rows++;
                table1->setRowCount(rows);
                table2->setRowCount(rows);
                for(int col = 0; col < columnCount; col++)
                {
                    QString fieldName = listOfColumns[col];
                    QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(),0);
                    QTableWidgetItem *newItem2 = new QTableWidgetItem("*",0);
                    newItem1->setBackground(brush);
                    newItem2->setBackground(brush);
                    table1->setItem(rows - 1, col, newItem1);
                    table2->setItem(rows - 1, col, newItem2);
                }
            }
        }
        else if(!record2.isEmpty())
        {
            rows++;
            table1->setRowCount(rows);
            table2->setRowCount(rows);
            for(int col = 0; col < columnCount; col++)
            {
                QString fieldName = listOfColumns[col];
                QTableWidgetItem *newItem1 = new QTableWidgetItem("*",0);
                QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                newItem1->setBackground(brush);
                newItem2->setBackground(brush);
                table1->setItem(rows - 1, col, newItem1);
                table2->setItem(rows - 1, col, newItem2);
            }
        }
    }

    for(auto key : keys2)
    {
        if(!keys1.contains(key))
        {
            QSqlRecord record1 = massiveOfRecors1.getMassive()->value(key);
            QSqlRecord record2 = massiveOfRecors2.getMassive()->value(key);
            if(record1.isEmpty())
            {
                rows++;
                table1->setRowCount(rows);
                table2->setRowCount(rows);
                for(int col = 0; col < columnCount; col++)
                {
                    QString fieldName = listOfColumns[col];
                    QTableWidgetItem *newItem1 = new QTableWidgetItem("*",0);
                    QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(),0);
                    newItem1->setBackground(brush);
                    newItem2->setBackground(brush);
                    table1->setItem(rows - 1, col, newItem1);
                    table2->setItem(rows - 1, col, newItem2);
                }
            }
        }
    }

    table1->resizeRowsToContents();
    table1->resizeColumnsToContents();
    table2->resizeRowsToContents();
    table2->resizeColumnsToContents();
}
#endif // COMPAREDBS_H
