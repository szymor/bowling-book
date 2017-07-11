#include "datechoicewindow.h"
#include "ui_datechoicewindow.h"
#include <QTextCharFormat>
#include <QMessageBox>
#include <windows.h>

DateChoiceWindow::DateChoiceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DateChoiceWindow)
{
    ui->setupUi(this);

    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(""));
        }

    this->on_calendarWidget_selectionChanged();
    tempEntry = NULL;


    DWORD vsn;
    GetVolumeInformationA(NULL,NULL,0,&vsn,NULL,NULL,NULL,0);
    // 0xBCEA4948 - moj pendrive
    // 0x18c4c1a1 - dysk C kregielnia
/*
    if(vsn != 0x18c4c1a1)
    {
        QMessageBox qmb;
        qmb.setText("Nie kradnij!");
        qmb.exec();
        exit(0);
    }
*/
}

DateChoiceWindow::~DateChoiceWindow()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            delete ui->tableWidget->item(i,j);
        }
    delete ui;
}

void DateChoiceWindow::on_bBook_clicked()
{
    FormWindow* fw;
    fw = new FormWindow(this, ui->calendarWidget->selectedDate(), ui->tableWidget->selectedItems());
    fw->exec();
    delete fw;
    reload();
}

void DateChoiceWindow::reload()
{
    QDate cdate;
    cdate.setDate(ui->calendarWidget->yearShown(), ui->calendarWidget->monthShown(), 1);

    QTextCharFormat normalfont;
    QTextCharFormat italicfont;
    QTextCharFormat boldfont;
    //normalfont.setFontFamily("Arial");
    //normalfont.setFontPointSize(12);
    italicfont = normalfont;
    boldfont = normalfont;
    italicfont.setFontItalic(true);;
    boldfont.setFontWeight(QFont::Bold);
    for(int d = 1; d < 32; ++d, cdate = cdate.addDays(1))
    {
        if( dm.HoursReserved(cdate) == 0 )
            ui->calendarWidget->setDateTextFormat(cdate,normalfont);
        else if( dm.HoursUnpaid(cdate) == 0 )
            ui->calendarWidget->setDateTextFormat(cdate,italicfont);
        else
            ui->calendarWidget->setDateTextFormat(cdate,boldfont);
    }

    Entry* entry;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            QDate selDate;
            selDate = ui->calendarWidget->selectedDate();
            entry = dm.GetEntry(selDate,i,j);
            if( entry )
            {
                ui->tableWidget->item(i,j)->setText(entry->firstname + " " + entry->lastname);
                if( entry->payment )
                    ui->tableWidget->item(i,j)->setBackgroundColor(QColor(0,255,0));
                else
                    ui->tableWidget->item(i,j)->setBackgroundColor(QColor(255,0,0));
            }
            else
            {
                ui->tableWidget->item(i,j)->setText("");
                ui->tableWidget->item(i,j)->setBackgroundColor(QColor(255,255,255));
            }
        }
}

void DateChoiceWindow::on_bBlackList_clicked()
{
    BlackListWindow* blw;
    blw = new BlackListWindow();
    blw->exec();
    delete blw;
}

void DateChoiceWindow::on_bDeleteAndBlacklist_clicked()
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
        QDate selDate;
        selDate = ui->calendarWidget->selectedDate();
        entry = dm.GetEntry(selDate,(*i)->row(),(*i)->column());
        if( entry )
        {
            person.firstname = entry->firstname;
            person.lastname = entry->lastname;
            person.phone = entry->phone;
            bl.AddPerson(person);
            dm.RemoveEntry(selDate,(*i)->row(),(*i)->column());
        }
    }
    dm.Save();
    bl.Save();
}

void DateChoiceWindow::on_bDelete_clicked()
{
    if( ui->tableWidget->selectedItems().count() == 0 )
        return;
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
    for(QList<QTableWidgetItem*>::iterator i = list.begin(); i != list.end(); ++i)
    {
        (*i)->setText("");
        (*i)->setBackgroundColor(QColor(255,255,255));
        QDate selDate;
        selDate = ui->calendarWidget->selectedDate();
        dm.RemoveEntry(selDate,(*i)->row(),(*i)->column());
    }
    dm.Save();
}

void DateChoiceWindow::on_calendarWidget_selectionChanged()
{
    int reserved, unpaid;
    QDate qd;

    reload();
    this->setWindowTitle(ui->calendarWidget->selectedDate().toString(Qt::SystemLocaleLongDate));
    qd = ui->calendarWidget->selectedDate();
    reserved = dm.HoursReserved(qd);
    unpaid = dm.HoursUnpaid(qd);
    ui->lReserved->setText("Zarezerwowanych godzin: " + QString::number(reserved,10));
    ui->lUnpaid->setText("w tym " + QString::number(unpaid,10) + " nieopłaconych");
}

void DateChoiceWindow::on_bClear_clicked()
{
    ui->lFirstName->setText("---");
    ui->lLastName->setText("---");
    ui->lPhone->setText("---");
    if(tempEntry)
    {
        delete tempEntry;
        tempEntry = NULL;
    }
}

void DateChoiceWindow::on_bCopy_clicked()
{
    if( ui->tableWidget->selectedItems().count() == 1 )
    {
        QList<QTableWidgetItem*>::iterator lit = ui->tableWidget->selectedItems().begin();
        QDate selDate;
        selDate = ui->calendarWidget->selectedDate();
        Entry* entry = dm.GetEntry(selDate,(*lit)->row(),(*lit)->column());
        if(entry)
        {
            tempEntry = new Entry();
            tempEntry->firstname = entry->firstname;
            tempEntry->lastname = entry->lastname;
            tempEntry->phone = entry->phone;
            tempEntry->notes = "";
            tempEntry->payment = false;
            tempEntry->price = 0;
            tempEntry->recipient = "";
            ui->lFirstName->setText(entry->firstname);
            ui->lLastName->setText(entry->lastname);
            ui->lPhone->setText(entry->phone);
        }
    }
}

void DateChoiceWindow::on_bPaste_clicked()
{
    if(!tempEntry)
        return;
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
    for(QList<QTableWidgetItem*>::iterator i = list.begin(); i != list.end(); ++i)
    {
        tempEntry->receptiontime = QDateTime::currentDateTime();
        tempEntry->date = ui->calendarWidget->selectedDate();
        tempEntry->hour = (*i)->row();
        tempEntry->lane = (*i)->column();
        dm.AddEntry(*tempEntry);
    }
    dm.Save();
    reload();
}

void DateChoiceWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    on_bBook_clicked();
}
