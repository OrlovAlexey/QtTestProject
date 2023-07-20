#include "groupsdistribution.h"
#include "ui_groupsdistribution.h"

GroupsDistribution::GroupsDistribution(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupsDistribution)
{
    ui->setupUi(this);

    model_receivers = new QSqlTableModel(this, db);
    model_receivers->QSqlQueryModel::setQuery("SELECT * FROM Receivers", db);

    ui->tableViewReceivers->setModel(model_receivers);
    ui->tableViewReceivers->setColumnHidden(0, true);
    ui->tableViewReceivers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    model_receivers->setHeaderData(1, Qt::Horizontal, tr("ФИО"));
    model_receivers->setHeaderData(2, Qt::Horizontal, tr("Email"));
    model_receivers->setHeaderData(3, Qt::Horizontal, tr("Должность"));


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
    table->setColumnHidden(1, true);
    QStringList labels = QStringList() << "Выбрать" << "ID" << "Группа";
    table->setHorizontalHeaderLabels(labels);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

GroupsDistribution::~GroupsDistribution()
{
    delete model_receivers;

    delete ui;
}

void GroupsDistribution::on_pushButton_clicked()
{
    // получение id получателя из левой таблицы
    int currentRow = ui->tableViewReceivers->currentIndex().row();
    int receiver_id = ui->tableViewReceivers->model()->index(currentRow, 0).data().toInt();

    // получение id групп с отмеченными чекбоксами из правой таблицы
    QVector<int> group_ids;
    QTableWidget* table = ui->tableWidgetGroups;
    for (int row = 0; row < table->rowCount(); ++row) {
        QCheckBox* currentGroupCB = static_cast<QCheckBox*>(table->cellWidget(row, 0));
        if (currentGroupCB->isChecked()) {
            QTableWidgetItem* currentGroupId = table->item(row, 1);
            group_ids.push_back(currentGroupId->text().toInt());
        }
    }

    // удаление пар (group_id, receiver_id) из таблицы связи с данным receiver_id
    QSqlQuery delete_query;
    delete_query.prepare("DELETE FROM Groups_Receivers WHERE receiver_id=:receiver_id;");
    delete_query.bindValue(":receiver_id", receiver_id);
    bool isSuccessfulDelete = delete_query.exec();
    if (!isSuccessfulDelete) {
        QString error = delete_query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось распределить получателя по группам (удалить старые связи в таблице)!");
        qDebug() << error;
        return;
    }

    // вставка пар (group_id, receiver_id) в таблицу связи с данным receiver_id
    for ( int group_id : group_ids ) {
        QSqlQuery insert_query;
        insert_query.prepare("INSERT INTO Groups_Receivers(group_id, receiver_id) VALUES (:group_id, :receiver_id); ");
        insert_query.bindValue(":group_id", group_id);
        insert_query.bindValue(":receiver_id", receiver_id);
        bool isSuccessful = insert_query.exec();
        if (!isSuccessful) {
            QString error = insert_query.lastError().text();
            if (error.contains("duplicate key value violates unique constraint")) {
                continue;
            }
            QMessageBox::critical(this, "Ошибка", "Не удалось распределить получателя по группам (вставить в таблицу)!");
            qDebug() << error;
            return;
        }
    }

    QMessageBox::information(this, "Успешно", "Получатель успешно распределен!");
}


void GroupsDistribution::on_tableViewReceivers_clicked(const QModelIndex &index)
{
    // получение id получателя из левой таблицы
    int receiver_id = ui->tableViewReceivers->model()->index(index.row(), 0).data().toInt();

    // получение всех id групп, связанных с этим receiver_id
    QSqlQuery get_query;
    get_query.prepare("SELECT * FROM Groups_Receivers WHERE receiver_id=:receiver_id;");
    get_query.bindValue(":receiver_id", receiver_id);
    bool isSuccessful = get_query.exec();
    if (!get_query.isSelect() || !isSuccessful) {
        qDebug() << get_query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Невозможно прочитать таблицу групп!");
    }

    // для быстрой проверки наличия данного group_id
    QSet<int> checked_group_ids;
    while (get_query.next()) {
        checked_group_ids.insert(get_query.value(0).toInt());
    }

    // отображение отметки чекбоксов для idшников из checked_group_ids
    QTableWidget* table = ui->tableWidgetGroups;
    for (int row = 0; row < table->rowCount(); ++row) {
        QCheckBox* currentGroupCB = static_cast<QCheckBox*>(table->cellWidget(row, 0));
        int group_id = table->item(row, 1)->text().toInt();
        bool isCurrentGroupChecked = (checked_group_ids.find(group_id) != checked_group_ids.end());
        currentGroupCB->setChecked(isCurrentGroupChecked);
    }
}

