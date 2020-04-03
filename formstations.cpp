#include "formstations.h"
#include "ui_formstations.h"
void FormStations::updateWidget(QStringList list)
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

    listOfColumns.clear();
    for(auto columns : list)
    {
        static int c = 0;
        static int r = 0;
        QCheckBox *box = new QCheckBox(columns);
        ui->gridLayoutOfStations->addWidget(box, r, c++ % 3);
        listOfColumns.append(box);
        if(c % 3 == 0)
        {
            r++;
        }
    }
}

QStringList FormStations::getListofColumns() const
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

FormStations::FormStations(QStringList list, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStations)
{
    ui->setupUi(this);

    updateWidget(list);
}

FormStations::~FormStations()
{
    delete ui;
}

void FormStations::on_pushButton_clicked()
{
    this->hide();
}
