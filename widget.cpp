#include "widget.h"
#include "ui_widget.h"

#include "globalFunctionsQT.h"

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMinimumSize(800, 600);
    initialize();
}

void Widget::initialize(){ 

    m_item1 = nullptr;
    m_item2 = nullptr;
    m_brushGreen.setStyle(Qt::SolidPattern);
    QColor color(QStringLiteral(u"#86FA69"));
    m_brushGreen.setColor(color);

    form = new FormColumns(QStringList());
    connect(form, &FormColumns::compare, this, &Widget::compareDBs);

    comboBox = form->getComboBox();

    connect(ui->tableWidgetLeft->verticalScrollBar(), &QAbstractSlider::valueChanged, ui->tableWidgetRight->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(ui->tableWidgetRight->verticalScrollBar(), &QAbstractSlider::valueChanged, ui->tableWidgetLeft->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(ui->tableWidgetLeft->horizontalScrollBar(), &QAbstractSlider::valueChanged, ui->tableWidgetRight->horizontalScrollBar(), &QAbstractSlider::setValue);
    connect(ui->tableWidgetRight->horizontalScrollBar(), &QAbstractSlider::valueChanged, ui->tableWidgetLeft->horizontalScrollBar(), &QAbstractSlider::setValue);
}

void Widget::compareDBs()
{
    if(!(m_tableLeft && m_tableRight) || !(m_tableLeft->isReady() && m_tableRight->isReady()))
        return;

    bool allStations = false;
    if(m_item1 && m_item2)
    {
        if(m_item1->text() != m_item2->text())
        {
            QMessageBox::critical(this, QStringLiteral(u"Ошибка"), QStringLiteral(u"Выбраны разные станции!"));
            return;
        }
    }
    else
    {
        if(selectedType != Stations)
        {
            auto clickedBtn = QMessageBox::question(this, QStringLiteral(u"Ошибка"),
                                                    QStringLiteral(u"Вы не выбрали станцию для сравнения, продолжить сравнение по всем станциям?"));

            if(clickedBtn == QMessageBox::No)
                return;

            allStations = true;
        }
    }

    if(m_isCompared && (selectedType != Stations))
        return;

    form->hide();

    ui->tableWidgetLeft->setUpdatesEnabled(false);
    ui->tableWidgetRight->setUpdatesEnabled(false);

    auto list = form->getListofColumns();

    // true сравнение по всем станциям, будут отображаться только различия
    if(allStations)
    {
        auto listOfStationsLeft = m_tableLeft->getListOfStations();
        auto listOfStationsRight = m_tableRight->getListOfStations();
        ui->tableWidgetLeft->clear();
        ui->tableWidgetLeft->setRowCount(0);
        ui->tableWidgetRight->clear();
        ui->tableWidgetRight->setRowCount(0);
        for(const auto &stationName : listOfStationsLeft)
        {
            if(listOfStationsRight.contains(stationName))
            {
                m_tableLeft->setStation(stationName);
                m_tableRight->setStation(stationName);

                m_tableLeft->createNewHashData(list, selectedType);
                m_tableRight->createNewHashData(list, selectedType);

                compareDbs(m_tableLeft->getHash(), m_tableRight->getHash(), ui->tableWidgetLeft, ui->tableWidgetRight, selectedType, list, allStations);
            }
        }
    }
    else
    {
        m_tableLeft->createNewHashData(list, selectedType);
        m_tableRight->createNewHashData(list, selectedType);
        currentIndex = -1;
        vectorOfDiffRows = compareDbs(m_tableLeft->getHash(), m_tableRight->getHash(), ui->tableWidgetLeft, ui->tableWidgetRight, selectedType, list, allStations);
    }

    m_item1 = nullptr;
    m_item2 = nullptr;

    if(selectedType != Stations)
        m_isCompared = true;

    ui->tableWidgetLeft->setUpdatesEnabled(true);
    ui->tableWidgetRight->setUpdatesEnabled(true);

    ui->tableWidgetLeft->resizeRowsToContents();
    ui->tableWidgetRight->resizeRowsToContents();

    ui->tableWidgetLeft->resizeColumnsToContents();
    ui->tableWidgetRight->resizeColumnsToContents();
}

void Widget::open()
{
    QObject *obj = QObject::sender();
    QString q = obj->objectName();
    QFileDialog fileDialog(this);
    QStringList nameFilters;
    nameFilters.append("*.mdb");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(QStringLiteral(u"Выберите базу данных Access для открытия:"));
    fileDialog.setNameFilters(nameFilters);
    fileDialog.setDirectory("C:\\");
    if(fileDialog.exec() == QDialog::Accepted)
    {
        QString name = fileDialog.selectedUrls().at(0).path();
        name.remove(0, 1);
        name.replace("/", "\\");
        setTable(name, q);
    }
}

void Widget::reset()
{
    if(m_tableLeft)
        m_tableLeft->resetTable();
    else
        return;

    if(m_tableRight)
        m_tableRight->resetTable();
    else
        return;

    m_item1 = nullptr;
    m_item2 = nullptr;
    m_isCompared = false;
    vectorOfDiffRows.clear();
    currentIndex = -1;
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

auto Widget::getListOfColumns(bool diff) -> QStringList
{
    QString queryStr = "";
    QString name;
    if(selectedType == TS)
    {
        name = QStringLiteral(u"TS");
        queryStr = QStringLiteral(u"SELECT * FROM TS;");
    }
    else if(selectedType == TU)
    {
        name = QStringLiteral(u"TU");
        queryStr = QStringLiteral(u"SELECT * FROM TU;");
    }
    else if(selectedType == RouteSrc)
    {
        name = QStringLiteral(u"RouteSrc");
        queryStr = QStringLiteral(u"SELECT * FROM RouteSrc;");
    }
    else if(selectedType == Stations)
    {
        name = QStringLiteral(u"Stations");
        queryStr = QStringLiteral(u"SELECT * FROM Stations;");
    }
    else if(selectedType == TuSpok)
    {
        name = QStringLiteral(u"TuSpok");
        queryStr = QStringLiteral(u"SELECT * FROM TuSpok;");
    }

    QSqlQuery query1(getDb(m_tableLeft->getNameDB()));
    query1.exec(queryStr);
    query1.next();
    QStringList listOfStations1;
    int i = 0;
    auto record1 = query1.record();
    while(!record1.fieldName(i).isEmpty())
        listOfStations1 << record1.fieldName(i++);

    QSqlQuery query2(getDb(m_tableRight->getNameDB()));
    query2.exec(queryStr);
    query2.next();
    QStringList listOfStations2;
    i = 0;
    auto record2 = query2.record();
    while(!record2.fieldName(i).isEmpty())
        listOfStations2 << record2.fieldName(i++);

    QStringList listFinal;
    QStringList listDiff;
    for(const auto &s : qAsConst(listOfStations1))
    {
        if(((selectedType == Stations) && (s == QLatin1String("NoSt")))
                || ((selectedType != TuSpok) && (s != QLatin1String("Cod")) && (s != QLatin1String("NoSt")))
                || ((selectedType == TuSpok) && (s != QLatin1String("No") && (s != QLatin1String("NoSt")))))
        {
            if(listOfStations2.contains(s))
                listFinal.append(s);
            else
                listDiff.append(s);
        }
    }

    if(diff && (listDiff.size() > 0))
        QMessageBox::information(this, QStringLiteral(u"Разные поля"), QString(QStringLiteral(u"Базы данных справа имеет недостающие поля в таблице %1, поля %2")).arg(name, listDiff.join(", ")));

    form->raise();

    return listFinal;
}

void Widget::openColumns()
{
    form->raise();
    form->show();
}

void Widget::setTable(const QString &name, const QString &q)
{
    if((name == ui->labelLeft->text()) || (name == ui->labelRight->text()))
    {
        QMessageBox::critical (this, QStringLiteral(u"Ошибка"), QStringLiteral(u"Выбранная база уже открыта!"));
        return;
    }

    if (q == QLatin1String("openButton1"))
    {
        if(m_tableLeft)
            m_tableLeft->resetTable(name);
        else
        {
            m_tableLeft = new MyTable(name);
            m_tableLeft->setTable(ui->tableWidgetLeft);
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
            m_tableRight->setTable(ui->tableWidgetRight);
        }
        ui->labelRight->setText(name);
    }

    if(m_tableLeft && m_tableRight)
    {
        if(m_tableLeft->isReady() && m_tableRight->isReady())
        {
            comboBox->clear();
            comboBox->addItems(QStringList() << QStringLiteral(u"Stations") << QStringLiteral(u"TS") << QStringLiteral(u"TU") << QStringLiteral(u"TuSpok")<< QStringLiteral(u"RouteSrc"));
            selectedType = Stations;
            form->updateWidget(getListOfColumns(false));

            form->on_pushButton_2_clicked();
            compareDBs();
            form->on_pushButton_3_clicked();

#if(QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=] (int index)
            {
                selectedType = (tableType)index;
                form->updateWidget(getListOfColumns(true));
            });
#else
            connect(comboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, [=] (QString string)
            {
                int index = comboBox->findText(string);
                selectedType = (tableType)index;
                form->updateWidget(getListOfColumns(true));
            });
#endif
        }
    }
}

void Widget::doubleClickedTableLeftItem(QTableWidgetItem *item)
{
    if(m_isCompared || !(m_tableLeft->isReady() && m_tableRight->isReady()))
        return;

    int row = item->row();
    int tableLeftColumnCount = ui->tableWidgetLeft->columnCount();
    int tableRightColumnCount = ui->tableWidgetRight->columnCount();
    int tableRightRowCount = ui->tableWidgetRight->rowCount();
    bool found = false;
    for(int i = 0; i < tableLeftColumnCount; ++i)
    {
        auto itemH = ui->tableWidgetLeft->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == QLatin1String("NameSt"))
        {
            item = ui->tableWidgetLeft->item(row, i);
            found = true;
            break;
        }
    }

    if(!found)
        return;

    if((!m_item1) || (m_item1->background() != m_brushGreen))
    {
        m_brush1 = item->background();
        item->setBackground(m_brushGreen);

        for(int i = 0; i < tableLeftColumnCount; ++i)
            ui->tableWidgetLeft->item(row, i)->setBackground(m_brushGreen);

        m_tableLeft->setStation(item->text());
        m_item1 = item;
    }
    else if (m_item1->background() == m_brushGreen)
    {
        m_item1->setBackground(m_brush1);

        for(int i = 0; i < tableLeftColumnCount; ++i)
            ui->tableWidgetLeft->item(m_item1->row(), i)->setBackground(m_brush1);

        m_brush1 = item->background();
        item->setBackground(m_brushGreen);

        for(int i = 0; i < tableLeftColumnCount; ++i)
            ui->tableWidgetLeft->item(row, i)->setBackground(m_brushGreen);

        m_tableLeft->setStation(item->text());
        m_item1 = item;
    }
    ui->tableWidgetLeft->clearSelection();

    int columnSecond = -1;
    for(int i = 0; i < tableRightColumnCount; ++i)
    {
        auto itemH = ui->tableWidgetRight->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == QLatin1String("NameSt"))
            columnSecond = i;
    }
    if(columnSecond == -1)
    {
        QMessageBox::information(this, QStringLiteral(u"Станция не найдена"), QStringLiteral(u"В противоположной таблице не была найдена выбранная вами сатнция, выберите другую станцию!"));
        return;
    }

    for(int i = 0; i < tableRightRowCount; ++i)
    {
        auto itemV = ui->tableWidgetRight->item(i, columnSecond);

        if(itemV->data(Qt::DisplayRole) == item->data(Qt::DisplayRole))
        {
            item = itemV;
            row = item->row();

            if((!m_item2) || (m_item2->background() != m_brushGreen))
            {
                m_brush2 = item->background();
                item->setBackground(m_brushGreen);

                for(int column = 0; column < tableRightColumnCount; ++column)
                    ui->tableWidgetRight->item(row, column)->setBackground(m_brushGreen);

                m_tableRight->setStation(item->text());
                m_item2 = item;
            }
            else if(m_item2->background() == m_brushGreen)
            {
                m_item2->setBackground(m_brush2);

                for(int column = 0; column < tableRightColumnCount; ++column)
                    ui->tableWidgetRight->item(m_item2->row(), column)->setBackground(m_brush2);

                m_brush2 = item->background();;
                item->setBackground(m_brushGreen);

                for(int column = 0; column < tableRightColumnCount; ++column)
                    ui->tableWidgetRight->item(row, column)->setBackground(m_brushGreen);

                m_tableRight->setStation(item->text());
                m_item2 = item;
            }
            ui->tableWidgetRight->clearSelection();
        }
    }
}

