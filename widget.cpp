#include "widget.h"
#include "multihashofrecords.h"
#include "comparedbs.h"
#include <sizeoofmassiveofnotequalrows.h>
#include <bits/stdc++.h>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
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
    qDebug() << model1 << m_tableLeft->getModel();
    qDebug() << model2 << m_tableRight->getModel();
    QVector<int>* vector = compareDbs(model1, model2);

    m_tableLeft->setModel(model1);
    m_tableRight->setModel(model2);
    qDebug() << model1 << m_tableLeft->getModel();
    qDebug() << model2 << m_tableRight->getModel();

    int sizeoOfMassiveOfNotEqualRows = vector->size();
    qDebug() << sizeoOfMassiveOfNotEqualRows;

    for (int i = 0; i < sizeoOfMassiveOfNotEqualRows; i++){
        m_tableLeft->highlightCell(vector->at(i));
        m_tableRight->highlightCell(vector->at(i));
    }

//    for (int i = 0; i < sizeoOfMassiveOfNotEqualRows; i++){
//        qDebug() << i << " - i";
//        if ((massive1[i][3] == 0) && (massive2[i][3] == 1)){
//            qDebug() << "Строка отсутсвует в левой таблице";
//            m_tableRight->highlightCell(massive2[i][0], massive2[i][1], massive2[i][2]);
//            qDebug() << "Выделение строки" << massive2[i][0] << " " << massive2[i][1]<< " " << massive2[i][2];
//            qDebug() << "Вставка строки под номером " << (findCell(m_tableRight->getModel(), massive2[i][0], massive2[i][1], massive2[i][2]));
//            qDebug() << m_tableLeft->getModel()->rowCount();
//            bool vstav = m_tableLeft->getModel()->insertRow(findCell(m_tableRight->getModel(), massive2[i][0], massive2[i][1], massive2[i][2]));
//            qDebug() << m_tableLeft->getModel()->rowCount();
//            qDebug() << "Строка вставлена в левую таблицу - " << vstav;
//        } else if ((massive2[i][3] == 0) && (massive1[i][3] == 1)){
//            qDebug() << "Строка отсутсвует в правой таблице";
//            m_tableLeft->highlightCell(massive1[i][0], massive1[i][1], massive1[i][2]);
//            qDebug() << "Выделение строки" << massive1[i][0] << " " << massive1[i][1]<< " " << massive1[i][2];;
//            qDebug() << "Вставка строки под номером " << (findCell(m_tableLeft->getModel(), massive1[i][0], massive1[i][1], massive1[i][2]));
//            qDebug() << m_tableRight->getModel()->rowCount();
//            bool vstav = m_tableRight->getModel()->insertRow(findCell(m_tableLeft->getModel(), massive1[i][0], massive1[i][1], massive1[i][2]));
//            qDebug() << m_tableRight->getModel()->rowCount();
//            qDebug() << "Строка вставлена в правую таблицу - " << vstav;
//        } else if ((massive1[i][3] == 1) && (massive2[i][3] == 1)){
//            qDebug() << "Строки есть" << massive1[i][0] << " " << massive1[i][1] << " " << massive1[i][2];
//            qDebug() << "Строки есть" << massive2[i][0] << " " << massive2[i][1] << " " << massive2[i][2];
//            m_tableLeft->highlightCell(massive1[i][0], massive1[i][1], massive1[i][2]);
//            m_tableRight->highlightCell(massive2[i][0], massive2[i][1], massive2[i][2]);
//            qDebug() << "Выделение строк";
//        }
//    }
}

