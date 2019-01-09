#include "mytableview.h"
#include "connection.h"

MyTable::MyTable(QString tableName, QWidget *parent)
    : QWidget(parent)
{
    m_nameDB = tableName;
    m_temp = 1;
    if (!createConnection(m_nameDB)){
    }
    MyTable::ready();
}

MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
}

void MyTable::ready (){
    m_model = new QSqlQueryModel(this);
    m_model2 = new QSqlRelationalTableModel(this, QSqlDatabase::database(m_nameDB));
    m_model2->setTable("Stations");
    m_model2->select();
    m_model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_view = new QTableView;
    m_view->horizontalHeader()->setSectionsMovable(true);
    m_view->setModel(m_model2);
    m_view->setItemDelegate(new QSqlRelationalDelegate(m_view));
    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_view->setCornerButtonEnabled(true);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_model->setQuery("SELECT Stations.NoSt, Stations.NameSt, Stations.ShortName FROM Stations ORDER BY Stations.NoSt ASC;", QSqlDatabase::database(m_nameDB));
//    while (m_model->canFetchMore()){
//        m_model->fetchMore();
//    }
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
    for (int i = 0; i < m_model2->rowCount(); i++){
        m_list.append(m_model2->record(i).field(1).value().toString());
    }
}

void MyTable::highlightCell(int k)
{
    m_view->selectRow(k);
}

void MyTable::setMyQuery(QString q){
    if (q == "Нет"){
        //m_model->setQuery("SELECT Stations.NoSt, Stations.NameSt, Stations.ShortName FROM Stations ORDER BY Stations.NoSt ASC;", QSqlDatabase::database(m_nameDB));
        m_model2->clear();
        m_model2->setTable("Stations");
        m_model2->select();
    } else {
//        QString query;
//        query = "SELECT Stations.NameSt, TS_Name.NameTs, TS.Question, "
//                "TS.Module, TS.I, TS.J, "
//                "TS.Lock, TS.Pulse, TS.Inverse, "
//                "TS.Occupation, TS.SvtfMain, TS.SvtfDiag, "
//                "TS.SvtfClass, TS.SvtfError, "
//                "TS.NoRc, TS.NoStrl, "
//                "TS.NoSvtf, TS.ExtData, "
//                "TS.StrlZsName, TS.StrlMuName "
//                "FROM TS, TS_Name, Stations "
//                "WHERE Stations.NameSt='"+q+"' AND TS.Cod=TS_Name.Cod AND TS.NoSt=Stations.NoSt ORDER BY TS.Module,TS.I, TS.J, TS_Name.NameTs ASC;";
        //query = "SELECT TU.Stativ, TU.Place, TU.Module,TU.I, TU.J, TU.Kolodka, TU.Klem, TU_Name.NameTu, Stations.NameSt, Stations.NoSt FROM TU, TU_Name, Stations WHERE TU.Cod=TU_Name.Cod AND TU.NoSt=Stations.NoSt ORDER BY TU.Stativ, TU.Place, TU.Module,TU.I, TU.J, TU.Kolodka, TU.Klem ASC;";
//        m_model->setQuery(query, QSqlDatabase::database(m_nameDB));
        m_model2->clear();
        m_model2->setTable("TS");
        m_model2->setRelation(0, QSqlRelation("Stations", "NoSt", "NameSt"));
        m_model2->setRelation(1, QSqlRelation("TS_Name", "Cod", "NameTS"));
        m_model2->setFilter("NameSt='"+q+"'");
       // m_model2->setSort(1, Qt::AscendingOrder);
        m_model2->select();
    }
    while (m_model2->canFetchMore()){
        m_model2->fetchMore();
    }
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
