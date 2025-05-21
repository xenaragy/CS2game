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

    // Calculate positions for top row (levels 1, 2, 3)
    int buttonWidth = 200;
    int buttonHeight = 200;
    int topRowY = 100;

    // Space buttons evenly across the width
    int spacing = (width() - (3 * buttonWidth)) / 4; // Equal spacing between and on edges
    int level1X = spacing;
    int level2X = 2 * spacing + buttonWidth;
    int level3X = 3 * spacing + 2 * buttonWidth;

    // Level 1 Button
    QPushButton* portalBtn = new QPushButton(this);
    portalBtn->setIcon(QIcon(":/backgrounds/portal.png"));
    portalBtn->setIconSize(QSize(buttonWidth, buttonHeight));
    portalBtn->setFixedSize(buttonWidth, buttonHeight);
    portalBtn->setFlat(true);
    portalBtn->setGeometry(level1X, topRowY, buttonWidth, buttonHeight);

    // Level 1 Text - positioned over the button
    QLabel* portalText = new QLabel("Level 1", this);
    portalText->setAlignment(Qt::AlignCenter);
    portalText->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    portalText->setGeometry(level1X + 40, topRowY + 80, 120, 30);

    connect(portalBtn, &QPushButton::clicked, this, &Home::startLevel1);

    // Level 2 Button
    level2Btn = new QPushButton(this);
    level2Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level2Btn->setIconSize(QSize(buttonWidth, buttonHeight));
    level2Btn->setFixedSize(buttonWidth, buttonHeight);
    level2Btn->setFlat(true);
    level2Btn->setGeometry(level2X, topRowY, buttonWidth, buttonHeight);

    // Level 2 Text - positioned over the button
    QLabel* level2Text = new QLabel("Level 2", this);
    level2Text->setAlignment(Qt::AlignCenter);
    level2Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level2Text->setGeometry(level2X + 40, topRowY + 80, 120, 30);

    connect(level2Btn, &QPushButton::clicked, this, &Home::startLevel2);

    // Level 3 Button
    level3Btn = new QPushButton(this);
    level3Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level3Btn->setIconSize(QSize(buttonWidth, buttonHeight));
    level3Btn->setFixedSize(buttonWidth, buttonHeight);
    level3Btn->setFlat(true);
    level3Btn->setGeometry(level3X, topRowY, buttonWidth, buttonHeight);

    // Level 3 Text - positioned over the button
    QLabel* level3Text = new QLabel("Level 3", this);
    level3Text->setAlignment(Qt::AlignCenter);
    level3Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level3Text->setGeometry(level3X + 40, topRowY + 80, 120, 30);

    connect(level3Btn, &QPushButton::clicked, this, &Home::startLevel3);

    // Calculate center positions for Level 4 and 5 (side by side)
    int centerX = width() / 2;  // Center of screen (400px)
    int level4X = centerX - 220; // 180px (adjusted left from center)
    int level5X = centerX + 20;  // 420px (adjusted right from center)
    int bottomRowY = 350;        // Y position for bottom row

    // Level 4 Button
    level4Btn = new QPushButton(this);
    level4Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level4Btn->setIconSize(QSize(200, 200));
    level4Btn->setFixedSize(200, 200);
    level4Btn->setFlat(true);
    level4Btn->setGeometry(level4X, bottomRowY, 200, 200);

    // Level 4 Text - positioned over the button
    QLabel* level4Text = new QLabel("Level 4", this);
    level4Text->setAlignment(Qt::AlignCenter);
    level4Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level4Text->setGeometry(level4X + 40, bottomRowY + 80, 120, 30);

    connect(level4Btn, &QPushButton::clicked, this, &Home::startLevel4);

    // Level 5 Button
    level5Btn = new QPushButton(this);
    level5Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level5Btn->setIconSize(QSize(200, 200));
    level5Btn->setFixedSize(200, 200);
    level5Btn->setFlat(true);
    level5Btn->setGeometry(level5X, bottomRowY, 200, 200);

    // Level 5 Text - positioned over the button
    QLabel* level5Text = new QLabel("Level 5", this);
    level5Text->setAlignment(Qt::AlignCenter);
    level5Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level5Text->setGeometry(level5X + 40, bottomRowY + 80, 120, 30);

    connect(level5Btn, &QPushButton::clicked, this, &Home::startLevel5);
}

void Home::startLevel(int levelNumber) {
    qDebug() << "Starting Level" << levelNumber;

    // Clean up any existing game window
    if (gameWindow) {
        gameWindow->disconnect(); // Disconnect all signals
        delete gameWindow;
        gameWindow = nullptr;
    }

    // Create new game window
    gameWindow = new MainWindow(this);

    // Connect signal to return to home
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

    // Connect all level completion signals to unlock next levels
    connect(gameWindow, &MainWindow::levelOneCompleted, this, &Home::unlockLevel2);
    connect(gameWindow, &MainWindow::levelTwoCompleted, this, &Home::unlockLevel3);
    connect(gameWindow, &MainWindow::levelThreeCompleted, this, &Home::unlockLevel4);
    connect(gameWindow, &MainWindow::levelFourCompleted, this, &Home::unlockLevel5);


    this->hide();
    gameWindow->setLevel(levelNumber);
    gameWindow->show();
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

void Home::startLevel4() {
    startLevel(4);
}
void Home::startLevel5() {
    startLevel(5);
}

void Home::unlockLevel2() {
    level2Btn->show();
}

void Home::unlockLevel3() {
    level3Btn->show();
}

void Home::unlockLevel4() {
    level4Btn->show();
}
void Home::unlockLevel5() {
    level5Btn->show();
}
