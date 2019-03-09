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

    static int* getMaxParameters(QSqlRelationalTableModel* model){
        int* tempMass = new int[3];
        int maxModule = 0;
        int maxI = 0;
        int maxJ = 0;
        for(int i = 0; i < model->rowCount(); i++){
            if (model->record(i).field("Module").value().toInt() > maxModule){
                maxModule = model->record(i).field("Module").value().toInt();
            }
            if (model->record(i).field("I").value().toInt() > maxI){
                maxI = model->record(i).field("I").value().toInt();
            }
            if (model->record(i).field("J").value().toInt() > maxJ){
                maxJ = model->record(i).field("J").value().toInt();
            }
        }
        tempMass[0] = ++maxModule;
        tempMass[1] = ++maxI;
        tempMass[2] = ++maxJ;
        return tempMass;

    }

private:
    QMultiHash <QString, QSqlRecord>* m_multiHash;
};



#endif // MASSIVEOFRECORDS_H
