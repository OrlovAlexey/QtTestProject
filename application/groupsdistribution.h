#ifndef GROUPSDISTRIBUTION_H
#define GROUPSDISTRIBUTION_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidget>
#include <QCheckBox>
#include <QVector>
#include <QSet>
#include <QStringList>

namespace Ui {
class GroupsDistribution;
}

class GroupsDistribution : public QDialog
{
    Q_OBJECT

public:
    explicit GroupsDistribution(QSqlDatabase db, QWidget *parent = nullptr);
    ~GroupsDistribution();

private slots:
    void on_pushButton_clicked();

    void on_tableViewReceivers_clicked(const QModelIndex &index);

private:
    Ui::GroupsDistribution *ui;
    QSqlTableModel* model_receivers;
};

#endif // GROUPSDISTRIBUTION_H
