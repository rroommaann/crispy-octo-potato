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
    MultiHashOfRecords(QSqlQuery &query, const QString &key);
    ~MultiHashOfRecords();
    QMultiMap<QVariant, QSqlRecord> *getMassive();

    QStringList getColumns() const;

private:
    QMultiMap <QVariant, QSqlRecord>* m_multiHash;
    QStringList columns;
};



#endif // MASSIVEOFRECORDS_H
