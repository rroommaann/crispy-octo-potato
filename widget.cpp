#include "widget.h"
#include "multihashofrecords.h"
#include "comparedbs.h"


using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800,600);
    initialize();
}

void Widget::initialize(){ 

    m_item1 = nullptr;
    m_item2 = nullptr;
    m_brushGreen.setStyle(Qt::SolidPattern);
    QColor color = "#86FA69";
    m_brushGreen.setColor(color);

    m_resetButton = new QPushButton(tr("Reset"));
    connect(m_resetButton, SIGNAL(clicked()), this, SLOT (reset()));

    m_stationsButton = new QPushButton(tr("Выбрать колонок для сравнения"));
    connect(m_stationsButton, SIGNAL(clicked()), this, SLOT (openColumns()));

    form = new FormStations(QStringList());

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

    connect(m_compareButton, SIGNAL(clicked()), this, SLOT (compareDBs()));
    connect(m_compareButton, SIGNAL(clicked()), this, SLOT (setIsCompared()));

    m_tableLeft = new MyTable();
    m_tableRight = new MyTable();
    m_tableLeftView = static_cast<QTableWidget *>(m_tableLeft->getView());
    m_tableRightView = static_cast<QTableWidget *>(m_tableRight->getView());

    m_nameLabelLeft = new QLabel(this);
    m_nameLabelLeft->setObjectName("nameLabel1");
    m_nameLabelLeft->setWordWrap(true);

    m_nameLabelRight = new QLabel(this);
    m_nameLabelRight->setObjectName("nameLabel2");
    m_nameLabelRight->setWordWrap(true);

    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->addWidget(m_openButtonLeft);
    m_buttonLayout->addWidget(m_closeButtonLeft);
    m_buttonLayout->addSpacing(20);
    m_buttonLayout->addWidget(m_openButtonRight);
    m_buttonLayout->addWidget(m_closeButtonRight);

    m_mainLayout = new QVBoxLayout(this);
    m_tablesLayout = new QHBoxLayout();
    m_labelLayout = new QHBoxLayout();
    m_labelLayoutLeft = new QHBoxLayout();
    m_labelLayoutRight = new QHBoxLayout();

    m_labelLayoutLeft->addWidget(m_nameLabelLeft);
    m_labelLayoutRight->addWidget(m_nameLabelRight);
    m_labelLayout->addLayout(m_labelLayoutLeft);
    m_labelLayout->addSpacing(20);
    m_labelLayout->addLayout(m_labelLayoutRight);

    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addWidget(m_compareButton);
    m_mainLayout->addWidget(m_resetButton);
    m_mainLayout->addWidget(m_stationsButton);
    m_mainLayout->addLayout(m_labelLayout);
    m_mainLayout->addLayout(m_tablesLayout);
    setLayout(m_mainLayout);
}

void Widget::compareDBs(){

    if (!(m_isTableLeftSet && m_isTableRightSet) && !(m_item1 && m_item2)){
        return;
    }

    if((m_item1 && m_item2) && (m_item1->text() != m_item2->text()))
    {
        QMessageBox::critical (this, tr("Ошибка"), tr("Выбраны разные станции!"));
        return;
    }

    QSqlRelationalTableModel* model1 = m_tableLeft->getModel();
    QSqlRelationalTableModel* model2 = m_tableRight->getModel();

    QVector<QTableWidget*>* vector = new QVector<QTableWidget*>(2);

    vector = compareDbs(model1, model2, form->getListofColumns());

    m_tablesLayout->removeWidget(m_tableLeftView);
    m_tablesLayout->removeWidget(m_tableRightView);
    m_tableLeftView->setParent(nullptr);
    m_tableRightView->setParent(nullptr);
    m_tableLeftView->deleteLater();
    m_tableRightView->deleteLater();

    m_tableLeftView = vector->value(0);
    m_tableRightView = vector->value(1);

    QTableView* temp = static_cast<QTableView *>(m_tableLeftView);
    m_tableLeft->setView(temp);

    temp = static_cast<QTableView *>(m_tableRightView);
    m_tableRight->setView(temp);

    m_tableLeftView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableLeftView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_tableRightView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableRightView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_tablesLayout->insertWidget(0, m_tableLeftView);
    m_tablesLayout->insertWidget(1, m_tableRightView);

    connect(m_tableLeftView->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightView->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRightView->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftView->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_tableLeftView->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightView->horizontalScrollBar(), SLOT(setValue(int)));
    connect(m_tableRightView->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftView->horizontalScrollBar(), SLOT(setValue(int)));
    connect(m_tableLeftView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(doubleClickedTableLeftItem(QTableWidgetItem *)));
    connect(m_tableRightView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(doubleClickedTableRightItem(QTableWidgetItem *)));
    delete vector;
}

