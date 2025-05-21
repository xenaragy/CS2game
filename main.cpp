#include "home.h"
#include <QApplication>


QList<QGraphicsItem*> scrollableItems;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Home homePage;
    homePage.show();

    return a.exec();
}
