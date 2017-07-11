#include "lanereservationwindow.h"
#include "ui_lanereservationwindow.h"

LaneReservationWindow::LaneReservationWindow(QWidget *parent, QDate date) :
    QDialog(parent),
    ui(new Ui::LaneReservationWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(date.toString());
    selDate = date;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(""));
        }
    reload();
}

LaneReservationWindow::~LaneReservationWindow()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            delete ui->tableWidget->item(i,j);
        }
    delete ui;
}

void LaneReservationWindow::on_bBook_clicked()
{
    FormWindow* fw;
    fw = new FormWindow(this, selDate, ui->tableWidget->selectedItems());
    fw->exec();
    delete fw;
    reload();
}

void LaneReservationWindow::reload()
{
    Entry* entry;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            entry = dm.GetEntry(selDate,i,j);
            if( entry )
            {
                ui->tableWidget->item(i,j)->setText(entry->firstname + " " + entry->lastname);
                if( entry->payment )
                    ui->tableWidget->item(i,j)->setBackgroundColor(QColor(0,255,0));
                else
                    ui->tableWidget->item(i,j)->setBackgroundColor(QColor(255,0,0));
            }
        }
}

void LaneReservationWindow::on_bDelete_clicked()
{
    if( ui->tableWidget->selectedItems().count() == 0 )
        return;
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
    for(QList<QTableWidgetItem*>::iterator i = list.begin(); i != list.end(); ++i)
    {
        (*i)->setText("");
        (*i)->setBackgroundColor(QColor(255,255,255));
        dm.RemoveEntry(selDate,(*i)->row(),(*i)->column());
    }
    dm.Save();
}

void LaneReservationWindow::on_bDeleteAndBlacklist_clicked()
{
    if( ui->tableWidget->selectedItems().count() == 0 )
        return;
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
    for(QList<QTableWidgetItem*>::iterator i = list.begin(); i != list.end(); ++i)
    {
        (*i)->setText("");
        (*i)->setBackgroundColor(QColor(255,255,255));

        Person person;
        Entry* entry;
        entry = dm.GetEntry(selDate,(*i)->row(),(*i)->column());
        person.firstname = entry->firstname;
        person.lastname = entry->lastname;
        person.phone = entry->phone;
        bl.AddPerson(person);
        dm.RemoveEntry(selDate,(*i)->row(),(*i)->column());
    }
    dm.Save();
    bl.Save();
}
