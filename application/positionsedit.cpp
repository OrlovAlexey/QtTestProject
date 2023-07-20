#include "positionsedit.h"
#include "ui_positionsedit.h"

PositionsEdit::PositionsEdit(QSqlDatabase database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PositionsEdit)
{
    ui->setupUi(this);
    ui->lineEditPosition->setPlaceholderText("Введите должность");

    db = database;
}

PositionsEdit::~PositionsEdit()
{
    delete ui;
}

void PositionsEdit::on_pushButtonAdd_clicked()
{
    QString position = ui->lineEditPosition->text();

    QSqlQuery insert_query;
    insert_query.prepare("INSERT INTO Positions(position) VALUES (:position); ");
    insert_query.bindValue(":position", position);
    bool isSuccessful = insert_query.exec();

    if (!isSuccessful) {
        QString error = insert_query.lastError().text();
        if (error.contains("duplicate key value violates unique constraint")) {
            QMessageBox::warning(this, "Предупреждение", "Такая должность в таблице уже содержится.");
            return;
        }

        QMessageBox::critical(this, "Ошибка", "Данные не удалось вставить в таблицу!");
        qDebug() << error;
        return;
    }

    QMessageBox::information(this, "Вставлено", "Должность успешно вставлена в таблицу!");
    ui->lineEditPosition->clear();
}


void PositionsEdit::on_pushButtonDelete_clicked()
{
    QString position = ui->lineEditPosition->text();

    QSqlQuery delete_query;
    delete_query.prepare("DELETE FROM Positions WHERE position=:position;");

    delete_query.bindValue(":position", position);
    bool isSuccessful = delete_query.exec();

    if (!isSuccessful) {
        QString error = delete_query.lastError().text();

        QMessageBox::critical(this, "Ошибка", "Не удалось удалить данные из таблицы!");
        qDebug() << error;
        return;
    }

    if (delete_query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Данные не были удалены из таблицы (такой должности там нет).");
        return;
    }

    QMessageBox::information(this, "Удалено", "Должность удалена из таблицы успешно!");
    ui->lineEditPosition->clear();
}


void PositionsEdit::on_pushButtonShow_clicked()
{
    ShowTable window(db, "Positions");
    window.setModal(true);
    window.exec();
}

