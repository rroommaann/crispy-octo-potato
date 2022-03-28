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
    TuSpok = 3,
    RouteSrc = 4,
    Stations = 0,
    };

static QString prepareQuery(const QString &stationName, const QStringList &list, tableType type)
{
    QString query;
    if(type == TS)
    {
        query = QStringLiteral(u"SELECT [Stations.NameSt], [TS_Name.NameTs");
        for(const auto &column : list)
        {
            QString temp = QStringLiteral(u"], [TS.");
            temp.append(column);
            query.append(temp);
        }
        query.append(QStringLiteral(u"] FROM TS, TS_Name, Stations"
                                    " WHERE Stations.NameSt='") +
                     stationName +
                     QStringLiteral(u"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt"
                                    " ORDER BY [TS_Name.NameTs] ASC;"));
    }
    else if (type == TU)
    {
        query = QStringLiteral(u"SELECT [Stations.NameSt], [TU_Name.NameTu");
        for(const auto &column : list)
        {
            QString temp = QStringLiteral(u"], [TU.");
            temp.append(column);
            query.append(temp);
        }
        query.append(QStringLiteral(u"] FROM TU, TU_Name, Stations"
                                    " WHERE Stations.NameSt='") +
                     stationName +
                     QStringLiteral(u"' AND TU.Cod=TU_Name.Cod AND TU.NoSt=Stations.NoSt"
                                    " ORDER BY [TU_Name.NameTu] ASC;"));
    }
    else if(type == Stations)
    {
        query = QStringLiteral(u"SELECT * "
                               "FROM Stations "
                               "ORDER BY [NoSt] ASC;");
    }
    else if(type == RouteSrc)
    {
        query = QStringLiteral(u"SELECT [Stations.NameSt");
        for(const auto &column : list)
        {
            QString temp = QStringLiteral(u"], [RouteSrc.");
            temp.append(column);
            query.append(temp);
        }
        query.append(QStringLiteral(u"] FROM RouteSrc, Stations"
                                    " WHERE Stations.NameSt='") +
                     stationName +
                     QStringLiteral(u"' AND RouteSrc.NoSt=Stations.NoSt"
                                    " ORDER BY [No] ASC;"));
    }
    else if(type == TuSpok)
    {
        query = QStringLiteral(u"SELECT [Stations.NameSt], [TuSpok.No");
        for(const auto &column : list)
        {
            QString temp = QStringLiteral(u"], [TuSpok.");
            temp.append(column);
            query.append(temp);
        }
        if(list.contains(QStringLiteral(u"NoSt2")))
            query.append(QStringLiteral(u"] FROM TuSpok, Stations"
                                        " WHERE Stations.NameSt='") +
                         stationName +
                         QStringLiteral(u"' AND TuSpok.NoSt=Stations.NoSt"
                                        " ORDER BY [TuSpok.No] ASC;"));
        else
            query.append(QStringLiteral(u"] FROM TuSpok, Stations"
                                        " WHERE Stations.NameSt='") +
                         stationName +
                         QStringLiteral(u"' AND TuSpok.NoSt=Stations.NoSt"
                                        " ORDER BY [TuSpok.No] ASC;"));
    }
    qDebug() << query;
    return query;
}

static QStringList prepareColumnsList(QStringList list, tableType type)
{
    QStringList listOfColumns;
    QString nameSt = QStringLiteral(u"NameSt");
    if(type == TS)
        listOfColumns = QStringList{nameSt, QStringLiteral(u"NameTs")};
    else if (type == TU)
        listOfColumns = QStringList{nameSt, QStringLiteral(u"NameTu")};
    else if (type == RouteSrc)
        listOfColumns = QStringList{nameSt};
    else if( type == TuSpok)
        listOfColumns = QStringList{nameSt, QStringLiteral(u"No")};

    listOfColumns.append(list);
    return listOfColumns;
}

