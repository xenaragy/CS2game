#include "home.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QTimer>

Home::Home(QWidget *parent)
    : QWidget(parent),
    backgroundLabel(nullptr),
    gameWindow(nullptr),
    level2Btn(nullptr),
    level3Btn(nullptr),
    level4Btn(nullptr),
    level5Btn(nullptr),
    level2Text(nullptr),
    level3Text(nullptr),
    level4Text(nullptr),
    level5Text(nullptr)
{
    setFixedSize(800, 600);
    setupUI();
}

Home::~Home()
{
    if (gameWindow) {
        gameWindow->disconnect(this);
        gameWindow->deleteLater();
        gameWindow = nullptr;
    }
}

void Home::setupUI()
{
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/backgrounds/homepage.jpeg").scaled(size()));
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();
    int buttonWidth = 200;
    int buttonHeight = 200;
    int topRowY = 100;

    // Spacing buttons
    int spacing = (width() - (3 * buttonWidth)) / 4;
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

    // Level 1 Text
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
    level2Btn->hide();

    // Level 2 Text
    level2Text = new QLabel("Level 2", this);
    level2Text->setAlignment(Qt::AlignCenter);
    level2Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level2Text->setGeometry(level2X + 40, topRowY + 80, 120, 30);
    level2Text->hide();

    connect(level2Btn, &QPushButton::clicked, this, &Home::startLevel2);

    // Level 3 Button
    level3Btn = new QPushButton(this);
    level3Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level3Btn->setIconSize(QSize(buttonWidth, buttonHeight));
    level3Btn->setFixedSize(buttonWidth, buttonHeight);
    level3Btn->setFlat(true);
    level3Btn->setGeometry(level3X, topRowY, buttonWidth, buttonHeight);
    level3Btn->hide();

    // Level 3 Text
    level3Text = new QLabel("Level 3", this);
    level3Text->setAlignment(Qt::AlignCenter);
    level3Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level3Text->setGeometry(level3X + 40, topRowY + 80, 120, 30);
    level3Text->hide(); // Initially hidden
    connect(level3Btn, &QPushButton::clicked, this, &Home::startLevel3);

    int centerX = width() / 2;
    int level4X = centerX - 220;
    int level5X = centerX + 20;
    int bottomRowY = 350;

    // Level 4 Button (initially hidden)
    level4Btn = new QPushButton(this);
    level4Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level4Btn->setIconSize(QSize(200, 200));
    level4Btn->setFixedSize(200, 200);
    level4Btn->setFlat(true);
    level4Btn->setGeometry(level4X, bottomRowY, 200, 200);
    level4Btn->hide();

    // Level 4 Text
    level4Text = new QLabel("Level 4", this);
    level4Text->setAlignment(Qt::AlignCenter);
    level4Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level4Text->setGeometry(level4X + 40, bottomRowY + 80, 120, 30);
    level4Text->hide();
    connect(level4Btn, &QPushButton::clicked, this, &Home::startLevel4);

    // Level 5 Button
    level5Btn = new QPushButton(this);
    level5Btn->setIcon(QIcon(":/backgrounds/portal.png"));
    level5Btn->setIconSize(QSize(200, 200));
    level5Btn->setFixedSize(200, 200);
    level5Btn->setFlat(true);
    level5Btn->setGeometry(level5X, bottomRowY, 200, 200);
    level5Btn->hide();

    // Level 5 Text
    level5Text = new QLabel("Level 5", this);
    level5Text->setAlignment(Qt::AlignCenter);
    level5Text->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    level5Text->setGeometry(level5X + 40, bottomRowY + 80, 120, 30);
    level5Text->hide();
    connect(level5Btn, &QPushButton::clicked, this, &Home::startLevel5);
}

void Home::startLevel(int levelNumber) {
    if (gameWindow) {
        gameWindow->disconnect(this);
        gameWindow->deleteLater();
        gameWindow = nullptr;
    }
    gameWindow = new MainWindow(nullptr);

    connect(gameWindow, &MainWindow::backToHome, this, [this]() {
        if (gameWindow) {
            gameWindow->hide();
        }
        this->show();
        QTimer::singleShot(500, this, [this]() {
            if (gameWindow) {
                gameWindow->disconnect();
                gameWindow->deleteLater();
                gameWindow = nullptr;
            }
        });
    }, Qt::QueuedConnection);
    connect(gameWindow, &MainWindow::levelOneCompleted, this, &Home::unlockLevel2, Qt::QueuedConnection);
    connect(gameWindow, &MainWindow::levelTwoCompleted, this, &Home::unlockLevel3, Qt::QueuedConnection);
    connect(gameWindow, &MainWindow::levelThreeCompleted, this, &Home::unlockLevel4, Qt::QueuedConnection);
    connect(gameWindow, &MainWindow::levelFourCompleted, this, &Home::unlockLevel5, Qt::QueuedConnection);
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

void Home::startLevel4() {
    startLevel(4);
}

void Home::startLevel5() {
    startLevel(5);
}

void Home::unlockLevel2() {
    if (!this) {
        return;
    }

    if (level2Btn && level2Text) {
        level2Btn->show();
        level2Text->show();
    }
}

void Home::unlockLevel3() {
    if (!this) {
        return;
    }

    if (level3Btn && level3Text) {
        level3Btn->show();
        level3Text->show();
    }
}

void Home::unlockLevel4() {
    if (!this) {
        return;
    }

    if (level4Btn && level4Text) {
        level4Btn->show();
        level4Text->show();
    }
}

void Home::unlockLevel5() {

    if (!this) {
        return;
    }

    if (level5Btn && level5Text) {
        level5Btn->show();
        level5Text->show();
    }
}
