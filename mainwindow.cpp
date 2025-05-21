#include "mainwindow.h"
#include "Level.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"
#include "message.h"
#include "obstacle.h"
#include "rewards.h"
#include "enemies.h"
#include "home.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <QPixmap>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QGraphicsView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Add this line to ensure keyboard focus
    this->setFocusPolicy(Qt::StrongFocus);
    setupLevel1();
    setWindowTitle("Desert Adventure Game");
    setFixedSize(800, 600);

    scene = nullptr;
    view = nullptr;
    player = nullptr;
    previousPlayer = nullptr;
    level = nullptr;
    healthBar = nullptr;
    healthOutline = nullptr;
    bg1 = nullptr;
    bg2 = nullptr;

    // Initialize new pointers
    store = nullptr;
    storeButton = nullptr;
    coinIcon = nullptr;
    coinCountLabel = nullptr;
    speedBoostIcon = nullptr;
    shieldIcon = nullptr;
    superAttackIcon = nullptr;

    // Initialize other variables
    levelFinished = false;
}
int MainWindow::globalCoins = 0;
MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called";

    // Delete UI first
    delete ui;

    // Delete other objects safely
    if (scene) {
        delete scene;
        scene = nullptr;
    }

    if (view) {
        // Don't delete view as it's a child of this and will be deleted automatically
        view = nullptr;
    }

    if (player) {
        // Player should be deleted with scene
        player = nullptr;
    }

    if (level) {
        delete level;
        level = nullptr;
    }

    if (store) {
        delete store;
        store = nullptr;
    }

}

void MainWindow::setLevel(int levelNumber) {

    // Clean up previous level if needed
    if (scene) {
        // Instead of deleting immediately, schedule deletion for later
        scene->clear();
        QTimer::singleShot(0, this, [this]() {
            if (scene) {
                delete scene;
                scene = nullptr;
            }
        });
    } else {
        scene = nullptr;
    }

    // Create a fresh scene for the new level
    if (levelNumber == 1) {
        setupLevel1();
    } else if (levelNumber == 2) {
        setupLevel2();
    } else if (levelNumber == 3) {
        setupLevel3();
    } else if (levelNumber == 4) {
        setupLevel4();
    } else if (levelNumber == 5) {
        setupLevel5();
    }

    // Reset level state
    levelFinished = false;

    // Show the window
    //show();
}


