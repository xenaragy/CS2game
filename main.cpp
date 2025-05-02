#include "home.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Home homePage;
    homePage.show();

    return a.exec();
}
