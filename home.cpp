#include "home.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>

Home::Home(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    setupUI();
}

void Home::setupUI()
{
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/backgrounds/homepage.jpeg").scaled(size()));
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();


    QPushButton* portalBtn = new QPushButton(this);
    portalBtn->setIcon(QIcon(":/backgrounds/portal.png"));
    portalBtn->setIconSize(QSize(200, 200));
    portalBtn->setFixedSize(200, 200);
    portalBtn->setFlat(true);

    QLabel* portalText = new QLabel("Level 1", this);
    portalText->setAlignment(Qt::AlignCenter);
    portalText->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    portalText->setGeometry(90, 150, 120, 140);
    portalBtn->move(50, 100);

    connect(portalBtn, &QPushButton::clicked, this, &Home::startLevel1);
}

void Home::startLevel1()
{
    this->hide();
    MainWindow* gameWindow = new MainWindow(this);
    gameWindow->show();
}
