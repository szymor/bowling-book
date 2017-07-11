#include "formwindow.h"
#include "ui_formwindow.h"
#include <QMessageBox>

FormWindow::FormWindow(QWidget *parent, QDate date, QList<QTableWidgetItem*> list) :
    QDialog(parent),
    ui(new Ui::FormWindow)
{
    ui->setupUi(this);
    selDate = date;
    selList = list;

    Entry* entry;
    if( list.count() == 1 )
        entry = dm.GetEntry(date, list.first()->row(), list.first()->column());
    else
        entry = NULL;

    if( entry )
    {
        ui->eFirstName->setText(entry->firstname);
        ui->eLastName->setText(entry->lastname);
        ui->ePhone->setText(entry->phone);
        ui->eNotes->setPlainText(entry->notes);
        ui->cbPayment->setChecked(entry->payment);
        ui->ePrice->setText(QString::number(entry->price, 'f', 2));
        ui->eRecipient->setText(entry->recipient);
        ui->eReceptionTime->setDateTime(entry->receptiontime);
    }
    else
    {
        ui->eFirstName->setText("");
        ui->eLastName->setText("");
        ui->ePhone->setText("");
        ui->eNotes->setPlainText("");
        ui->cbPayment->setChecked(false);
        ui->ePrice->setText("");
        ui->eRecipient->setText("");
        ui->eReceptionTime->setDateTime(QDateTime::currentDateTime());
    }
    on_cbPayment_clicked();
}

FormWindow::~FormWindow()
{
    delete ui;
}

void FormWindow::on_bBook_clicked()
{
    Entry entry;
    entry.firstname = ui->eFirstName->text();
    entry.lastname = ui->eLastName->text();
    entry.phone = ui->ePhone->text();
    entry.notes = ui->eNotes->toPlainText();
    entry.payment = ui->cbPayment->isChecked();
    entry.price = ui->ePrice->text().toDouble();
    entry.recipient = ui->eRecipient->text();
    entry.receptiontime = ui->eReceptionTime->dateTime();
    entry.date = this->selDate;

    if( (ui->eFirstName->text() == "") || (ui->eLastName->text() == "") || (ui->ePhone->text() == "") )
    {
        QMessageBox msgbox;
        msgbox.setText("Uzupełnij odpowiednie pola!");
        msgbox.exec();
        return;
    }
    if( bl.IsBlacklisted(entry.firstname, entry.lastname) )
    {
        if( QMessageBox::question(this, "Pytanie", "Podana osoba jest na czarnej liście. Czy na pewno chcesz rezerwować?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No )
            return;
    }
    if( bl.IsBlacklisted(entry.phone) )
    {
        if( QMessageBox::question(this, "Pytanie", "Podany numer telefonu jest na czarnej liście. Czy na pewno chcesz rezerwować?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No )
            return;
    }

    for(QList<QTableWidgetItem*>::iterator i = selList.begin(); i != selList.end(); ++i)
    {
        entry.hour = (*i)->row();
        entry.lane = (*i)->column();
        dm.AddEntry(entry);
    }
    dm.Save();
    this->close();
}

void FormWindow::on_cbPayment_clicked()
{
    bool aux = ui->cbPayment->isChecked();
    ui->ePrice->setEnabled(aux);
    ui->eRecipient->setEnabled(aux);
    ui->eReceptionTime->setEnabled(aux);
}
