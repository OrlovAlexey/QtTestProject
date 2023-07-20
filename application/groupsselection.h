#ifndef GROUPSSELECTION_H
#define GROUPSSELECTION_H

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
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

namespace Ui {
class GroupsSelection;
}

class GroupsSelection : public QDialog
{
    Q_OBJECT

public:
    explicit GroupsSelection(QSqlDatabase db, QWidget *parent = nullptr);
    ~GroupsSelection();

private slots:
    void on_pushButtonShow_clicked();

    void on_pushButtonDownload_clicked();

private:
    Ui::GroupsSelection *ui;
};

#endif // GROUPSSELECTION_H
