#ifndef GROUPSEDIT_H
#define GROUPSEDIT_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "showtable.h"

namespace Ui {
class GroupsEdit;
}

class GroupsEdit : public QDialog
{
    Q_OBJECT

public:
    explicit GroupsEdit(QSqlDatabase database, QWidget *parent = nullptr);
    ~GroupsEdit();

private slots:
    void on_pushButtonAdd_3_clicked();

    void on_pushButtonDelete_3_clicked();

    void on_pushButtonShow_clicked();

private:
    Ui::GroupsEdit *ui;
    QSqlDatabase db;
};

#endif // GROUPSEDIT_H
