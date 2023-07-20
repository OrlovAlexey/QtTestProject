#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitDB();

    // таблица должностей
    query1 = new QSqlQuery(db);
    query1->exec("CREATE TABLE Positions(position_id SERIAL PRIMARY KEY, position TEXT UNIQUE NOT NULL);");

    // таблица получателей
    query2 = new QSqlQuery(db);
    query2->exec("CREATE TABLE Receivers(receiver_id SERIAL PRIMARY KEY, name TEXT NOT NULL, email TEXT UNIQUE NOT NULL, position TEXT NOT NULL);");

    // таблица групп
    query3 = new QSqlQuery(db);
    query3->exec("CREATE TABLE Groups(group_id SERIAL PRIMARY KEY, group_name TEXT UNIQUE NOT NULL);");

    // таблица связи получатели-группы
    query4 = new QSqlQuery(db);
    query4->exec("CREATE TABLE Groups_Receivers(group_id INTEGER REFERENCES Groups(group_id), \
                 receiver_id INTEGER REFERENCES Receivers(receiver_id), \
                 CONSTRAINT groups_receivers_pk PRIMARY KEY(group_id,receiver_id) );");
}

void MainWindow::InitDB() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setDatabaseName("postgres");
    db.setPassword("1234");
    if (db.open()) {
        qDebug("open");
    } else {
        qDebug("didn't open");
        QMessageBox::critical(this, "Ошибка", "Невозможно подключиться к базе данных!");
    }
    /* очистка базы данных
     *
    QSqlQuery query;
    query.exec("DROP TABLE Groups_Receivers;");
    query.exec("DROP TABLE Receivers;");
    query.exec("DROP TABLE Positions;");
    query.exec("DROP TABLE Groups;");*/
}


MainWindow::~MainWindow()
{
    db.close();
    delete query1;
    delete query2;
    delete query3;
    delete query4;

    delete ui;
}

template <typename T>
void MainWindow::showNewWindow() { // для удобства
    T window(db);
    window.setModal(true);
    window.exec();
}

void MainWindow::on_pushButton1_clicked()
{
    showNewWindow<ReceiversEdit>();
}


void MainWindow::on_pushButton2_clicked()
{
    showNewWindow<PositionsEdit>();
}


void MainWindow::on_pushButton3_clicked()
{
    showNewWindow<GroupsEdit>();
}


void MainWindow::on_pushButton4_clicked()
{
    showNewWindow<GroupsDistribution>();
}


void MainWindow::on_pushButton_clicked()
{
    showNewWindow<GroupsSelection>();
}

