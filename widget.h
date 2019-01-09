#ifndef WIDGET_H
#define WIDGET_H

#include "mytableview.h"

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QComboBox>
#include <QUrl>
#include <QFileInfo>
#include <QtSql>
#include <QDebug>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:


    QBoxLayout* m_mainLayout;
    QList<QUrl>* m_filePath;

    QPushButton* m_openButtonLeft;
    QPushButton* m_openButtonRight;

    QPushButton* m_closeButtonLeft;
    QPushButton* m_closeButtonRight;

    QPushButton* m_compareButton;

    MyTable* m_tableLeft;
    MyTable* m_tableRight;

    QLabel* m_nameLabelLeft;
    QLabel* m_nameLabelRight;

    QBoxLayout* m_buttonLayout;
    QBoxLayout* m_tablesLayout;
    QBoxLayout* m_labelLayout;
    QBoxLayout* m_labelLayoutLeft;
    QBoxLayout* m_labelLayoutRight;

    QComboBox* m_comboBoxLeft;
    QComboBox* m_comboBoxRight;

    QStringList* m_stationsList1;
    QStringList* m_stationsList2;

    void setTable(QString name, QString q);
    void sortMassive(int** massive, int **massive1, int sizeoOfMassiveOfNotEqualRows, QSqlRelationalTableModel* model1, QSqlRelationalTableModel *model2);

private slots:
    void chooseStation(QString string);
    void open();
    void close();
    void CompareDBs();

};

#endif // WIDGET_H