void Widget::setIsCompared(){
    m_isCompared = true;
}

void Widget::open()
{
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    QFileDialog fileDialog(this);
    QStringList nameFilters;
    nameFilters.append("*.mdb");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open File"));
    fileDialog.setNameFilters(nameFilters);
    fileDialog.setDirectory("C:\\");
    if (fileDialog.exec() == QDialog::Accepted){
        QString name = fileDialog.selectedUrls().at(0).path();
        name.remove(0,1);
        name.replace("/", "\\");
        setTable(name, q);
    }
}

void Widget::close()
{
    if (m_isCompared) reset();
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    if (q == "closeButton1" && m_nameLabelLeft->text() != ""){
        m_tablesLayout->takeAt(0);
        delete m_tableLeftView;
        delete m_tableLeft->getModel();
        QSqlDatabase::database(m_nameLabelLeft->text()).close();
        QSqlDatabase::removeDatabase(m_nameLabelLeft->text());
        m_tableLeftView = new QTableWidget();
        m_tablesLayout->insertWidget(0, m_tableLeftView);
        m_nameLabelLeft->setText("");
        m_isTableLeftSet = false;
        m_item1 = nullptr;
    }
    else if (q == "closeButton2" && m_nameLabelRight->text() != "")
    {
        m_tablesLayout->takeAt(1);
        delete m_tableRightView;
        delete m_tableRight->getModel();
        QSqlDatabase::database(m_nameLabelRight->text()).close();
        QSqlDatabase::removeDatabase(m_nameLabelRight->text());
        m_tableRightView = new QTableWidget();
        m_tablesLayout->insertWidget(1, m_tableRightView);
        m_nameLabelRight->setText("");
        m_isTableRightSet = false;
        m_item2 = nullptr;
    }
    if ((m_isTableLeftSet == false) && (m_isTableRightSet == false)){
        delete m_tableLeftView;
        delete m_tableRightView;
    }
}

void Widget::reset(){

    if (!(m_isTableLeftSet && m_isTableRightSet))
    {
        return;
    }
    else if(m_isTableLeftSet && m_isTableRightSet)
    {
        m_tableLeftView->deleteLater();
        m_tableRightView->deleteLater();
        m_item1 = nullptr;
        m_item2 = nullptr;
        delete m_tableRight->getModel();
        delete m_tableLeft->getModel();
        QStringList list =  QSqlDatabase::connectionNames();
        for(int i = 0; i < list.size(); i++)
        {
            QSqlDatabase::removeDatabase(list[i]);
        }
        m_isTableLeftSet = false;
        m_isTableRightSet = false;
        QString name1 = m_nameLabelLeft->text();
        QString name2 = m_nameLabelRight->text();
        m_nameLabelLeft->setText("");
        m_nameLabelRight->setText("");
        setTable(name1, "openButton1");
        setTable(name2, "openButton2");
        m_isCompared = false;
        return;
    }
}

void Widget::openColumns()
{
    QSqlRelationalTableModel* model1 = m_tableLeft->getModel();
    QSqlQuery query1(model1->database());
    query1.exec("SELECT * FROM TS");
    query1.next();
    QStringList listOfStations1;
    int i = 0;
    while (!query1.record().fieldName(i).isEmpty())
    {
        listOfStations1 << query1.record().fieldName(i);
        i++;
    }

    QSqlRelationalTableModel* model2 = m_tableRight->getModel();
    QSqlQuery query2(model2->database());
    query2.exec("SELECT * FROM TS");
    query2.next();
    QStringList listOfStations2;
    i = 0;
    while (!query2.record().fieldName(i).isEmpty())
    {
        listOfStations2 << query2.record().fieldName(i);
        i++;
    }

    QStringList listFinal;
    for (auto s : listOfStations1)
    {
        if(listOfStations2.contains(s))
        {
            listFinal.append(s);
        }
    }

    form->updateWidget(listFinal);
    form->show();
}

