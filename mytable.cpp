#include "mytable.h"
#include "connection.h"

MyTable::MyTable(QString tableName, QWidget *parent)
    : QWidget(parent)
{
    m_nameDB = tableName;
    m_temp = 1;
    if (!createConnection(m_nameDB)){

    }
    m_model2 = new QSqlRelationalTableModel(this, QSqlDatabase::database(m_nameDB));
    m_model2->setTable("Stations");
    m_model2->select();
    m_model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setViewReady();
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
    for (int i = 0; i < m_model2->rowCount(); i++){
        m_list.append(m_model2->record(i).field(1).value().toString());
    }
}

MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
}

void MyTable::setViewReady(){
    m_view = new QTableView;
    m_view->horizontalHeader()->setSectionsMovable(true);
    m_view->setModel(m_model2);
    m_view->setItemDelegate(new QSqlRelationalDelegate(m_view));
    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_view->setCornerButtonEnabled(true);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

QTableView* MyTable::setStation(QString q){
    if (q == "Нет"){
        m_model2->clear();
        setViewReady();
        m_model2->setTable("Stations");
        m_model2->select();
    } else {
        m_model2->clear();
        setViewReady();
        m_model2->setTable("TS");
        m_model2->setRelation(0, QSqlRelation("Stations", "NoSt", "NameSt"));
        m_model2->setRelation(1, QSqlRelation("TS_Name", "Cod", "NameTS"));
        m_model2->setFilter("NameSt='"+q+"'");
        m_model2->select();
    }
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
    return m_view;
}

QList<QString> MyTable::getStationsList(){
    return this->m_list;
}

void MyTable::disconnectFromDataBase (){
    m_model2->clear();
    m_model2 = new QSqlRelationalTableModel(this);
    {
        QSqlDatabase db = QSqlDatabase::database(m_nameDB);
        db.open();
        db.close();
    }
    qDebug() << QSqlDatabase::contains(m_nameDB);
    QSqlDatabase::removeDatabase(m_nameDB);
}

bool MyTable::checkDB(){
    return (m_temp == 1);
}

QTableView* MyTable::getView(){
    if (MyTable::checkDB()){
        return this->m_view;
    } else return new QTableView;
}

void MyTable::setView(QTableView* &tableView){
    this->m_view = tableView;
}

QSqlRelationalTableModel* MyTable::getModel(){
    if (MyTable::checkDB()){
        return m_model2;
    } else return new QSqlRelationalTableModel;
}

QSqlRelationalTableModel* MyTable::setModel(QSqlRelationalTableModel* &model){
    QItemSelectionModel *m = m_view->selectionModel();
    delete m;
    this->m_model2 = model;
    m_view->setModel(m_model2);
    return m_model2;
}

MyTable::~MyTable(){
}
