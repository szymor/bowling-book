#include "blacklistwindow.h"
#include "ui_blacklistwindow.h"

BlackListWindow::BlackListWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackListWindow)
{
    ui->setupUi(this);
    reload();
}

BlackListWindow::~BlackListWindow()
{
    clear_items();
    delete ui;
}

void BlackListWindow::reload()
{
    this->clear_items();
    ui->tableWidget->setRowCount(bl.people.count());
    int j = 0;
    for(QVector<Person>::iterator i = bl.people.begin(); i != bl.people.end(); ++i)
    {
        ui->tableWidget->setItem(j,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(j,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(j,2, new QTableWidgetItem(""));
        ui->tableWidget->item(j,0)->setText(i->firstname);
        ui->tableWidget->item(j,1)->setText(i->lastname);
        ui->tableWidget->item(j,2)->setText(i->phone);
        ++j;
    }
}

void BlackListWindow::clear_items()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            if( ui->tableWidget->item(i,j) )
                delete ui->tableWidget->item(i,j);
        }
}

void BlackListWindow::on_bDelete_clicked()
{
    if( ui->tableWidget->selectedItems().count() != 1 )
        return;
    int row = ui->tableWidget->selectedItems().front()->row();
    QString first, last, phone;
    first = ui->tableWidget->item(row,0)->text();
    last = ui->tableWidget->item(row,1)->text();
    phone = ui->tableWidget->item(row,2)->text();
    bl.RemovePerson(first, last, phone);
    bl.Save();
    reload();
}
