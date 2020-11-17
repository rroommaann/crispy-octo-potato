#include "widget.h"
#include "ui_widget.h"

#include "globalFunctionsQT.h"

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMinimumSize(800,600);
    initialize();
}

void Widget::initialize(){ 

    m_item1 = nullptr;
    m_item2 = nullptr;
    m_brushGreen.setStyle(Qt::SolidPattern);
    QColor color = "#86FA69";
    m_brushGreen.setColor(color);

    form = new FormColumns(QStringList());
    connect(form, &FormColumns::compare, this, &Widget::compareDBs);

    comboBox = form->getComboBox();

    connect(ui->tableWidget->verticalScrollBar(), SIGNAL (valueChanged(int)), ui->tableWidget_2->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->tableWidget_2->verticalScrollBar(), SIGNAL (valueChanged(int)), ui->tableWidget->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->tableWidget->horizontalScrollBar(), SIGNAL (valueChanged(int)), ui->tableWidget_2->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->tableWidget_2->horizontalScrollBar(), SIGNAL (valueChanged(int)), ui->tableWidget->horizontalScrollBar(), SLOT(setValue(int)));
}

void Widget::compareDBs()
{
    if(!(m_tableLeft && m_tableRight))
        return;

    if (!(m_tableLeft->isReady() && m_tableRight->isReady()))
        return;

    if(m_item1 && m_item2)
    {
        if(m_item1->text() != m_item2->text())
        {
            QMessageBox::critical (this, tr("Ошибка"), tr("Выбраны разные станции!"));
            return;
        }
    }
    else
        if(selectedType != Stations)
            return;

    if(m_isCompared && (selectedType != Stations))
        return;

    form->hide();
    m_tableLeft->createNewHashData(form->getListofColumns(), selectedType);
    m_tableRight->createNewHashData(form->getListofColumns(), selectedType);

    compareDbs(m_tableLeft->getHash(), m_tableRight->getHash(), ui->tableWidget, ui->tableWidget_2, selectedType, form->getListofColumns());

    m_item1 = nullptr;
    m_item2 = nullptr;
    if(selectedType != Stations)
        m_isCompared = true;
    m_tableLeft->getTable()->resizeRowsToContents();
    m_tableRight->getTable()->resizeRowsToContents();
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

void Widget::reset()
{
    if(m_tableLeft)
        m_tableLeft->resetTable();

    if(m_tableRight)
        m_tableRight->resetTable();

    m_item1 = nullptr;
    m_item2 = nullptr;
    m_isCompared = false;
    QTimer::singleShot(10, this, [=]
                       {
                           int index = comboBox->currentIndex();
                           comboBox->setCurrentIndex(0);
                           QTimer::singleShot(10, this, [=]
                                              {
                                                  form->on_pushButton_2_clicked();
                                                  compareDBs();
                                                  comboBox->blockSignals(true);
                                                  comboBox->setCurrentIndex(index);
                                                  selectedType = (tableType)index;
                                                  form->updateWidget(getListOfColumns(false));
                                                  comboBox->blockSignals(false);
                                              });
                       });
}

QStringList Widget::getListOfColumns(bool diff)
{
    QString queryStr = "";
    QString name;
    if(selectedType == TS)
    {
        name = "TS";
        queryStr = "SELECT * FROM TS;";
    }
    else if (selectedType == TU)
    {
        name = "TU";
        queryStr = "SELECT * FROM TU;";
    }
    else if (selectedType == RouteSrc)
    {
        name = "RouteSrc";
        queryStr = "SELECT * FROM RouteSrc;";
    }
    else if(selectedType == Stations)
    {
        name = "Stations";
        queryStr = "SELECT * FROM Stations;";
    }

    QSqlQuery query1(getDb(m_tableLeft->getNameDB()));
    query1.exec(queryStr);
    query1.next();
    QStringList listOfStations1;
    int i = 0;
    while (!query1.record().fieldName(i).isEmpty())
    {
        listOfStations1 << query1.record().fieldName(i);
        i++;
    }

    QSqlQuery query2(getDb(m_tableRight->getNameDB()));
    query2.exec(queryStr);
    query2.next();
    QStringList listOfStations2;
    i = 0;
    while (!query2.record().fieldName(i).isEmpty())
    {
        listOfStations2 << query2.record().fieldName(i);
        i++;
    }

    QStringList listFinal;
    QStringList listDiff;
    for (auto s : listOfStations1)
    {
        if(listOfStations2.contains(s))
        {
            if((selectedType == Stations) && (s == "NoSt"))
                listFinal.append(s);
            else if((s != "Cod") && (s != "NoSt"))
                listFinal.append(s);
        }
        else
        {
            if((selectedType == Stations) && (s == "NoSt"))
                listDiff.append(s);
            else if((s != "Cod") && (s != "NoSt"))
                listDiff.append(s);
        }
    }

    if(diff && listDiff.size() > 0)
        QMessageBox::information(this, "Разные поля", QString("Базы данных имеют не общие поля в таблице %1, поля %2").arg(name).arg(listDiff.join(", ")));

    form->raise();

    return listFinal;
}

void Widget::openColumns()
{
    form->raise();
    form->show();
}

void Widget::setTable(QString name, QString q)
{
    if(name == ui->labelLeft->text() || name == ui->labelRight->text())
    {
        QMessageBox::critical (this, tr("Ошибка"), tr("Выбранная база уже открыта!!"));
        return;
    }

    if (q == "openButton1")
    {
        if(m_tableLeft)
            m_tableLeft->resetTable(name);
        else
        {
            m_tableLeft = new MyTable(name);
            m_tableLeft->setTable(ui->tableWidget);
        }
        ui->labelLeft->setText(name);
    }
    else
    {
        if(m_tableRight)
            m_tableRight->resetTable(name);
        else
        {
            m_tableRight = new MyTable(name);
            m_tableRight->setTable(ui->tableWidget_2);
        }
        ui->labelRight->setText(name);
    }

    if(m_tableLeft && m_tableRight)
    {
        if (m_tableLeft->isReady() && m_tableRight->isReady())
        {
            comboBox->clear();
            comboBox->addItems(QStringList() << "Stations" << "TS" << "TU" << "RouteSrc");
            selectedType = Stations;
            form->updateWidget(getListOfColumns(false));

            form->on_pushButton_2_clicked();
            compareDBs();
            form->on_pushButton_3_clicked();

            connect(comboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, [=] (QString string)
                    {
                        int index = comboBox->findText(string);
                        selectedType = (tableType)index;
                        form->updateWidget(getListOfColumns(true));
                    });
        }
    }
}

void Widget::doubleClickedTableLeftItem(QTableWidgetItem *item)
{
    if(m_isCompared)
        return;

    int row = item->row();
    int column = -1;
    for(int i = 0; i < m_tableLeft->getTable()->horizontalHeader()->count(); ++i)
    {
        auto itemH = m_tableLeft->getTable()->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == "NameSt")
            column = i;
    }
    if(column == -1)
        return;

    item = m_tableLeft->getTable()->item(row, column);

    if((!m_item1) || (m_item1->background() != m_brushGreen))
    {
        m_brush1 = item->background();
        item->setBackground(m_brushGreen);

        m_tableLeft->setStation(item->text());

        m_item1 = item;
    }
    else if (m_item1->background() == m_brushGreen)
    {
        m_item1->setBackground(m_brush1);
        m_brush1 = item->background();
        item->setBackground(m_brushGreen);

        m_tableLeft->setStation(item->text());

        m_item1 = item;
    }
    m_item1->setSelected(false);

    int columnSecond = -1;
    for(int i = 0; i < m_tableRight->getTable()->horizontalHeader()->count(); ++i)
    {
        auto itemH = m_tableRight->getTable()->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == "NameSt")
            columnSecond = i;
    }
    if(columnSecond == -1)
        return;

    for(int i = 0; i < m_tableRight->getTable()->verticalHeader()->count(); ++i)
    {
        auto itemV = m_tableRight->getTable()->item(i, columnSecond);

        if(itemV->data(Qt::DisplayRole) == item->data(Qt::DisplayRole))
        {
            item = itemV;

            if((!m_item2) || (m_item2->background() != m_brushGreen))
            {
                m_brush2 = item->background();
                item->setBackground(m_brushGreen);

                m_tableRight->setStation(item->text());

                m_item2 = item;
            }
            else if (m_item2->background() == m_brushGreen)
            {
                m_item2->setBackground(m_brush2);
                m_brush2 = item->background();;
                item->setBackground(m_brushGreen);

                m_tableRight->setStation(item->text());

                m_item2 = item;
            }
            m_item2->setSelected(false);
        }
    }
}

