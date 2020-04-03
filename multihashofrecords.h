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
    MultiHashOfRecords(QSqlQuery *query, QString key);
    MultiHashOfRecords();
    ~MultiHashOfRecords();
    QMap<QString, QSqlRecord>* getMassive();

    QStringList getColumns() const;

private:
    QMap <QString, QSqlRecord>* m_multiHash;
    QStringList columns;
};



#endif // MASSIVEOFRECORDS_H
