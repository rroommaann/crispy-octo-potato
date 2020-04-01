#ifndef FORMSTATIONS_H
#define FORMSTATIONS_H

#include <QWidget>
#include <QCheckBox>


namespace Ui {
class FormStations;
}

class FormStations : public QWidget
{
    Q_OBJECT

public:
    explicit FormStations(QStringList list, QWidget *parent = nullptr);
    ~FormStations();
    void updateWidget(QStringList list);


    QStringList getListofColumns() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormStations *ui;
    QStringList list;
    QList<QCheckBox*> listOfColumns;
};

#endif // FORMSTATIONS_H
