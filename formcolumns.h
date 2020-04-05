#ifndef FORMSTATIONS_H
#define FORMSTATIONS_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>


namespace Ui {
class FormStations;
}

class FormColumns : public QWidget
{
    Q_OBJECT

public:
    explicit FormColumns(QStringList list, QWidget *parent = nullptr);
    ~FormColumns();
    void updateWidget(QStringList list);
    QStringList getListofColumns() const;

    QComboBox *getComboBox();
public slots:
    void on_pushButton_2_clicked();

signals:
    void ok();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::FormStations *ui;
    QStringList list;
    QList<QCheckBox*> listOfColumns;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // FORMSTATIONS_H
