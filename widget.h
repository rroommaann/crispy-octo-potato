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
    QBoxLayout* m_mainLayout;
    QList<QUrl>* m_filePath;

    QPushButton* m_openButtonLeft;
    QPushButton* m_openButtonRight;

    QPushButton* m_closeButtonLeft;
    QPushButton* m_closeButtonRight;

    QPushButton* m_resetButton;

    QPushButton* m_compareButton;

    MyTable* m_tableLeft;
    MyTable* m_tableRight;
    QTableWidget* m_tableLeftView;
    QTableWidget* m_tableRightView;

    QLabel* m_nameLabelLeft;
    QLabel* m_nameLabelRight;

    QBoxLayout* m_buttonLayout;
    QBoxLayout* m_tablesLayout;
    QBoxLayout* m_labelLayout;
    QBoxLayout* m_labelLayoutLeft;
    QBoxLayout* m_labelLayoutRight;

    QVector<QTableWidget*>* m_vector;

    QTableWidgetItem* m_item1;
    QTableWidgetItem* m_item2;

    QBrush m_brushGreen;
    QBrush m_brush1;
    QBrush m_brush2;

    void setTable(QString name, QString q);

private slots:
    void doubleClickedTableLeftItem(QTableWidgetItem *item);
    void doubleClickedTableRightItem(QTableWidgetItem *item);
    void open();
    void close();
    void compareDBs();
    void setIsCompared();
    void reset();
};

#endif // WIDGET_H
