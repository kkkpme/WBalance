#include "WBalance.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WBalance w;
    w.show();
    return a.exec();
}
