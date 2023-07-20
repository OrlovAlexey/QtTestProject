#include "showtable.h"
#include "ui_showtable.h"

ShowTable::ShowTable(QSqlDatabase db, QString table, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowTable)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this, db);
    //    model->setTable(table); // doesn't work with postgres
    //    model->select();

    model->QSqlQueryModel::setQuery("SELECT * FROM " + table, db);

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    if (table == "Positions") {
        model->setHeaderData(1, Qt::Horizontal, tr("Должность"));
    }
    if (table == "Receivers") {
        model->setHeaderData(1, Qt::Horizontal, tr("ФИО"));
        model->setHeaderData(2, Qt::Horizontal, tr("Email"));
        model->setHeaderData(3, Qt::Horizontal, tr("Должность"));
    }
    if (table == "Groups") {
        model->setHeaderData(1, Qt::Horizontal, tr("Группа"));
    }
}

ShowTable::~ShowTable()
{
    delete model;

    delete ui;
}
