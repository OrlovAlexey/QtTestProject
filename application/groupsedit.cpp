#include "groupsedit.h"
#include "ui_groupsedit.h"

GroupsEdit::GroupsEdit(QSqlDatabase database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupsEdit)
{
    ui->setupUi(this);
    ui->lineEditGroup->setPlaceholderText("Введите имя группы");

    db = database;
}

GroupsEdit::~GroupsEdit()
{
    delete ui;
}

void GroupsEdit::on_pushButtonAdd_3_clicked()
{
    QString group_name = ui->lineEditGroup->text();

    QSqlQuery insert_query;
    insert_query.prepare("INSERT INTO Groups(group_name) VALUES (:group_name); ");
    insert_query.bindValue(":group_name", group_name);
    bool isSuccessful = insert_query.exec();

    if (!isSuccessful) {
        QString error = insert_query.lastError().text();
        if (error.contains("duplicate key value violates unique constraint")) {
            QMessageBox::warning(this, "Предупреждение", "Такая группа в таблице уже содержится.");
            return;
        }

        QMessageBox::critical(this, "Ошибка", "Данные не удалось вставить в таблицу!");
        qDebug() << error;
        return;
    }

    QMessageBox::information(this, "Вставлено", "Группа успешно вставлена в таблицу!");
}


void GroupsEdit::on_pushButtonDelete_3_clicked()
{
    QString group_name = ui->lineEditGroup->text();

    QSqlQuery delete_query;
    delete_query.prepare("DELETE FROM Groups WHERE group_name=:group_name;");
    delete_query.bindValue(":group_name", group_name);
    bool isSuccessful = delete_query.exec();

    if (!isSuccessful) {
        QString error = delete_query.lastError().text();
        if (error.contains("violates foreign key constraint")) {
            QMessageBox::warning(this, "Предупреждение", "Данные не были удалены, сначала уберите всех получателей из этой группы!");
            return;
        }
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить данные из таблицы!");
        qDebug() << error;
        return;
    }
    if (delete_query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Данные не были удалены из таблицы (такой группы там нет).");
        return;
    }

    QMessageBox::information(this, "Удалено", "Группа удалена из таблицы успешно!");
    ui->lineEditGroup->clear();
}


void GroupsEdit::on_pushButtonShow_clicked()
{
    ShowTable window(db, "Groups");
    window.setModal(true);
    window.exec();
}

