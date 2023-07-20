#include "receiversedit.h"
#include "ui_receiversedit.h"

ReceiversEdit::ReceiversEdit(QSqlDatabase database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiversEdit)
{
    ui->setupUi(this);
    ui->lineEditName->setPlaceholderText("Введите ФИО");
    ui->lineEditEmail->setPlaceholderText("Введите email");
    ui->comboBox->setPlaceholderText("Выберите должность");

    db = database;

    // заполняем comboBox данными из таблицы Positions
    QSqlQuery get_query;
    bool isSuccessful = get_query.exec("SELECT * FROM Positions;");
    if (!get_query.isSelect() || !isSuccessful) {
        qDebug() << get_query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Невозможно прочитать таблицу должностей");
    }
    while (get_query.next()) {
        ui->comboBox->addItem(get_query.value(1).toString());
    }
}

ReceiversEdit::~ReceiversEdit()
{
    delete ui;
}


void ReceiversEdit::on_pushButtonAdd_clicked()
{
    QString name = ui->lineEditName->text();
    QString email = ui->lineEditEmail->text();
    QString position = ui->comboBox->currentText();

    QSqlQuery insert_query;
    insert_query.prepare("INSERT INTO Receivers(name, email, position) VALUES (:name, :email, :position); ");
    insert_query.bindValue(":name", name);
    insert_query.bindValue(":email", email);
    insert_query.bindValue(":position", position);
    bool isSuccessful = insert_query.exec();

    if (!isSuccessful) {
        QString error = insert_query.lastError().text();
        if (error.contains("duplicate key value violates unique constraint")) {
            QMessageBox::warning(this, "Предупреждение", "Данные с таким email в таблице уже содержатся.");
            return;
        }

        QMessageBox::critical(this, "Ошибка", "Данные не удалось вставить в таблицу!");
        qDebug() << error;
        return;
    }

    QMessageBox::information(this, "Вставлено", "Данные успешно вставлены в таблицу!");
    ui->lineEditName->clear();
    ui->lineEditEmail->clear();
    ui->comboBox->clearEditText();
}


void ReceiversEdit::on_pushButtonDelete_clicked()
{
    QString name = ui->lineEditName->text();
    QString email = ui->lineEditEmail->text();
    QString position = ui->comboBox->currentText();

    QSqlQuery delete_query;
    delete_query.prepare("DELETE FROM Receivers WHERE name=:name AND email=:email AND position=:position;");
    delete_query.bindValue(":name", name);
    delete_query.bindValue(":email", email);
    delete_query.bindValue(":position", position);
    bool isSuccessful = delete_query.exec();

    if (!isSuccessful) {
        QString error = delete_query.lastError().text();
        if (error.contains("violates foreign key constraint")) {
            QMessageBox::warning(this, "Предупреждение", "Данные не были удалены, сначала уберите этого получателя из всех групп!");
            return;
        }
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить данные из таблицы!");
        qDebug() << error;
        return;
    }

    if (delete_query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Данные не были удалены из таблицы (таких данных там нет).");
        return;
    }

    QMessageBox::information(this, "Удалено", "Данные удалены из таблицы успешно!");
    ui->lineEditName->clear();
    ui->lineEditEmail->clear();
    ui->comboBox->clearEditText();
}


void ReceiversEdit::on_pushButtonShow_clicked()
{
    ShowTable window(db, "Receivers");
    window.setModal(true);
    window.exec();
}

