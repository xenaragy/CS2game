#include "home.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QTimer>

Home::Home(QWidget *parent)
    : QWidget(parent), gameWindow(nullptr)
{
    setFixedSize(800, 600);
    setupUI();
}

Home::~Home()
{
    if (gameWindow) {
        gameWindow->disconnect(); // Disconnect all signals
        delete gameWindow;
    }
}

void Home::setupUI()
{
    // Background setup
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/backgrounds/homepage.jpeg").scaled(size()));
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();

    // Level 1 Button
    QPushButton* portalBtn = new QPushButton(this);
    portalBtn->setIcon(QIcon(":/backgrounds/portal.png"));
    portalBtn->setIconSize(QSize(200, 200));
    portalBtn->setFixedSize(200, 200);
    portalBtn->setFlat(true);
    portalBtn->move(50, 100);

    QLabel* portalText = new QLabel("Level 1", this);
    portalText->setAlignment(Qt::AlignCenter);
    portalText->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    portalText->setGeometry(90, 150, 120, 140);

    connect(portalBtn, &QPushButton::clicked, this, &Home::startLevel1);

    // Level 2 Button
    level2Btn = new QPushButton(this);
    level2Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level2Btn->setIconSize(QSize(200, 200));
    level2Btn->setFixedSize(200, 200);
    level2Btn->setFlat(true);
    level2Btn->setGeometry(50, 350, 200, 200);

    QLabel* level2Text = new QLabel("Level 2", this);
    level2Text->setAlignment(Qt::AlignCenter);
    level2Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level2Text->setGeometry(90, 400, 120, 140);

    connect(level2Btn, &QPushButton::clicked, this, &Home::startLevel2);

    // Level 3 Button
    level3Btn = new QPushButton(this);
    level3Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level3Btn->setIconSize(QSize(200, 200));
    level3Btn->setFixedSize(200, 200);
    level3Btn->setFlat(true);
    level3Btn->setGeometry(300, 350, 200, 200);

    QLabel* level3Text = new QLabel("Level 3", this);
    level3Text->setAlignment(Qt::AlignCenter);
    level3Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level3Text->setGeometry(340, 400, 120, 140);

    connect(level3Btn, &QPushButton::clicked, this, &Home::startLevel3);
}

void Home::startLevel(int levelNumber) {
    qDebug() << "Starting Level" << levelNumber;

    // Clean up any existing game window
    if (gameWindow) {
        gameWindow->disconnect(); // Disconnect all signals from window
        delete gameWindow;
        gameWindow = nullptr;
    }

    // Create new game window
    gameWindow = new MainWindow(nullptr);

    // Connect signals in a consistent way
    connect(gameWindow, &MainWindow::backToHome, this, [this]() {
        qDebug() << "Received backToHome signal";
        if (gameWindow) {
            gameWindow->hide();
        }
        this->show();

        // Clean up game window after a delay
        QTimer::singleShot(500, this, [this]() {
            if (gameWindow) {
                gameWindow->disconnect();
                delete gameWindow;
                gameWindow = nullptr;
                qDebug() << "Game window deleted after returning home";
            }
        });
    });

    // Connect level completion signals
    connect(gameWindow, &MainWindow::levelOneCompleted, this, &Home::unlockLevel2);
    connect(gameWindow, &MainWindow::levelTwoCompleted, this, &Home::unlockLevel3);

    // Set up and show the level
    gameWindow->setLevel(levelNumber);
    gameWindow->show();
    this->hide();
}

void Home::startLevel1() {
    startLevel(1);
}

void Home::startLevel2() {
    startLevel(2);
}

void Home::startLevel3() {
    startLevel(3);
}

void Home::unlockLevel2() {
    qDebug() << "Level 2 unlocked";
    level2Btn->show();
}

void Home::unlockLevel3() {
    qDebug() << "Level 3 unlocked";
    level3Btn->show();
}
