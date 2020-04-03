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

#include "formstations.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:

    boolean m_isTableLeftSet = false;
    boolean m_isTableRightSet = false;
    boolean m_isCompared = false;

    void initialize ();
    QBoxLayout* m_mainLayout = nullptr;
    QList<QUrl>* m_filePath = nullptr;

    QPushButton* m_openButtonLeft = nullptr;
    QPushButton* m_openButtonRight = nullptr;;

    QPushButton* m_closeButtonLeft = nullptr;
    QPushButton* m_closeButtonRight = nullptr;

    QPushButton* m_resetButton = nullptr;
    QPushButton* m_stationsButton = nullptr;

    QPushButton* m_compareButton = nullptr;

    MyTable* m_tableLeft = nullptr;
    MyTable* m_tableRight = nullptr;
    QTableWidget* m_tableLeftView = nullptr;
    QTableWidget* m_tableRightView = nullptr;

    QLabel* m_nameLabelLeft = nullptr;
    QLabel* m_nameLabelRight = nullptr;

    QBoxLayout* m_buttonLayout = nullptr;
    QBoxLayout* m_tablesLayout = nullptr;
    QBoxLayout* m_labelLayout = nullptr;
    QBoxLayout* m_labelLayoutLeft = nullptr;
    QBoxLayout* m_labelLayoutRight = nullptr;

    QVector<QTableWidget*>* m_vector = nullptr;

    QTableWidgetItem* m_item1 = nullptr;
    QTableWidgetItem* m_item2 = nullptr;

    QBrush m_brushGreen;
    QBrush m_brush1;
    QBrush m_brush2;

    FormStations *form = nullptr;

    void setTable(QString name, QString q);

private slots:
    void doubleClickedTableLeftItem(QTableWidgetItem *item);
    void doubleClickedTableRightItem(QTableWidgetItem *item);
    void open();
    void close();
    void compareDBs();
    void setIsCompared();
    void reset();
    void openColumns();
};

#endif // WIDGET_H
