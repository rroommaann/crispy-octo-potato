#include "widget.h"
#include "multihashofrecords.h"
#include "comparedbs.h"
#include <bits/stdc++.h>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy size(QSizePolicy::Expanding,QSizePolicy::Expanding,QSizePolicy::DefaultType);
    setSizePolicy(size);
    initialize();
}

void Widget::initialize(){
    m_openButtonLeft = new QPushButton(tr("Open"), this);
    m_openButtonLeft->setObjectName("openButton1");
    m_openButtonRight = new QPushButton(tr("Open"), this);
    m_openButtonRight->setObjectName("openButton2");

    m_closeButtonLeft = new QPushButton(tr("Close DB"), this);
    m_closeButtonLeft->setObjectName("closeButton1");
    m_closeButtonRight = new QPushButton(tr("Close DB"), this);
    m_closeButtonRight->setObjectName("closeButton2");

    m_compareButton = new QPushButton(tr("Compare"), this);

    connect(m_openButtonLeft, SIGNAL(clicked()), this, SLOT (open()));
    connect(m_openButtonRight, SIGNAL(clicked()), this, SLOT (open()));

    connect(m_closeButtonLeft, SIGNAL(clicked()), this, SLOT (close()));
    connect(m_closeButtonRight, SIGNAL(clicked()), this, SLOT (close()));

    connect(m_compareButton, SIGNAL(clicked()), this, SLOT (CompareDBs()));

    m_tableLeft = new MyTable();
    m_tableRight = new MyTable();
    m_tableLeftVeiew = m_tableLeft->getView();
    m_tableRightVeiew = m_tableRight->getView();

    m_comboBoxLeft = new QComboBox();
    m_comboBoxLeft->setObjectName("comboBox1");
    m_comboBoxRight = new QComboBox();
    m_comboBoxRight->setObjectName("comboBox2");

    m_nameLabelLeft = new QLabel(this);
    m_nameLabelLeft->setObjectName("nameLabel1");
    m_nameLabelLeft->setWordWrap(true);

    m_nameLabelRight = new QLabel(this);
    m_nameLabelRight->setObjectName("nameLabel2");
    m_nameLabelRight->setWordWrap(true);

    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_openButtonLeft);
    m_buttonLayout->addWidget(m_closeButtonLeft);
    m_buttonLayout->addSpacing(20);
    m_buttonLayout->addWidget(m_openButtonRight);
    m_buttonLayout->addWidget(m_closeButtonRight);

    m_mainLayout = new QVBoxLayout;
    m_tablesLayout = new QHBoxLayout;
    m_tablesLayout->addStrut(500);
    m_labelLayout = new QHBoxLayout;
    m_labelLayoutLeft = new QHBoxLayout;
    m_labelLayoutRight = new QHBoxLayout;

    m_labelLayoutLeft->addWidget(m_nameLabelLeft);
    m_labelLayoutRight->addWidget(m_nameLabelRight);
    m_labelLayout->addLayout(m_labelLayoutLeft);
    m_labelLayout->addSpacing(20);
    m_labelLayout->addLayout(m_labelLayoutRight);

    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addWidget(m_compareButton);
    m_mainLayout->addLayout(m_labelLayout);
    m_mainLayout->addLayout(m_tablesLayout);
    setLayout(m_mainLayout);
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

void Widget::CompareDBs(){

    if (m_comboBoxLeft->currentText() != m_comboBoxRight->currentText()){
        QMessageBox::critical (this, tr("Ошибка"), tr("Выбраны разные станции!"));
        return;
    }
    m_tableLeft->getView()->setSelectionMode(QAbstractItemView::MultiSelection);
    m_tableRight->getView()->setSelectionMode(QAbstractItemView::MultiSelection);

    QSqlRelationalTableModel* model1 = m_tableLeft->getModel();
    QSqlRelationalTableModel* model2 = m_tableRight->getModel();

    QVector<QTableWidget*>* vector = new QVector<QTableWidget*>(2);
    vector = compareDbs(model1, model2);

    m_tablesLayout->removeWidget(m_tableLeftVeiew);
    m_tablesLayout->removeWidget(m_tableRightVeiew);
    delete m_tableLeftVeiew;
    delete m_tableRightVeiew;
    m_tableLeft->setView(m_tableLeftVeiew);
    m_tableRight->setView(m_tableRightVeiew);
    m_tableLeftVeiew = vector->value(0);
    m_tableRightVeiew = vector->value(1);
    m_tablesLayout->insertWidget(0, m_tableLeftVeiew);
    m_tablesLayout->insertWidget(1, m_tableRightVeiew);
    connect(m_tableLeftVeiew->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightVeiew->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRightVeiew->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftVeiew->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableLeftVeiew->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightVeiew->horizontalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRightVeiew->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftVeiew->horizontalScrollBar(), SLOT(setValue(int)));

    showMaximized();
}