void Widget::sortMassive(int** massive,int** massive1,int sizeoOfMassiveOfNotEqualRows, QSqlRelationalTableModel* model1,QSqlRelationalTableModel* model2){
    int* temp = new int [4];
    qDebug() << sizeoOfMassiveOfNotEqualRows;
    for (int i = 1; i < sizeoOfMassiveOfNotEqualRows - 1; i++){
        for (int j = 0; j < sizeoOfMassiveOfNotEqualRows - i - 1; j++){
            int rowIndex1 = findCell(model1, massive[j][0], massive[j][1], massive[j][2]);
            int rowIndex2 = findCell(model2, massive[j+1][0], massive[j+1][1], massive[j+1][2]);
            if (rowIndex1 == -1) {
                rowIndex1 = findCell(model2, massive[j][0], massive[j][1], massive[j][2]);
            }
            if (rowIndex2 == -1) {
                rowIndex2 = findCell(model1, massive[j+1][0], massive[j+1][1], massive[j+1][2]);
            }
            if(rowIndex1 > rowIndex2){
                qDebug() << "check" << j;

                std::swap(massive[j],massive[j+1]);
            }
        }

    }
//    for (int i = 0; i < sizeoOfMassiveOfNotEqualRows; i++) {
//        temp[i] = new int [4];
//        temp[i][0] = INT_MAX;
//        temp[i][1] = INT_MAX;
//        temp[i][2] = INT_MAX;
//        temp[i][3] = 1;
//    }
//    int n = 0;
//    for (int i = 0; i < sizeoOfMassiveOfNotEqualRows; i++){
//        if (massive[i][0] < temp[i][0]){
//            temp[n] = massive[i];
//            n++;
//        } else  if ((massive[i][1] < temp[i][1]) && (massive[i][0] == temp[i][0])){
//            temp[n] = massive[i];
//            n++;
//        } else  if ((massive[i][2] < temp[i][2]) && (massive[i][0] == temp[i][0]) && (massive[i][1] == temp[i][1])){
//            temp[n] = massive[i];
//            n++;
//        }
//    }
}

void Widget::chooseStation(QString string){
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    if (q == "comboBox1"){
        m_tableLeft->setMyQuery(string);
    } else if (q == "comboBox2"){
        m_tableRight->setMyQuery(string);
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
        QString newName = name;
        newName.replace(".mdb", "Copy.mdb");
        QFile file;
        file.setFileName(name);
        qDebug() << file.copy(newName);
        qDebug() << newName;
        setTable(name, q);
    }
}

void Widget::close()
{
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    if (q == "closeButton1" && m_nameLabelLeft->text() != ""){
        m_tableLeft->disconnectFromDataBase ();
        m_nameLabelLeft->setText("");
        m_comboBoxLeft->clear();
    }
    else if (q == "closeButton2" && m_nameLabelRight->text() != "")
    {
        m_tableRight->disconnectFromDataBase ();
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
        m_tablesLayout->removeWidget(m_tableLeft->getView());
        delete m_tableLeft->getView();
        delete m_tableLeft;
        qDebug() << "rrr1";
        m_tableLeft = new MyTable(name);
        qDebug() << "rrr1";
        m_stationsList1 = new QStringList(m_tableLeft->getStationsList());
        m_comboBoxLeft->addItems(*m_stationsList1);
        m_labelLayoutLeft->insertWidget(1, m_comboBoxLeft);
        m_tablesLayout->insertWidget(0, m_tableLeft->getView());
        m_nameLabelLeft->setText(name);
        connect(m_comboBoxLeft, SIGNAL(currentIndexChanged(QString)), this, SLOT(chooseStation(QString)));
    }
    else
    {
        if(name == m_nameLabelLeft->text()){
            QMessageBox::critical (this, tr("Ошибка"), tr("Выбранная база уже открыта!!"));
            return;
        }
        m_tablesLayout->removeWidget(m_tableRight->getView());
        delete m_tableRight->getView();
        delete m_tableRight;
        m_tableRight = new MyTable(name);
        m_stationsList2 = new QStringList(m_tableRight->getStationsList());
        m_comboBoxRight->addItems(*m_stationsList2);
        m_labelLayoutRight->insertWidget(1, m_comboBoxRight);
        m_tablesLayout->insertWidget(1, m_tableRight->getView());
        m_nameLabelRight->setText(name);
        connect(m_comboBoxRight, SIGNAL(currentIndexChanged(QString)), this, SLOT(chooseStation(QString)));
    }
    connect(m_tableLeft->getView()->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableRight->getView()->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRight->getView()->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeft->getView()->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableLeft->getView()->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableRight->getView()->horizontalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRight->getView()->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeft->getView()->horizontalScrollBar(), SLOT(setValue(int)));

    showMinimized();
    showMaximized();
}

Widget::~Widget()
{
}
