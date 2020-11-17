#include "formcolumns.h"
#include "ui_formcolumns.h"
void FormColumns::updateWidget(QStringList list)
{
    if (ui->gridLayoutOfStations)
    {
        QLayoutItem* item;
        while ((item = ui->gridLayoutOfStations->takeAt( 0 )))
        {
            delete item->widget();
            delete item;
        }
    }

    listOfCheckBoxes.clear();
    int listSize = list.size();
    short offset = listSize % 4;
    short rows = (listSize / 4) - 1;

    static short currentRow = 0;
    static short currentColumn = 0;
    for(auto column : list)
    {
        QCheckBox *box = new QCheckBox(column);
        box->setChecked(false);

        ui->gridLayoutOfStations->addWidget(box, currentRow, currentColumn);
        listOfCheckBoxes.append(box);
        if(currentRow < rows)
            ++currentRow;
        else if(currentRow == rows)
        {
            if(offset - currentColumn > 0)
                ++currentRow;
            else
            {
                currentRow = 0;
                ++currentColumn;
            }
        }
        else
        {
            currentRow = 0;
            ++currentColumn;
        }
    }
    currentRow = 0;
    currentColumn = 0;
    synchBoxes();
}

QStringList FormColumns::getListofColumns() const
{
    QStringList list;

    for(auto p : listOfCheckBoxes)
        if(p->isChecked())
            list.append(p->text());

    return list;
}

QComboBox *FormColumns::getComboBox()
{
    return ui->comboBox;
}

FormColumns::FormColumns(QStringList list, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStations)
{
    ui->setupUi(this);
    setWindowFlags((Qt::WindowFlags)(Qt::Dialog  | Qt::WindowCloseButtonHint));

    updateWidget(list);
}

FormColumns::~FormColumns()
{
    delete ui;
}

void FormColumns::on_pushButton_clicked()
{
    this->hide();
    for(auto box : listOfCheckBoxes)
    {
        if(box->checkState() == Qt::Checked)
        {
            if(hashOfLastSelection.contains(ui->comboBox->currentIndex()))
            {
                auto set = hashOfLastSelection.value(ui->comboBox->currentIndex());
                QString text = box->text();
                if(!set.contains(text))
                {
                    set.insert(text);
                    hashOfLastSelection.insert(ui->comboBox->currentIndex(), set);
                }
            }
            else
            {
                QSet<QString> set;
                set.insert(box->text());
                hashOfLastSelection.insert(ui->comboBox->currentIndex(), set);
            }
        }
        else if(box->checkState() == Qt::Unchecked)
        {
            if(hashOfLastSelection.contains(ui->comboBox->currentIndex()))
            {
                auto set = hashOfLastSelection.value(ui->comboBox->currentIndex());
                QString text = box->text();
                if(set.contains(text))
                    set.remove(text);
            }
        }
    }
}

void FormColumns::on_pushButton_2_clicked()
{
    for(auto p : listOfCheckBoxes)
        if(!p->isChecked())
            p->setChecked(true);
}

void FormColumns::on_pushButton_3_clicked()
{
    for(auto p : listOfCheckBoxes)
        if(p->isChecked())
            p->setChecked(false);
}

void FormColumns::synchBoxes()
{
    if(hashOfLastSelection.contains(ui->comboBox->currentIndex()))
    {
        auto set = hashOfLastSelection.value(ui->comboBox->currentIndex());
        for(auto p : listOfCheckBoxes)
            if(set.contains(p->text()))
                p->setChecked(true);
            else
                p->setChecked(false);
    }
}

void FormColumns::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void FormColumns::on_pushButton_4_clicked()
{
    emit compare();
}
