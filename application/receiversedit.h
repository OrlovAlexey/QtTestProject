#ifndef RECEIVERSEDIT_H
#define RECEIVERSEDIT_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "showtable.h"

namespace Ui {
class ReceiversEdit;
}

class ReceiversEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiversEdit(QSqlDatabase database, QWidget *parent = nullptr);
    ~ReceiversEdit();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonShow_clicked();

private:
    Ui::ReceiversEdit *ui;
    QSqlDatabase db;
};

#endif // RECEIVERSEDIT_H
