#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>

#include "receiversedit.h"
#include "positionsedit.h"
#include "groupsedit.h"
#include "groupsdistribution.h"
#include "groupsselection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton1_clicked();

    void on_pushButton2_clicked();

    void on_pushButton3_clicked();

    void on_pushButton4_clicked();

    void on_pushButton_clicked();

private:
    void InitDB();
    template <typename T>
    void showNewWindow();

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery* query1;
    QSqlQuery* query2;
    QSqlQuery* query3;
    QSqlQuery* query4;
};
#endif // MAINWINDOW_H
