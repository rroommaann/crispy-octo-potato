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

    int c = 0;
    int r = 0;
    listOfColumns.clear();
    for(auto columns : list)
    {
        QCheckBox *box = new QCheckBox(columns);
        ui->gridLayoutOfStations->addWidget(box, r, c++ % 3);
        listOfColumns.append(box);
        if(c % 3 == 0)
        {
            r++;
        }
    }
}

QStringList FormColumns::getListofColumns() const
{
    QStringList list;

    for(auto p : listOfColumns)
    {
        if(p->isChecked())
        {
            list.append(p->text());
        }
    }

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
    setWindowFlags((Qt::WindowFlags)(Qt::WindowStaysOnTopHint | Qt::Dialog  | Qt::WindowCloseButtonHint));

    updateWidget(list);
}

FormColumns::~FormColumns()
{
    delete ui;
}

void FormColumns::on_pushButton_clicked()
{
    this->hide();
    emit ok();
}

void FormColumns::on_pushButton_2_clicked()
{
    for(auto p : listOfColumns)
    {
        if(!p->isChecked())
        {
            p->setChecked(true);
        }
    }
}

void FormColumns::on_pushButton_3_clicked()
{
    for(auto p : listOfColumns)
    {
        if(p->isChecked())
        {
            p->setChecked(false);
        }
    }
}

void FormColumns::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}
