#ifndef POSITIONSEDIT_H
#define POSITIONSEDIT_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "showtable.h"

namespace Ui {
class PositionsEdit;
}

class PositionsEdit : public QDialog
{
    Q_OBJECT

public:
    explicit PositionsEdit(QSqlDatabase database, QWidget *parent = nullptr);
    ~PositionsEdit();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonShow_clicked();

private:
    Ui::PositionsEdit *ui;
    QSqlDatabase db;
};

#endif // POSITIONSEDIT_H