static QVector<int> compareDbs(MultiHashOfRecords *hash1, MultiHashOfRecords *hash2, QTableWidget *table1, QTableWidget *table2, tableType type = TS, QStringList list = QStringList(), bool allStations = false)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    QColor color(QStringLiteral(u"#FF9BAA"));
    brush.setColor(color);

    QBrush diffBrush;
    diffBrush.setStyle(Qt::SolidPattern);
    QColor diffColor(QStringLiteral(u"#FF0000"));
    diffBrush.setColor(diffColor);

    int rows = 0;
    QVector<int> vectorOfDiffRows;
    if(allStations)
        rows = table1->rowCount();
    else
    {
        table1->clear();
        table2->clear();
    }
    table1->setRowCount(rows);
    table2->setRowCount(rows);

    QStringList listOfColumns = prepareColumnsList(list, type);

    int columnCount = listOfColumns.count();
    table1->setColumnCount(columnCount);
    table2->setColumnCount(columnCount);

    int sizeOfColumnList = listOfColumns.size();
    for(int i = 0; i < sizeOfColumnList; ++i)
    {
        table1->setHorizontalHeaderItem(i, new QTableWidgetItem());
        table2->setHorizontalHeaderItem(i, new QTableWidgetItem());
    }
    table1->setHorizontalHeaderLabels(listOfColumns);
    table2->setHorizontalHeaderLabels(listOfColumns);

    auto keys = hash1->getMassive()->keys();
    for(const auto &key : qAsConst(keys))
    {
        QList<QSqlRecord> list1 = hash1->getMassive()->values(key);
        QList<QSqlRecord> list2 = hash2->getMassive()->values(key);
        if(list1.isEmpty() && !list2.isEmpty())
        {
            int sizeOfList2 = list2.size();
            for(int i = 0; i < sizeOfList2; ++i)
            {
                QSqlRecord record = list2[i];
                rows++;
                table1->setRowCount(rows);
                table2->setRowCount(rows);
                for(int col = 0; col < columnCount; ++col)
                {
                    QString fieldName = listOfColumns[col];
                    QTableWidgetItem *newItem1 = new QTableWidgetItem("*", 0);
                    QTableWidgetItem *newItem2 = new QTableWidgetItem(record.field(fieldName).value().toString(), 0);
                    newItem1->setBackground(brush);
                    newItem2->setBackground(brush);
                    table1->setItem(rows - 1, col, newItem1);
                    table2->setItem(rows - 1, col, newItem2);
                }
                vectorOfDiffRows << (rows - 1);
            }
        }
        else if(list2.isEmpty() && !list1.isEmpty())
        {
            int sizeOfList1 = list1.size();
            for(int i = 0; i < sizeOfList1; ++i)
            {
                QSqlRecord record = list1[i];
                rows++;
                table1->setRowCount(rows);
                table2->setRowCount(rows);
                int columnCount1 = table1->columnCount();
                for(int col = 0; col < columnCount1; ++col)
                {
                    QString fieldName = listOfColumns[col];
                    QTableWidgetItem *newItem1 = new QTableWidgetItem(record.field(fieldName).value().toString(), 0);
                    QTableWidgetItem *newItem2 = new QTableWidgetItem("*", 0);
                    newItem1->setBackground(brush);
                    newItem2->setBackground(brush);
                    table1->setItem(rows - 1, col, newItem1);
                    table2->setItem(rows - 1, col, newItem2);
                }
                vectorOfDiffRows << (rows - 1);
            }
        }
        else
        {
            int maxListSize = (list1.size() >= list2.size()) ? list1.size() : list2.size();
            for(int i = 0; i < maxListSize; ++i)
            {
                QSqlRecord record1;
                QSqlRecord record2;
                if(list1.size() > list2.size())
                {
                    record1 = list1[i];
                    record2 = (i >= list2.size()) ? QSqlRecord() : list2[i];
                } else if(list2.size() > list1.size())
                {
                    record2 = list1[i];
                    record1 = i >= list1.size() ? QSqlRecord() : list1[i];
                } else if(list1.size() == list2.size())
                {
                    record1 = list1[i];
                    record2 = list2[i];
                }

                if(!record1.isEmpty() && !record2.isEmpty())
                {
                    bool differentRow = false;
                    for(const auto &s : qAsConst(listOfColumns))
                    {
                        if(record1.field(s).value().toString() != record2.field(s).value().toString())
                        {
                            differentRow = true;
                            break;
                        }
                    }
                    if(!differentRow && !allStations)
                    {
                        rows++;
                        table1->setRowCount(rows);
                        table2->setRowCount(rows);
                        for(int col = 0; col < columnCount; ++col)
                        {
                            QString fieldName = listOfColumns[col];
                            QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(), 0);
                            QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(), 0);
                            table1->setItem(rows - 1, col, newItem1);
                            table2->setItem(rows - 1, col, newItem2);
                        }
                    }
                    else if(differentRow)
                    {
                        rows++;
                        table1->setRowCount(rows);
                        table2->setRowCount(rows);
                        for(int col = 0; col < columnCount; ++col)
                        {
                            QString fieldName = listOfColumns[col];
                            QTableWidgetItem *newItem1 = new QTableWidgetItem(record1.field(fieldName).value().toString(), 0);
                            QTableWidgetItem *newItem2 = new QTableWidgetItem(record2.field(fieldName).value().toString(), 0);

                            bool differentColumn = record1.field(fieldName).value().toString() != record2.field(fieldName).value().toString();

                            newItem1->setBackground(differentColumn ? diffBrush : brush);
                            newItem2->setBackground(differentColumn ? diffBrush : brush);

                            table1->setItem(rows - 1, col, newItem1);
                            table2->setItem(rows - 1, col, newItem2);
                        }
                        vectorOfDiffRows << (rows - 1);
                        differentRow = false;
                    }
                }
            }
        }
    }

    keys = hash2->getMassive()->keys();
    for(const auto &key : qAsConst(keys))
    {
        if(!hash1->getMassive()->keys().contains(key))
        {
            QList<QSqlRecord> list1 = hash1->getMassive()->values(key);
            QList<QSqlRecord> list2 = hash2->getMassive()->values(key);
            if(list1.isEmpty() && !list2.isEmpty())
            {
                int sizeOfList2 = list2.size();
                for(int i = 0; i < sizeOfList2; ++i)
                {
                    QSqlRecord record = list2[i];
                    rows++;
                    table1->setRowCount(rows);
                    table2->setRowCount(rows);
                    for(int col = 0; col < columnCount; ++col)
                    {
                        QString fieldName = listOfColumns[col];
                        QTableWidgetItem *newItem1 = new QTableWidgetItem("*", 0);
                        QTableWidgetItem *newItem2 = new QTableWidgetItem(record.field(fieldName).value().toString(), 0);
                        newItem1->setBackground(brush);
                        newItem2->setBackground(brush);
                        table1->setItem(rows - 1, col, newItem1);
                        table2->setItem(rows - 1, col, newItem2);
                    }
                    vectorOfDiffRows << (rows - 1);
                }
            }
        }
    }
    return vectorOfDiffRows;
}
#endif // COMPAREDBS_H
