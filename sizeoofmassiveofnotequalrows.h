#ifndef SIZEOOFMASSIVEOFNOTEQUALROWS_H
#define SIZEOOFMASSIVEOFNOTEQUALROWS_H

#include <QSqlRelationalTableModel>
#include <multihashofrecords.h>

static int sizeOfMassives(QSqlRelationalTableModel* model1, QSqlRelationalTableModel* model2){

    int* parameters1 = MultiHashOfRecords::getMaxParameters(model1);
    int* parameters2 = MultiHashOfRecords::getMaxParameters(model2);

    int maxModules = (parameters1[0] > parameters2[0]) ? parameters1[0] : parameters2[0];
    int maxI       = (parameters1[1] > parameters2[1]) ? parameters1[1] : parameters2[1];
    int maxJ       = (parameters1[2] > parameters2[2]) ? parameters1[2] : parameters2[2];

    return (maxModules*maxI*maxJ*2);
}




#endif // SIZEOOFMASSIVEOFNOTEQUALROWS_H
