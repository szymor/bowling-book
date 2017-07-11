#ifndef LANERESERVATIONWINDOW_H
#define LANERESERVATIONWINDOW_H

#include <QDialog>
#include <QDate>
#include "formwindow.h"

namespace Ui {
class LaneReservationWindow;
}

class LaneReservationWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit LaneReservationWindow(QWidget *parent, QDate date);
    ~LaneReservationWindow();
    QDate selDate;
    
private slots:
    void on_bBook_clicked();

    void on_bDelete_clicked();

    void on_bDeleteAndBlacklist_clicked();

private:
    void reload();
    Ui::LaneReservationWindow *ui;
};

#endif // LANERESERVATIONWINDOW_H