void Widget::doubleClickedTableRightItem(QTableWidgetItem *item)
{
    if(m_isCompared || !(m_tableLeft->isReady() && m_tableRight->isReady()))
        return;

    int row = item->row();
    int tableLeftColumnCount = ui->tableWidgetLeft->columnCount();
    int tableRightColumnCount = ui->tableWidgetRight->columnCount();
    int tableLeftRowCount = ui->tableWidgetLeft->rowCount();
    bool found = false;
    for(int i = 0; i < tableRightColumnCount; ++i)
    {
        auto itemH = ui->tableWidgetRight->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == QLatin1String("NameSt"))
        {
            item = ui->tableWidgetRight->item(row, i);
            found = true;
            break;
        }
    }
    if(!found)
        return;

    if((!m_item2) || (m_item2->background() != m_brushGreen))
    {
        m_brush2 = item->background();

        for(int i = 0; i < tableRightColumnCount; ++i)
            ui->tableWidgetRight->item(row, i)->setBackground(m_brushGreen);

        m_tableRight->setStation(item->text());
        m_item2 = item;
    }
    else if(m_item2->background() == m_brushGreen)
    {
        m_item2->setBackground(m_brush2);

        for(int i = 0; i < tableRightColumnCount; ++i)
            ui->tableWidgetRight->item(m_item2->row(), i)->setBackground(m_brush2);

        m_brush2 = item->background();
        item->setBackground(m_brushGreen);

        for(int i = 0; i < tableRightColumnCount; ++i)
            ui->tableWidgetRight->item(row, i)->setBackground(m_brushGreen);

        m_tableRight->setStation(item->text());
        m_item2 = item;
    }
    ui->tableWidgetRight->clearSelection();

    int columnSecond = -1;
    for(int i = 0; i < tableLeftColumnCount; ++i)
    {
        auto itemH = ui->tableWidgetLeft->horizontalHeaderItem(i);
        if(itemH->data(Qt::DisplayRole) == QLatin1String("NameSt"))
            columnSecond = i;
    }
    if(columnSecond == -1)
    {
        QMessageBox::information(this, QStringLiteral(u"Станция не найдена"), QStringLiteral(u"В противоположной таблице не была найдена выбранная вами сатнция, выберите другую станцию!"));
        return;
    }

    for(int i = 0; i < tableLeftRowCount; ++i)
    {
        auto itemV = ui->tableWidgetLeft->item(i, columnSecond);
        if(itemV->data(Qt::DisplayRole) == item->data(Qt::DisplayRole))
        {
            item = itemV;
            row = item->row();
            if((!m_item1) || (m_item1->background() != m_brushGreen))
            {
                m_brush1 = item->background();
                item->setBackground(m_brushGreen);

                for(int column = 0; column < tableLeftColumnCount; ++column)
                    ui->tableWidgetLeft->item(row, column)->setBackground(m_brushGreen);

                m_tableLeft->setStation(item->text());
                m_item1 = item;
            }
            else if (m_item1->background() == m_brushGreen)
            {
                m_item1->setBackground(m_brush1);

                for(int column = 0; column < tableLeftColumnCount; ++column)
                    ui->tableWidgetLeft->item(m_item1->row(), column)->setBackground(m_brush1);

                m_brush1 = item->background();
                item->setBackground(m_brushGreen);

                for(int column = 0; column < tableLeftColumnCount; ++column)
                    ui->tableWidgetLeft->item(row, column)->setBackground(m_brushGreen);

                m_tableLeft->setStation(item->text());
                m_item1 = item;
            }
            ui->tableWidgetLeft->clearSelection();
        }
    }
}

Widget::~Widget()
{
    delete m_tableRight;
    delete m_tableLeft;
    QStringList list =  QSqlDatabase::connectionNames();
    for(int i = 0; i < list.size(); ++i)
        QSqlDatabase::removeDatabase(list[i]);
}

void Widget::on_pushButtonUp_clicked()
{
    int vectorSize = vectorOfDiffRows.size();
    if(m_isCompared && (vectorSize > 0))
    {
        if((currentIndex == -1) || !(--currentIndex >= 0))
            currentIndex = vectorSize - 1;

        int row = vectorOfDiffRows.at(currentIndex);
        auto item = ui->tableWidgetLeft->item(row, 0);
        ui->tableWidgetLeft->scrollToItem(item, QAbstractItemView::PositionAtTop);
    }
}

void Widget::on_pushButtonDown_clicked()
{
    int vectorSize = vectorOfDiffRows.size();
    if(m_isCompared && (vectorSize > 0))
    {
        if(!(vectorSize > (++currentIndex)))
            currentIndex = 0;

        int row = vectorOfDiffRows.at(currentIndex);
        auto item = ui->tableWidgetLeft->item(row, 0);
        ui->tableWidgetLeft->scrollToItem(item, QAbstractItemView::PositionAtTop);
    }
}