void Widget::doubleClickedTableRightItem(QTableWidgetItem *item)
{
    if(m_isCompared)
        return;

    int row = item->row();
    int column = -1;
    for(int i = 0; i < m_tableRight->getTable()->horizontalHeader()->count(); ++i)
    {
        auto itemH = m_tableRight->getTable()->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == "NameSt")
            column = i;
    }
    if(column == -1)
        return;

    item = m_tableRight->getTable()->item(row, column);

    if((!m_item2) || (m_item2->background() != m_brushGreen))
    {
        m_brush2 = item->background();
        item->setBackground(m_brushGreen);

        m_tableRight->setStation(item->text());

        m_item2 = item;
    }
    else if (m_item2->background() == m_brushGreen)
    {
        m_item2->setBackground(m_brush2);
        m_brush2 = item->background();;
        item->setBackground(m_brushGreen);

        m_tableRight->setStation(item->text());

        m_item2 = item;
    }
    m_item2->setSelected(false);

    int columnSecond = -1;
    for(int i = 0; i < m_tableLeft->getTable()->horizontalHeader()->count(); ++i)
    {
        auto itemH = m_tableLeft->getTable()->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == "NameSt")
            columnSecond = i;
    }
    if(columnSecond == -1)
        return;

    for(int i = 0; i < m_tableLeft->getTable()->verticalHeader()->count(); ++i)
    {
        auto itemV = m_tableLeft->getTable()->item(i, columnSecond);
        if(itemV->data(Qt::DisplayRole) == item->data(Qt::DisplayRole))
        {
            item = itemV;

            if((!m_item1) || (m_item1->background() != m_brushGreen))
            {
                m_brush1 = item->background();
                item->setBackground(m_brushGreen);

                m_tableLeft->setStation(item->text());

                m_item1 = item;
            }
            else if (m_item1->background() == m_brushGreen)
            {
                m_item1->setBackground(m_brush1);
                m_brush1 = item->background();
                item->setBackground(m_brushGreen);

                m_tableLeft->setStation(item->text());

                m_item1 = item;
            }
            m_item1->setSelected(false);
        }
    }
}

Widget::~Widget()
{
    delete m_tableRight;
    delete m_tableLeft;
    QStringList list =  QSqlDatabase::connectionNames();
    for(int i = 0; i < list.size(); i++)
    {
        QSqlDatabase::removeDatabase(list[i]);
    }
}
