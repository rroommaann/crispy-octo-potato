#include "mytable.h"

MyTable::MyTable(QString tableName, QWidget *parent)
    : QWidget(parent)
{
    m_nameDB = tableName;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", m_nameDB);
        db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + tableName);
        db.setUserName("Admin");
        db.setPassword("64465");
        db.open();
    }
    m_model2 = new QSqlRelationalTableModel(this, QSqlDatabase::database(m_nameDB));
    m_model2->setTable("Stations");
    m_model2->select();
    m_model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setViewReady();
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
}

MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
    m_model2 = new QSqlRelationalTableModel(this);
    m_view = new QTableView(this);
}

void MyTable::setViewReady(){
    m_view = new QTableView();
    m_view->horizontalHeader()->setSectionsMovable(true);
    m_view->setModel(m_model2);
    m_view->setItemDelegate(new QSqlRelationalDelegate(m_view));
    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MyTable::setStation(QString q){
    m_model2->clear();
    setViewReady();
    m_model2->setTable("TS");
    m_model2->setRelation(0, QSqlRelation("Stations", "NoSt", "NameSt"));
    m_model2->setRelation(1, QSqlRelation("TS_Name", "Cod", "NameTS"));
    m_model2->setFilter("NameSt='"+q+"'");
    m_model2->select();
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
}

QTableView* MyTable::getView(){
    return m_view;
}

void MyTable::setView(QTableView* &tableView){
    m_view = tableView;
}

QSqlRelationalTableModel* MyTable::getModel(){
    return m_model2;
}

MyTable::~MyTable(){
}