void Widget::chooseStation(QString string){
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    if (q == "comboBox1"){
        m_tablesLayout->removeWidget(m_tableLeftVeiew);
        delete m_tableLeftVeiew;
        m_tableLeftVeiew = new QTableView;
        m_tableLeftVeiew = m_tableLeft->setStation(string);
        m_tablesLayout->insertWidget(0, m_tableLeftVeiew);
    } else if (q == "comboBox2"){
        m_tablesLayout->removeWidget(m_tableRightVeiew);
        delete m_tableRightVeiew;
        m_tableRightVeiew = new QTableView;
        m_tableRightVeiew = m_tableRight->setStation(string);
        m_tablesLayout->insertWidget(1, m_tableRightVeiew);
    }
}

void Widget::open()
{
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    QFileDialog fileDialog(this);
    QStringList nameFilters;
    nameFilters.append("Arm.mdb");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open File"));
    fileDialog.setNameFilters(nameFilters);
    fileDialog.setDirectory("C:\\");
    if (fileDialog.exec() == QDialog::Accepted){
        QString name = fileDialog.selectedUrls().at(0).path();
        setTable(name, q);
    }
}

void Widget::close()
{
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    if (q == "closeButton1" && m_nameLabelLeft->text() != ""){
        delete m_tableLeftVeiew;
        m_tableLeftVeiew = new QTableView;
        m_tablesLayout->insertWidget(0, m_tableLeftVeiew);
        m_nameLabelLeft->setText("");
        m_comboBoxLeft->clear();
    }
    else if (q == "closeButton2" && m_nameLabelRight->text() != "")
    {
        delete m_tableRightVeiew;
        m_tableRightVeiew = new QTableView;
        m_tablesLayout->insertWidget(1, m_tableRightVeiew);
        m_nameLabelRight->setText("");
        m_comboBoxRight->clear();
    }
}

void Widget::setTable(QString name, QString q){
    name.remove(0,1);
    name.replace("/", "\\");
    if (q == "openButton1"){
        if(name == m_nameLabelRight->text()){
            QMessageBox::critical (this, tr("Ошибка"), tr("Выбранная база уже открыта!!"));
            return;
        }
        m_tablesLayout->removeWidget(m_tableLeftVeiew);
        delete m_tableLeft;
        m_tableLeft = new MyTable(name);
        m_stationsList1 = new QStringList(m_tableLeft->getStationsList());
        m_comboBoxLeft->addItems(*m_stationsList1);
        m_labelLayoutLeft->insertWidget(1, m_comboBoxLeft);
        m_tableLeftVeiew = m_tableLeft->getView();
        m_tablesLayout->insertWidget(0, m_tableLeftVeiew);
        m_nameLabelLeft->setText(name);
        connect(m_comboBoxLeft, SIGNAL(currentIndexChanged(QString)), this, SLOT(chooseStation(QString)));
    }
    else
    {
        if(name == m_nameLabelLeft->text()){
            QMessageBox::critical (this, tr("Ошибка"), tr("Выбранная база уже открыта!!"));
            return;
        }
        m_tablesLayout->removeWidget(m_tableRightVeiew);
        delete m_tableRight;
        m_tableRight = new MyTable(name);
        m_stationsList2 = new QStringList(m_tableRight->getStationsList());
        m_comboBoxRight->addItems(*m_stationsList2);
        m_labelLayoutRight->insertWidget(1, m_comboBoxRight);
        m_tableRightVeiew = m_tableRight->getView();
        m_tablesLayout->insertWidget(1, m_tableRightVeiew);
        m_nameLabelRight->setText(name);
        connect(m_comboBoxRight, SIGNAL(currentIndexChanged(QString)), this, SLOT(chooseStation(QString)));
    }
    connect(m_tableLeft->getView()->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableRight->getView()->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRight->getView()->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeft->getView()->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableLeft->getView()->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableRight->getView()->horizontalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRight->getView()->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeft->getView()->horizontalScrollBar(), SLOT(setValue(int)));

}

Widget::~Widget()
{
}
