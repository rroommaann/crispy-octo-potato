#ifndef MASSIVEOFRECORDS_H
#define MASSIVEOFRECORDS_H
#include <QVector>
#include <QSql>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlRelationalTableModel>
#include <QDebug>


class MultiHashOfRecords
{
public:
    MultiHashOfRecords(QSqlRelationalTableModel *model);
    MultiHashOfRecords();
    ~MultiHashOfRecords();
    QMultiHash<QString, QSqlRecord>* getMassive();

private:
    QMultiHash <QString, QSqlRecord>* m_multiHash;
};



#endif // MASSIVEOFRECORDS_H
