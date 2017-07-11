#ifndef DATECHOICEWINDOW_H
#define DATECHOICEWINDOW_H

#include <QMainWindow>
#include "lanereservationwindow.h"
#include "blacklistwindow.h"

namespace Ui {
class DateChoiceWindow;
}

class DateChoiceWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DateChoiceWindow(QWidget *parent = 0);
    ~DateChoiceWindow();
    
private slots:
    void on_bBook_clicked();

    void on_bBlackList_clicked();

    void on_bDeleteAndBlacklist_clicked();

    void on_bDelete_clicked();

    void on_calendarWidget_selectionChanged();

    void on_bClear_clicked();

    void on_bCopy_clicked();

    void on_bPaste_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    void reload();
    Ui::DateChoiceWindow *ui;
    Entry* tempEntry;
};

#endif // DATECHOICEWINDOW_H
