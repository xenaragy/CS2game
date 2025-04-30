#include "home.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Show the Home page first
    Home homePage;
    homePage.show();

    return a.exec();
}
