#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>
#include <QtWidgets>
#include <QSql>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QMouseEvent>
#include <QSqlRelationalDelegate>

class MyTable : public QWidget
{
    Q_OBJECT

public:
    explicit MyTable(QString tableName, QWidget *parent = nullptr);
    MyTable(QWidget *parent = nullptr);
    QTableView * getView ();
    QSqlRelationalTableModel *getModel();
    void setStation(QString q);
    void setView(QTableView* &);
    ~MyTable();

private:
    int m_temp = 0;
    bool checkDB();
    void setViewReady();
    QSqlRelationalTableModel* m_model2;
    QTableView* m_view;
    QString m_nameDB;
};

#endif
