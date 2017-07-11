#ifndef BLACKLISTWINDOW_H
#define BLACKLISTWINDOW_H

#include <QDialog>
#include "datamanager.h"

namespace Ui {
class BlackListWindow;
}

class BlackListWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit BlackListWindow(QWidget *parent = 0);
    ~BlackListWindow();

private slots:
    void on_bDelete_clicked();

private:
    void reload();
    void clear_items();
    Ui::BlackListWindow *ui;
};

#endif // BLACKLISTWINDOW_H
