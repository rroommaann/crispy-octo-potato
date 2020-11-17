#ifndef WIDGET_H
#define WIDGET_H

#include "mytable.h"

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QComboBox>
#include <QUrl>
#include <QFileInfo>
#include <QtSql>
#include <QDebug>
#include <QFile>
#include <QTimer>

#include "multihashofrecords.h"
#include "comparedbs.h"

#include <QSizePolicy>

#include "formcolumns.h"


namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QStringList getListOfColumns(bool diff);

private:
    Ui::Widget *ui;

    bool m_isTableLeftSet = false;
    bool m_isTableRightSet = false;
    bool m_isCompared = false;

    void initialize ();

    MyTable* m_tableLeft = nullptr;
    MyTable* m_tableRight = nullptr;

    QTableWidgetItem* m_item1 = nullptr;
    QTableWidgetItem* m_item2 = nullptr;

    QBrush m_brushGreen;
    QBrush m_brush1;
    QBrush m_brush2;

    tableType selectedType = TS;

    FormColumns *form = nullptr;
    QComboBox *comboBox = nullptr;

    void setTable(QString name, QString q);

private slots:
    void doubleClickedTableLeftItem(QTableWidgetItem *item);
    void doubleClickedTableRightItem(QTableWidgetItem *item);
    void open();
    void compareDBs();
    void reset();
    void openColumns();
};

#endif // WIDGET_H
