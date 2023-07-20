#ifndef SHOWTABLE_H
#define SHOWTABLE_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class ShowTable;
}

class ShowTable : public QDialog
{
    Q_OBJECT

public:
    explicit ShowTable(QSqlDatabase db, QString table, QWidget *parent = nullptr);
    ~ShowTable();

private:
    Ui::ShowTable* ui;
    QSqlTableModel* model;
};

#endif // SHOWTABLE_H
