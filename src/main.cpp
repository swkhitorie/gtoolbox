#include "qtb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTB w;
    w.show();

    return a.exec();
}
