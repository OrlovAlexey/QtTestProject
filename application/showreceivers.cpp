#include "showreceivers.h"
#include "ui_showreceivers.h"

ShowReceivers::ShowReceivers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowReceivers)
{
    ui->setupUi(this);


}

ShowReceivers::~ShowReceivers()
{
    delete ui;
}
