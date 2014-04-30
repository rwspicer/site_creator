#include "site_creator.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    site_creator w;
    w.setWindowTitle("Site Creator");

    w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.show();
    return a.exec();
}