void MainWindow::setupLevel1()
{
    if (scene) {
        scene->clear();
        delete scene;
    }
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (view) {
        delete view;
    }

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);
    view->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(view);

    QPixmap bgPixmap(":/backgrounds/desertbackground.jpg");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
    player->setCoins(globalCoins);
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    if (level) {
        delete level;
    }
    level = new Level(1, scene, player);
    level->setupLevel();

    int margin = 10;
    int viewRight = this->width();

    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    healthOutline->setBrush(Qt::NoBrush);
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    healthBar->setPen(Qt::NoPen);
    scene->addItem(healthBar);
    healthOutline->setPos(viewRight - 210, margin + 30);
    healthBar->setPos(viewRight - 210, margin + 30);

    levelText = new QLabel("Level: 1", this);
    levelText->setStyleSheet("color: black; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    waterIcon = new QLabel(this);
    waterIcon->setPixmap(QPixmap(":/Obstacles/waterdroplet.png").scaled(30, 30));
    waterIcon->move(175, 30);
    waterIcon->resize(30, 30);

    coinIcon = new QLabel(this);
    coinIcon->setPixmap(QPixmap(":/Rewards/coin.png").scaled(30, 30));
    coinIcon->move(50, 70);
    coinIcon->resize(30, 30);

    if (!coinCountLabel) {
        coinCountLabel = new QLabel(this);
    }
    coinCountLabel->setText(QString::number(globalCoins));
    coinCountLabel->setStyleSheet("color: gold; font-weight: bold;");
    coinCountLabel->move(85, 70);
    coinCountLabel->resize(100, 30);

    scoreText = new QLabel("0/20", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    levelFinished = false;

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer"); // Give it a name
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    // Create store button
    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    // Create store instance
    store = new Store(player, this);
}

void MainWindow::setupLevel2() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (view) {
        delete view;
    }

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    QPixmap bgPixmap(":/backgrounds/ForrestBackground.png");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
    player->setCoins(globalCoins);
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    if (level) {
        delete level;
    }
    level = new class Level2(scene, player);
    level->setupLevel();

    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

    levelText = new QLabel("Level: 2", this);
    levelText->setStyleSheet("color: black; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    AppleIcon = new QLabel(this);
    AppleIcon->setPixmap(QPixmap(":/Rewards/apple.png").scaled(30, 30));
    AppleIcon->move(175, 30);
    AppleIcon->resize(30, 30);

    coinIcon = new QLabel(this);
    coinIcon->setPixmap(QPixmap(":/Rewards/coin.png").scaled(30, 30));
    coinIcon->move(50, 70);
    coinIcon->resize(30, 30);

    if (!coinCountLabel) {
        coinCountLabel = new QLabel(this);
    }
    coinCountLabel->setText(QString::number(globalCoins));
    coinCountLabel->setStyleSheet("color: gold; font-weight: bold;");
    coinCountLabel->move(85, 70);
    coinCountLabel->resize(100, 30);

    scoreText = new QLabel("0/20", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer"); // Give it a name
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    // Create store button
    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    // Create store instance
    if (player) {
        store = new Store(player, this);
    }

    qDebug() << "Level 2 setup complete!";
}

void MainWindow::setupLevel3() {

    // If scene already exists, clean it up
    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (view) {
        delete view;
    }

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    QPixmap bgPixmap(":/backgrounds/snowbackground.png");
    bgPixmap = bgPixmap.scaled(800, 680);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
    player->setCoins(globalCoins);
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    // Clean up old level instance if it exists
    if (level) {
        delete level;
    }
    level = new class Level3(scene, player);
    level->setupLevel();

    // Health bar UI
    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

    // Update level and score display
    levelText = new QLabel("Level: 3", this);
    levelText->setStyleSheet("color: black; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    FlakeIcon = new QLabel(this);
    FlakeIcon->setPixmap(QPixmap(":/Rewards/snowflake.png").scaled(30, 30));
    FlakeIcon->move(175, 30);
    FlakeIcon->resize(30, 30);

    scoreText = new QLabel("0/25", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    coinIcon = new QLabel(this);
    coinIcon->setPixmap(QPixmap(":/Rewards/coin.png").scaled(30, 30));
    coinIcon->move(50, 70);
    coinIcon->resize(30, 30);

    if (!coinCountLabel) {
        coinCountLabel = new QLabel(this);
    }
    coinCountLabel->setText(QString::number(globalCoins));
    coinCountLabel->setStyleSheet("color: gold; font-weight: bold;");
    coinCountLabel->move(85, 70);
    coinCountLabel->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer"); // Give it a name
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    // Create store button
    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    // Create store instance
    store = new Store(player, this);

    qDebug() << "Level 3 setup complete!";
}

void MainWindow::setupLevel4() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (view) {
        delete view;
    }

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    // Create timer label
    levelTimerLabel = new QLabel("Time: 01:30", this);
    levelTimerLabel->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
    levelTimerLabel->move(650, 10);
    levelTimerLabel->resize(150, 30);
    levelTimerLabel->show();

    QPixmap bgPixmap(":/backgrounds/spacebackground.jpg");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
    player->setCoins(globalCoins);
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    if (level) {
        delete level;
    }
    level = new class Level4(scene, player);
    level->setupLevel();

    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

    levelText = new QLabel("Level: 4", this);
    levelText->setStyleSheet("color: white; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    StarIcon = new QLabel(this);
    StarIcon->setPixmap(QPixmap(":/Rewards/star.png").scaled(30, 30));
    StarIcon->move(175, 30);
    StarIcon->resize(30, 30);

    scoreText = new QLabel("0/30", this);
    scoreText->setStyleSheet("color: white; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    coinIcon = new QLabel(this);
    coinIcon->setPixmap(QPixmap(":/Rewards/coin.png").scaled(30, 30));
    coinIcon->move(50, 70);
    coinIcon->resize(30, 30);

    if (!coinCountLabel) {
        coinCountLabel = new QLabel(this);
    }
    coinCountLabel->setText(QString::number(globalCoins));
    coinCountLabel->setStyleSheet("color: gold; font-weight: bold;");
    coinCountLabel->move(85, 70);
    coinCountLabel->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetStars();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer"); // Give it a name
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    // Create store button
    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    // Create store instance
    store = new Store(player, this);

    qDebug() << "Level 4 setup complete!";
}

void MainWindow::setupLevel5() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (view) {
        delete view;
    }

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view);

    QPixmap bgPixmap(":/backgrounds/cavebackground.jpg");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
    player->setCoins(globalCoins);
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    if (level) {
        delete level;
    }
    level = new class Level5(scene, player);
    level->setupLevel();

    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

    levelText = new QLabel("Level: 5", this);
    levelText->setStyleSheet("color: white; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    DiamondIcon = new QLabel(this);
    DiamondIcon->setPixmap(QPixmap(":/Rewards/diamond.png").scaled(30, 30));
    DiamondIcon->move(175, 30);
    DiamondIcon->resize(30, 30);

    scoreText = new QLabel("0/40", this);
    scoreText->setStyleSheet("color: white; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    coinIcon = new QLabel(this);
    coinIcon->setPixmap(QPixmap(":/Rewards/coin.png").scaled(30, 30));
    coinIcon->move(50, 70);
    coinIcon->resize(30, 30);

    if (!coinCountLabel) {
        coinCountLabel = new QLabel(this);
    }
    coinCountLabel->setText(QString::number(globalCoins));
    coinCountLabel->setStyleSheet("color: gold; font-weight: bold;");
    coinCountLabel->move(85, 70);
    coinCountLabel->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetStars();
    player->resetDiamonds();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer"); // Give it a name
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    // Create store button
    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    // Create store instance
    store = new Store(player, this);

}
void MainWindow::updateHealthBar() {
    int hp = player->getHealth();
    healthBar->setRect(0, 0, 2 * hp, 20);
    for (QGraphicsItem* item : scene->items()) {
        Obstacle* obs = dynamic_cast<Obstacle*>(item);
        if (obs) {
            obs->handleCollision(player);
        }
    }
    if (player->getHealth() <= 0) {
        showGameOver();
        level->resetLevel();
        return;
    }
}


void MainWindow::showGameOver() {
    // First, prevent multiple game over screens by adding a flag
    static bool gameOverInProgress = false;

    // If a game over sequence is already in progress, exit
    if (gameOverInProgress) {
        return;
    }

    // Set the flag to prevent multiple calls
    gameOverInProgress = true;

    // Stop all timers in the level to prevent further callbacks
    if (level && level->getLevelNumber() == 4) {
        class Level4* level4 = static_cast<class Level4*>(level);
        if (level4) {
            // Stop all timers explicitly
            level4->stopAllTimers();
        }
    }

    // Create game over text
    QGraphicsTextItem* gameOver = scene->addText("GAME OVER");
    gameOver->setDefaultTextColor(Qt::red);
    QFont font = gameOver->font();
    font.setPointSize(30);
    font.setBold(true);
    gameOver->setFont(font);
    gameOver->setPos(width()/2 - 100, height()/2 - 50);

    // Single-shot timer to handle reset
    QTimer* resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);

    connect(resetTimer, &QTimer::timeout, [this, gameOver, resetTimer]() {
        // Remove game over text
        if (scene && gameOver) {
            scene->removeItem(gameOver);
            delete gameOver;
        }

        // Reset level only once
        if (level) {
            level->resetLevel();

            // Reset timer display for level 4
            if (level->getLevelNumber() == 4 && levelTimerLabel) {
                updateLevelTimer(90);
            }
        }

        // Clear the game over flag
        gameOverInProgress = false;

        // Clean up the timer
        resetTimer->deleteLater();
    });

    // Start the timer
    resetTimer->start(2000);
}


void MainWindow::resetAfterGameOver() {
    // First reset player state
    player->resetPlayer();
    resetGameState();

    // Then reset level
    level->resetLevel();
}

void MainWindow::updateScore() {
    if (levelFinished) return;

    int collected = 0;
    int requiredAmount = 20;

    // Determine what collectibles to count based on level
    if (level->getLevelNumber() == 1) {
        collected = player->getCollectedDroplets();
    } else if (level->getLevelNumber() == 2) {
        collected = player->getCollectedApples();
    } else if (level->getLevelNumber() == 3) {
        collected = player->getCollectedSnowflakes();
        requiredAmount = 25;
    } else if (level->getLevelNumber() == 4){
        collected = player->getCollectedStars();
        requiredAmount = 30;
    } else if (level->getLevelNumber() == 5){
        collected = player->getCollectedDiamonds();
        requiredAmount = 40;
    }

    scoreText->setText(QString::number(collected) + "/" + QString::number(requiredAmount));

    if (collected >= requiredAmount) {
        qDebug() << "Level completed with" << collected << "items!";
        levelFinished = true;

        // Show appropriate completion message
        if (level->getLevelNumber() == 1) {
            Message* endMessage = Message::createLevelOneCompleteMessage();
            endMessage->showMessage(scene, 300, 300);
            emit levelOneCompleted();  // Signal to unlock level 2
        } else if (level->getLevelNumber() == 2) {
            Message* endMessage = Message::createLevelTwoCompleteMessage();
            endMessage->showMessage(scene, 300, 300);
            emit levelTwoCompleted();  // Signal to unlock level 3
        } else if (level->getLevelNumber() == 3) {
            Message* endMessage = Message::createLevelThreeCompleteMessage();
            endMessage->showMessage(scene, 300, 300);
            emit levelThreeCompleted();  // Signal to unlock level 4
        } else if (level->getLevelNumber() == 4) {
            Message* endMessage = Message::createLevelFourCompleteMessage();
            endMessage->showMessage(scene, 300, 300);
            emit levelFourCompleted();  // Signal for game completion
        } else if (level->getLevelNumber() == 5) {
            Message* endMessage = Message::createLevelFiveCompleteMessage();
            endMessage->showMessage(scene, 300, 300);
            emit levelFiveCompleted();  // Signal for game completion
        }

        // Always return to home screen after completing any level
        QTimer::singleShot(4000, this, [this]() {
            qDebug() << "Emitting backToHome signal after level completion";
            emit backToHome();
        });
    }
}
void MainWindow::updateGame()
{
    if (gamePaused) {
        return;
    }

    // Add static flag to prevent re-entry
    static bool updatingGame = false;

    // If already updating, exit to prevent recursive calls
    if (updatingGame) {
        return;
    }

    // Set the flag
    updatingGame = true;

    if (player->y() > 600 || player->getHealth() <= 0) {
        showGameOver();  // Show game over message
        player->setPosition(100, 400);  // Reset player position
        updatingGame = false;  // Reset the flag
        return;  // Skip the rest of updateGame
    }
     if (!player->hasFocus() && !player->isFrozenBySpaceship()) {
        player->setFocus();
    }

     // Handle player attacks
     handlePlayerAttacks();

     // Check if screen scrolling is allowed based on remaining enemies
     bool canScroll = true;

     // For level 3 and 4, check if enemies are still alive
     if (level && (level->getLevelNumber() == 3 || level->getLevelNumber() == 4 || level->getLevelNumber() == 5)) {
         // Get player's current x position to determine which section of the level they're in
         int playerX = player->x();

         // Check for enemies in the player's current view section
         // We divide the level into sections (e.g., 0-800, 800-1600, etc.)
         int sectionStart = (playerX / 800) * 800;
         int sectionEnd = sectionStart + 800;

         // Count enemies in this section
         int enemiesInSection = 0;

         // Count enemies in this section
         for (QGraphicsItem* item : scene->items()) {
             if (!item) continue;

             // Check if item is within the current section
             if (item->x() >= sectionStart && item->x() < sectionEnd) {
                 if (level->getLevelNumber() == 3) {
                     Penguin* penguin = dynamic_cast<Penguin*>(item);
                     if (penguin && penguin->isAlive()) {
                         enemiesInSection++;
                     }
                 } else if (level->getLevelNumber() == 4) {
                     Alien* alien = dynamic_cast<Alien*>(item);
                     if (alien && alien->isAlive()) {
                         enemiesInSection++;
                     }
                 } else if (level->getLevelNumber() == 5) {
                     CaveCreature* cavemons = dynamic_cast<CaveCreature*>(item);
                     if (cavemons && cavemons->isAlive()) {
                         enemiesInSection++;
                     }
                 }
             }
         }

         // If there are still enemies in this section, don't allow scrolling
         if (enemiesInSection > 0 && player->x() >= 600) {
             canScroll = false;

             // Show hint message if player tries to move right
             if (player->isMovingRight()) {
                 // Use a static counter to limit how often we show messages
                 static int messageCounter = 0;
                 if (messageCounter++ % 100 == 0) { // Show message every 100 frames
                     if (level->getLevelNumber() == 3) {
                         Message* hintmessage = Message::HintMessagelevel3();
                         hintmessage->showMessage(scene, 300, 300);
                     } else if (level->getLevelNumber() == 4) {
                         Message* hintmessage = Message::HintMessagelevel4();
                         hintmessage->showMessage(scene, 300, 300);
                     } else if (level->getLevelNumber() == 5) {
                         Message* hintmessage = Message::HintMessagelevel5();
                         hintmessage->showMessage(scene, 300, 300);
                     }
                 }
             }
         } else if (player->x() >= 600) {
             // All enemies defeated, allowing scroll
             canScroll = true;
         }
     }

    const int scrollSpeed = 3;
    if (player && player->x() >= 600 && player->isMovingRight() && canScroll) {
        bg1->moveBy(-scrollSpeed, 0);
        bg2->moveBy(-scrollSpeed, 0);
        if (bg1->x() + 800 <= 0) {
            bg1->setX(bg2->x() + 800);
        }
        if (bg2->x() + 800 <= 0) {
            bg2->setX(bg1->x() + 800);
        }
        for (QGraphicsItem* item : scene->items()) {
            if (!item) continue;  // Safety check
            Obstacle* obs = dynamic_cast<Obstacle*>(item);
            if (obs) {
                obs->handleCollision(player);
            }
        }
        updateHealthBar();
        for (QGraphicsItem* item : scene->items()) {
            if (!item) continue;  // Safety check
            if (item == player || item == bg1 || item == bg2 ||
                item == healthOutline || item == healthBar) continue;
            item->moveBy(-scrollSpeed, 0);
             // Check if this item is near the edge of the screen
            if (item->x() + item->boundingRect().width() < 0) {
                item->setX(scene->sceneRect().width());
            }
        }
    }
    //check coin collisions (all levels)
    for (QGraphicsItem* item : scene->items()) {
        Coin* coin = dynamic_cast<Coin*>(item);
        if (coin) {
            coin->checkCollision(player);
        }
    }
    // Check collectibles based on level
    if (level) {  // Safety check
        int levelNum = level->getLevelNumber();

        if (levelNum == 1) {
            for (QGraphicsItem* item : scene->items()) {
                if (!item) continue;  // Safety check

                WaterDroplet* droplet = dynamic_cast<WaterDroplet*>(item);
                if (droplet) {
                    droplet->checkCollision(player);
                }
            }
        } else if (levelNum == 2) {
            for (QGraphicsItem* item : scene->items()) {
                if (!item) continue;  // Safety check

                Apple* apple = dynamic_cast<Apple*>(item);
                if (apple) {
                    apple->checkCollision(player);
                }
            }
        } else if (levelNum == 3) {
            for (QGraphicsItem* item : scene->items()) {
                if (!item) continue;  // Safety check

                Snowflake* flake = dynamic_cast<Snowflake*>(item);
                if (flake) {
                    flake->checkCollision(player);
                }
            }
        } else if (levelNum == 4) {
            if (dynamic_cast<class Level4*>(level)) {
                dynamic_cast<class Level4*>(level)->checkAsteroidAttack();
            }
            for (QGraphicsItem* item : scene->items()) {
                if (!item) continue;  // Safety check

                Star* star = dynamic_cast<Star*>(item);
                if (star) {
                    star->checkCollision(player);
                }
            }
        } else if (levelNum == 5) {
            if (dynamic_cast<class Level5*>(level)) {
                dynamic_cast<class Level5*>(level)->checkTrollAttack();
                dynamic_cast<class Level5*>(level)->checkBatAttack();

            }

            for (QGraphicsItem* item : scene->items()) {
                if (!item) continue;  // Safety check

                Diamond* diamond = dynamic_cast<Diamond*>(item);
                if (diamond) {
                    diamond->checkCollision(player);
                }
            }
        }
    }

    int margin = 10;
    int viewRight = this->width();
    healthOutline->setPos(viewRight - 230, margin + 30);
    healthBar->setPos(viewRight - 230, margin + 30);
    updateHealthBar();
    updateScore();
    // Sync global coins with player coins
    globalCoins = player->getCoins();
    // Update coin count display
    if (coinCountLabel) {
        coinCountLabel->setText(QString::number(globalCoins));
    }
    updatingGame = false;
}

void MainWindow::handlePlayerAttacks() {
    if (!player->isAttacking()) {
        return; // Exit if player is not attacking
    }

    for (QGraphicsItem* item : scene->items()) {
        // Level 2 - Tiger
        if (level->getLevelNumber() == 2) {
            Tiger* tiger = dynamic_cast<Tiger*>(item);
            if (tiger && tiger->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = tiger->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    tiger->takeDamage(25);
                }
            }
        }
        // Level 3 - Polar Bear and Penguin
        else if (level->getLevelNumber() == 3) {
            // Check for polar bear
            PolarBear* bear = dynamic_cast<PolarBear*>(item);
            if (bear && bear->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = bear->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    bear->takeDamage(25);
                }
            }

            // Check for penguin
            Penguin* penguin = dynamic_cast<Penguin*>(item);
            if (penguin && penguin->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = penguin->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    bool killed = penguin->takeDamage(25);
                    if (killed) {
                        player->heal(10);
                        updateHealthBar();
                        Message* killedinguin = Message::killedPinguinBonus();
                        killedinguin->showMessage(scene, 300, 300);
                    }
                }
            }
        }
        // Level 4 - Alien
        else if (level->getLevelNumber() == 4) {
            Alien* alien = dynamic_cast<Alien*>(item);
            if (alien && alien->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = alien->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    bool killed = alien->takeDamage(25);
                    if (killed) {
                        player->heal(10);
                        updateHealthBar();
                        Message* killedAliend = Message::killedAlienBonus();
                        killedAliend->showMessage(scene, 300, 300);
                    }
                }
            }
        }
        //Level 5 - Troll
        else if (level->getLevelNumber() == 5) {
            Troll* troll = dynamic_cast<Troll*>(item);
            if (troll && troll->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = troll->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    troll->takeDamage(40);
                }
            }


        // CaveCreature
            CaveCreature* creature = dynamic_cast<CaveCreature*>(item);
            if (creature && creature->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = creature->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    bool killed = creature->takeDamage(25);
                    if (killed) {
                        player->heal(10);
                        updateHealthBar();
                        Message* kileddCreature = Message::killedCaveCreatureBonus();
                        kileddCreature->showMessage(scene, 300, 300);
                    }
                }
            }
        }

        // Level 5 - Caveman
        Caveman* caveman = dynamic_cast<Caveman*>(item);
        if (caveman && caveman->isAlive()) {
            QPointF playerPos = player->pos();
            QPointF enemyPos = caveman->pos();
            qreal distance = QLineF(playerPos, enemyPos).length();
            if (distance < 150) {
                bool killed = caveman->takeDamage(25);
                if (killed) {
                    player->heal(10);
                    updateHealthBar();
                    caveman->setPixmap(QPixmap(":/Enemies/deadCaveMan.png").scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    Message* killedcaveman = Message::killedCaveCreatureBonus();
                    killedcaveman->showMessage(scene, 300, 300);
                }
            }
        }

    }
}
void MainWindow::goToHome() {
    if (!levelFinished) {
        return;
    }

    QTimer::singleShot(3000, this, [=]() {

        // Hide all game elements
        player->setVisible(false);
        for (QGraphicsItem* item : scene->items()) {
            if (item != healthOutline && item != healthBar && item != bg1 && item != bg2 && item != player) {
                item->setVisible(false);
            }
        }

        // Hide UI elements
        levelText->hide();
        scoreText->hide();
        waterIcon->hide();

        // Emit the signal to go back to Home
        emit backToHome();
    });
    if (levelTimerLabel) {
        levelTimerLabel->hide();
    }
}



