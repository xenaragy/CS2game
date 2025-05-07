#include "home.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QDebug>

Home::Home(QWidget *parent)
    : QWidget(parent), gameWindow(nullptr)
{
    setFixedSize(800, 600);
    setupUI();
}

Home::~Home()
{
    if (gameWindow) {
        delete gameWindow;
    }
}

void Home::setupUI()
{
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/backgrounds/homepage.jpeg").scaled(size()));
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();

    // Level 1 Portal Button
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

    // Level 2 Portal Button
    level2Btn = new QPushButton(this);
    level2Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level2Btn->setIconSize(QSize(200, 200));
    level2Btn->setFixedSize(200, 200);
    level2Btn->setFlat(true);
    level2Btn->setGeometry(50, 350, 200, 200);
    // Initially we don't hide Level 2 button as per your current code

    QLabel* level2Text = new QLabel("Level 2", this);
    level2Text->setAlignment(Qt::AlignCenter);
    level2Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level2Text->setGeometry(90, 400, 120, 140);

    connect(level2Btn, &QPushButton::clicked, this, &Home::startLevel2);
}

void Home::startLevel1() {
    this->hide();  // Hide Home screen

    if (!gameWindow) {
        gameWindow = new MainWindow(nullptr);

        // Connect the backToHome signal to show the Home window again
        connect(gameWindow, &MainWindow::backToHome, this, [this]() {
            gameWindow->hide();
            this->show();  // This should make the Home screen visible again
        });

        // Connect level completion signal
        connect(gameWindow, &MainWindow::levelOneCompleted, this, &Home::unlockLevel2);
    }

    gameWindow->setLevel(1);   // Load Level 1
    gameWindow->show();
}

void Home::startLevel2() {
    this->hide();

    if (!gameWindow) {
        gameWindow = new MainWindow(nullptr);
        gameWindow->setAttribute(Qt::WA_DeleteOnClose, false);

        connect(gameWindow, &MainWindow::backToHome, this, [this]() {
            this->show();
            gameWindow->hide();
        });
    }

    gameWindow->setLevel(2);
    gameWindow->show();
}

void Home::unlockLevel2()
{
    qDebug() << "Level 1 completed - Level 2 unlocked";
    level2Btn->show();    // Show Level 2 portal button after Level 1 is completed
}
