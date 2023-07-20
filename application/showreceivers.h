#ifndef SHOWRECEIVERS_H
#define SHOWRECEIVERS_H

#include <QDialog>

namespace Ui {
class ShowReceivers;
}

class ShowReceivers : public QDialog
{
    Q_OBJECT

public:
    explicit ShowReceivers(QWidget *parent = nullptr);
    ~ShowReceivers();

private:
    Ui::ShowReceivers *ui;
};

#endif // SHOWRECEIVERS_H
