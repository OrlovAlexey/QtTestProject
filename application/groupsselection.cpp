#include "groupsselection.h"
#include "ui_groupsselection.h"

GroupsSelection::GroupsSelection(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupsSelection)
{
    ui->setupUi(this);

    // чтение таблицы групп
    QSqlQuery get_query;
    bool isSuccessful = get_query.exec("SELECT * FROM Groups;");
    if (!get_query.isSelect() || !isSuccessful) {
        qDebug() << get_query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Невозможно прочитать таблицу групп!");
    }

    // отображение таблицы групп с чекбоксами
    QTableWidget* table = ui->tableWidgetGroups;
    table->setColumnCount(3);
    table->setRowCount(get_query.size());
    int i = 0;
    while (get_query.next()) {
        QCheckBox* cb = new QCheckBox();
        table->setCellWidget(i, 0, cb);
        QTableWidgetItem* id = new QTableWidgetItem(get_query.value(0).toString());
        table->setItem(i, 1, id);
        QTableWidgetItem* group_name = new QTableWidgetItem(get_query.value(1).toString());
        table->setItem(i, 2, group_name);
        ++i;
    }
    table->setColumnHidden(1, true); // hide id
    QStringList labels = QStringList() << "Выбрать" << "ID" << "Группа";
    table->setHorizontalHeaderLabels(labels);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

GroupsSelection::~GroupsSelection()
{
    delete ui;
}


void GroupsSelection::on_pushButtonShow_clicked()
{
    // получение idшников отмеченных групп
    QVector<int> group_ids;
    QTableWidget* tableGroups = ui->tableWidgetGroups;
    for (int row = 0; row < tableGroups->rowCount(); ++row) {
        QCheckBox* currentGroupCB = static_cast<QCheckBox*>(tableGroups->cellWidget(row, 0));
        if (currentGroupCB->isChecked()) {
            QTableWidgetItem* currentGroupId = tableGroups->item(row, 1);
            group_ids.push_back(currentGroupId->text().toInt());
        }
    }

    // чтение таблицы связи, сохранение в сет всех receiver_id, связанных с текущим group_id
    QSet<int> receiver_ids;
    for ( int group_id : group_ids ) {
        QSqlQuery get_query;
        get_query.prepare("SELECT receiver_id FROM Groups_Receivers WHERE group_id=:group_id;");
        get_query.bindValue(":group_id", group_id);

        bool isSuccessful = get_query.exec();
        if (!isSuccessful) {
            QString error = get_query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Не удалось прочитать таблицу связи группы-получатели!");
            qDebug() << error;
            return;
        }

        while (get_query.next()) {
            receiver_ids.insert(get_query.value(0).toInt());
        }
    }

    // чтение таблицы получателей и их отображение (по одному)
    QTableWidget* tableReceivers = ui->tableWidgetReceivers;
    tableReceivers->setColumnCount(4);
    tableReceivers->setRowCount(receiver_ids.size());
    tableReceivers->setColumnHidden(0, true);
    QStringList labels = QStringList() << "ID" << "ФИО" << "Email" << "Должность";
    tableReceivers->setHorizontalHeaderLabels(labels);
    tableReceivers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for (int receiver_id : receiver_ids ) {
        QSqlQuery get_query;
        get_query.prepare("SELECT receiver_id, name, email, position FROM Receivers WHERE receiver_id=:receiver_id;");
        get_query.bindValue(":receiver_id", receiver_id);
        bool isSuccessful = get_query.exec();
        if (!get_query.isSelect() || !isSuccessful) {
            qDebug() << get_query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Невозможно прочитать таблицу получателей!");
        }

        get_query.next();
        QTableWidgetItem* id = new QTableWidgetItem(get_query.value(0).toString());
        tableReceivers->setItem(row, 0, id);
        QTableWidgetItem* name = new QTableWidgetItem(get_query.value(1).toString());
        tableReceivers->setItem(row, 1, name);
        QTableWidgetItem* email = new QTableWidgetItem(get_query.value(2).toString());
        tableReceivers->setItem(row, 2, email);
        QTableWidgetItem* position = new QTableWidgetItem(get_query.value(3).toString());
        tableReceivers->setItem(row, 3, position);

        ++row;
    }

    // сортировка по имени
    tableReceivers->sortByColumn(1, Qt::AscendingOrder);
}

void GroupsSelection::on_pushButtonDownload_clicked()
{
    // чтобы отобразить скачиваемую таблицу
    on_pushButtonShow_clicked();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "", tr("JSON files (*.json)"));
    if (fileName.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Невозможно сохранить в данный файл!");
        return;
    }

    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка", "Невозможно открыть данный файл!");
        return;
    }

    QTableWidget* table = ui->tableWidgetReceivers;
    int rowCount = table->model()->rowCount();
    int columnCount = table->model()->columnCount();

    QJsonObject json;
    json["rowCount"] = rowCount;
    json["columnCount"] = columnCount;

    QJsonArray data;
    for (int i = 0; i < rowCount; ++i) {
        QJsonArray row;
        for (int j = 0; j < columnCount; ++j) {
            row.append(QJsonValue(table->item(i, j)->text()));
        }
        data.append(row);
    }
    json["data"] = data;

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}