void MainWindow::resetGameState() {
    levelFinished = false;
    player->resetDroplets(); // Reset all types
    player->resetApples();
    player->resetSnowflakes();
    player -> resetStars();
    player -> resetDiamonds();
    level->resetLevel();
}


void MainWindow::nextLevel() {
    int currentLevel = level->getLevelNumber();
    if (currentLevel == 1) {
        setupLevel2();
    } else if (currentLevel == 2) {
        setupLevel3();
    } else if (currentLevel ==3){
        setupLevel4();
    }else if (currentLevel ==4){
        setupLevel5();
    }
}



void MainWindow::switchToNextLevel() {
    // Don't create and close a new Home window
    // Just switch to Level 2 directly

    int currentLevel = level->getLevelNumber();  // Get the current level

    // Save coin count before destroying level/player
    globalCoins = player->getCoins();
    qDebug() << "Saved coins before level switch:" << globalCoins;

    if (level) {
        delete level;
        level = nullptr;
    }

    if (currentLevel == 1) {
        setupLevel2();
        // Manually set player coins again after setup
        player->setCoins(globalCoins);
        qDebug() << "After setup level 2, player coins:" << player->getCoins();
        Message* level2StartMessage = Message::createLevelTwoStartMessage();
        level2StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 2) {
        setupLevel3();
        // Manually set player coins again after setup
        player->setCoins(globalCoins);
        qDebug() << "After setup level 2, player coins:" << player->getCoins();

        Message* level3StartMessage = Message::createLevelThreeStartMessage();
        level3StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 3) {
        setupLevel4();
        // Manually set player coins again after setup
        player->setCoins(globalCoins);
        qDebug() << "After setup level 2, player coins:" << player->getCoins();

        Message* level4StartMessage = Message::createLevelFourStartMessage();
        level4StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 4) {
        setupLevel5();
        // Manually set player coins again after setup
        player->setCoins(globalCoins);
        qDebug() << "After setup level 2, player coins:" << player->getCoins();

        Message* level5StartMessage = Message::createLevelFiveStartMessage();
        level5StartMessage->showMessage(scene, 300, 300);
    }
}

