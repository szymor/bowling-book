#include "datechoicewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DateChoiceWindow w;
    w.show();
    
    return a.exec();
}