void Widget::setTable(QString name, QString q){
    if(name == m_nameLabelRight->text() || name == m_nameLabelLeft->text()){
        QMessageBox::critical (this, tr("Ошибка"), tr("Выбранная база уже открыта!!"));
        return;
    }
    if (q == "openButton1"){
        if (m_isTableLeftSet) emit m_closeButtonLeft->clicked(true);
        m_tableLeft = new MyTable(name);
        QLayoutItem *item = m_tablesLayout->itemAt(1);
        if (!item){
            m_tableRightView = new QTableWidget();
            m_tablesLayout->insertWidget(1, m_tableRightView);
            m_nameLabelRight->setText("");
            m_isTableRightSet = false;
            m_item2 = nullptr;
        } else {
            m_tablesLayout->removeWidget(m_tableLeftView);
            m_tableLeftView->deleteLater();
        }
        m_tableLeftView = static_cast<QTableWidget *>(m_tableLeft->getView());
        m_tablesLayout->insertWidget(0, m_tableLeftView);
        m_nameLabelLeft->setText(name);
        m_isTableLeftSet = true;
    }
    else
    {
       if (m_isTableRightSet) emit m_closeButtonRight->clicked(true);
        m_tableRight = new MyTable(name);
        QLayoutItem *item = m_tablesLayout->itemAt(0);
        if (!item){
            m_tableLeftView = new QTableWidget();
            m_tablesLayout->insertWidget(0, m_tableLeftView);
            m_nameLabelLeft->setText("");
            m_isTableLeftSet = false;
            m_item1 = nullptr;
        } else {
            m_tablesLayout->removeWidget(m_tableRightView);
            m_tableRightView->deleteLater();
        }
        m_tableRightView = static_cast<QTableWidget *>(m_tableRight->getView());
        m_tablesLayout->insertWidget(1, m_tableRightView);
        m_nameLabelRight->setText(name);
        m_isTableRightSet = true;
    }

    if (m_isTableLeftSet && m_isTableRightSet){
        connect(m_tableLeftView->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightView->verticalScrollBar(), SLOT(setValue(int)));
        connect(m_tableRightView->verticalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftView->verticalScrollBar(), SLOT(setValue(int)));
        connect(m_tableLeftView->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableRightView->horizontalScrollBar(), SLOT(setValue(int)));
        connect(m_tableRightView->horizontalScrollBar(), SIGNAL (valueChanged(int)), m_tableLeftView->horizontalScrollBar(), SLOT(setValue(int)));
        openColumns();
        compareDBs();
    }
}

void Widget::doubleClickedTableLeftItem(QTableWidgetItem *item){
    if((!m_item1) || (m_item1->background() != m_brushGreen)) {
        m_brush1 = item->background();
        item->setBackground(m_brushGreen);
        m_tableLeft->setStation(item->text());
        m_item1 = item;
    } else if (m_item1->background() == m_brushGreen){
        m_item1->setBackground(m_brush1);
        m_brush1 = item->background();
        item->setBackground(m_brushGreen);
        m_tableLeft->setStation(item->text());
        m_item1 = item;
    }
    m_item1->setSelected(false);
}
void Widget::doubleClickedTableRightItem(QTableWidgetItem *item){
    if((!m_item2) || (m_item2->background() != m_brushGreen)) {
        m_brush2 = item->background();
        item->setBackground(m_brushGreen);
        m_tableRight->setStation(item->text());
        m_item2 = item;
    } else if (m_item2->background() == m_brushGreen){
        m_item2->setBackground(m_brush2);
        m_brush2 = item->background();;
        item->setBackground(m_brushGreen);
        m_tableRight->setStation(item->text());
        m_item2 = item;
    }
    m_item2->setSelected(false);
}

Widget::~Widget()
{
    delete m_tableRight;
    delete m_tableLeft;
    QStringList list =  QSqlDatabase::connectionNames();
    for(int i = 0; i < list.size(); i++){
        QSqlDatabase::removeDatabase(list[i]);
    }
}