void MainWindow::handleLevelTwoComplete() {
    // After a delay, switch to Level 3
    QTimer::singleShot(4000, this, [this]() {
        switchToNextLevel();  // Move to Level 3
    });
}

void MainWindow::handleLevelThreeComplete() {
    // After a delay, switch to Level 4
    QTimer::singleShot(4000, this, [this]() {
        switchToNextLevel();  // Move to Level 4
    });
}
void MainWindow::updateLevelTimer(int seconds) {
    if (levelTimerLabel) {
        int minutes = seconds / 60;
        int secs = seconds % 60;
        levelTimerLabel->setText(QString("Time: %1:%2")
                                     .arg(minutes, 2, 10, QChar('0'))
                                     .arg(secs, 2, 10, QChar('0')));
    }
}
// Add this function to your MainWindow.cpp file
void MainWindow::openStore() {
    // Make sure we have a valid store and player
    if (!store || !player) {
        return;
    }

    // Update the store display with current coin count
    store->updateDisplay();

    // Show the store as a modal dialog
    store->exec();

    // When the store closes, ensure player has focus for controls
    if (player) {
        player->setFocus();
    }
}
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Pass keyboard events to the player for movement control
    if (player) {
        player->keyPressEvent(event);
    }

    // Handle global keyboard shortcuts
    switch (event->key()) {
    case Qt::Key_R:
        // Restart the current level
        if (level) {
            level->resetLevel();
            resetGameState();
        }
        break;

    case Qt::Key_E:
        // Exit to home screen
        gamePaused = false; // Make sure game is not paused when returning to home

        // Use a single-shot timer to ensure clean return to home
        QTimer::singleShot(100, this, [this]() {
            emit backToHome();
        });
        break;

    case Qt::Key_P:
        // Toggle pause state
        if (gamePaused) {
            resumeGame();
        } else {
            pauseGame();
        }
        break;

    default:
        // Let the base class handle other keys
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    // Pass keyboard events to the player for movement control
    if (player) {
        player->keyReleaseEvent(event);
    }

    // Let the base class handle other keys
    QMainWindow::keyReleaseEvent(event);
}

