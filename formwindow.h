#ifndef FORMWINDOW_H
#define FORMWINDOW_H

#include <QDialog>
#include <QList>
#include <QTableWidgetItem>
#include <QDate>
#include "datamanager.h"

namespace Ui {
class FormWindow;
}

class FormWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormWindow(QWidget *parent, QDate date, QList<QTableWidgetItem*> list);
    ~FormWindow();
    QDate selDate;
    QList<QTableWidgetItem*> selList;
    
private slots:
    void on_bBook_clicked();

    void on_cbPayment_clicked();

private:
    Ui::FormWindow *ui;
};

#endif // FORMWINDOW_H
