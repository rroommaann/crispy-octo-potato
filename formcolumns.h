#ifndef FORMSTATIONS_H
#define FORMSTATIONS_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSet>


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

signals:
    void compare();

public slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::FormStations *ui;
    QStringList list;
    QList<QCheckBox*> listOfCheckBoxes;
    QHash<int, QSet<QString>> hashOfLastSelection;

    void synchBoxes();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // FORMSTATIONS_H