// Update the pauseGame method:
void MainWindow::pauseGame()
{
    if (gamePaused) return; // Already paused
    gamePaused = true;

    // Stop all timers and animations
    for (QTimer* timer : findChildren<QTimer*>()) {
        timer->stop();
    }

    // Notify player about pause state
    if (player) {
        player->setPaused(true);
    }

    // Display pause message
    pauseText = scene->addText("GAME PAUSED\nPress P to resume");
    pauseText->setDefaultTextColor(Qt::white);
    QFont font = pauseText->font();
    font.setPointSize(24);
    font.setBold(true);
    pauseText->setFont(font);

    // Center the text
    QRectF textRect = pauseText->boundingRect();
    pauseText->setPos((800 - textRect.width()) / 2,
                      (600 - textRect.height()) / 2);

    // Make text stand out by adding a semi-transparent background
    QGraphicsRectItem* background = new QGraphicsRectItem(pauseText->boundingRect());
    background->setBrush(QBrush(QColor(0, 0, 0, 150)));
    background->setPos(pauseText->pos());
    background->setData(0, "pauseBackground"); // Tag for identification
    background->setZValue(999); // Just below text
    scene->addItem(background);
    pauseText->setZValue(1000); // Ensure text is on top
}

// Update the resumeGame method:
void MainWindow::resumeGame()
{
    if (!gamePaused) return; // Not paused
    gamePaused = false;

    // Notify player about pause state
    if (player) {
        player->setPaused(false);
    }

    // Remove pause message
    if (pauseText) {
        scene->removeItem(pauseText);
        delete pauseText;
        pauseText = nullptr;

        // Find and remove the background by its data tag
        for (QGraphicsItem* item : scene->items()) {
            if (item && item->data(0).toString() == "pauseBackground") {
                scene->removeItem(item);
                delete item;
                break;
            }
        }
    }

    // Instead of creating a new timer, restart existing timers
    QTimer* gameTimer = findChild<QTimer*>("gameTimer");
    if (!gameTimer) {
        // Create a new timer only if one doesn't exist
        gameTimer = new QTimer(this);
        gameTimer->setObjectName("gameTimer"); // Give it a name for future reference
        connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    }
    gameTimer->start(16);
}
