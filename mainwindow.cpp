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
    store = nullptr;
    storeButton = nullptr;
    coinIcon = nullptr;
    coinCountLabel = nullptr;
    speedBoostIcon = nullptr;
    shieldIcon = nullptr;
    superAttackIcon = nullptr;
    levelFinished = false;
}
int MainWindow::globalCoins = 0;
MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called";
    // Stop all timers first to prevent callbacks during destruction
    QList<QTimer*> allTimers = findChildren<QTimer*>();
    for (QTimer* timer : allTimers) {
        if (timer) {
            timer->stop();
            timer->disconnect();
        }
    }
    // Clean up level first (it might have running timers)
    if (level) {
        // If it's Level4, stop all its timers
        if (level->getLevelNumber() == 4) {
            class Level4* level4 = dynamic_cast<class Level4*>(level);
            if (level4) {
                level4->stopAllTimers();
            }
        }
        delete level;
        level = nullptr;
    }
    // Delete store safely
    if (store) {
        delete store;
        store = nullptr;
    }
    // Clear scene before deleting it
    if (scene) {
        scene->clear();
        delete scene;
        scene = nullptr;
    }
    // Delete UI last
    if (ui) {
        delete ui;
        ui = nullptr;
    }
    // Set other pointers to null for safety
    view = nullptr;
    player = nullptr;
    previousPlayer = nullptr;
    healthBar = nullptr;
    healthOutline = nullptr;
    bg1 = nullptr;
    bg2 = nullptr;
    storeButton = nullptr;
    coinIcon = nullptr;
    coinCountLabel = nullptr;
    speedBoostIcon = nullptr;
    shieldIcon = nullptr;
    superAttackIcon = nullptr;
}

void MainWindow::setLevel(int levelNumber) {

    if (scene) {
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

    levelFinished = false;
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
    timer->setObjectName("gameTimer"); // updating game timer
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);
    //control label
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
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer");
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);


    if (player) {
        store = new Store(player, this);
    }
}

void MainWindow::setupLevel3() {

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

    if (level) {
        delete level;
    }
    level = new class Level3(scene, player);
    level->setupLevel();

    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

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

    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer");
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);
    store = new Store(player, this);

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

    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetStars();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer");
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);

    store = new Store(player, this);
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

    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetStars();
    player->resetDiamonds();
    player->setHealth(100);

    QTimer* timer = new QTimer(this);
    timer->setObjectName("gameTimer");
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    QLabel* controlsLabel = new QLabel("Controls: ← → to move, ↑ to jump, A to attack, R to restart, E to exit, P to pause", this);
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 100);");
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setGeometry(0, height() - 30, width(), 30);

    storeButton = new QPushButton("Shop", this);
    storeButton->setGeometry(700, 70, 80, 30);
    storeButton->setStyleSheet("background-color: gold; font-weight: bold;");
    connect(storeButton, &QPushButton::clicked, this, &MainWindow::openStore);
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
        class Level4* level4 = dynamic_cast<class Level4*>(level);
        if (level4) {
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
        // Remove game over text safely
        if (scene && gameOver && scene->items().contains(gameOver)) {
            scene->removeItem(gameOver);
            delete gameOver;
        }

        // Reset level only if it still exists and is valid
        if (level) {
            try {
                // Verify the level pointer is still valid before calling methods
                int levelNum = level->getLevelNumber();  // Test access first
                level->resetLevel();

                // Reset timer display for level 4
                if (levelNum == 4 && levelTimerLabel) {
                    levelTimerLabel->setText("Time: 01:30");  // Reset to 1:30
                    levelTimerLabel->show();  // Make sure it's visible
                }

                // Reset timer display for other levels if needed
                else if (levelTimerLabel) {
                    levelTimerLabel->hide();  // Hide timer for non-timed levels
                }
            } catch (...) {
                // If any exception occurs, just skip the reset
                qDebug() << "Exception during level reset, skipping";
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
    player->resetPlayer();
    resetGameState();
    level->resetLevel();
}


void MainWindow::updateScore() {
    if (levelFinished) return;
    if (!level) {
        return;
    }
    int collected = 0;
    int requiredAmount = 20;
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
    if (scoreText) {
        scoreText->setText(QString::number(collected) + "/" + QString::number(requiredAmount));
    }
    if (collected >= requiredAmount) {
        levelFinished = true;
        if (level->getLevelNumber() == 1) {
            Message* endMessage = Message::createLevelOneCompleteMessage();
            if (endMessage && scene) {
                endMessage->showMessage(scene, 300, 300);
            }
            emit levelOneCompleted();
        } else if (level->getLevelNumber() == 2) {
            Message* endMessage = Message::createLevelTwoCompleteMessage();
            if (endMessage && scene) {
                endMessage->showMessage(scene, 300, 300);
            }
            emit levelTwoCompleted();
        } else if (level->getLevelNumber() == 3) {
            Message* endMessage = Message::createLevelThreeCompleteMessage();
            if (endMessage && scene) {
                endMessage->showMessage(scene, 300, 300);
            }
            emit levelThreeCompleted();
        } else if (level->getLevelNumber() == 4) {
            Message* endMessage = Message::createLevelFourCompleteMessage();
            if (endMessage && scene) {
                endMessage->showMessage(scene, 300, 300);
            }
            emit levelFourCompleted();
        } else if (level->getLevelNumber() == 5) {
            Message* endMessage = Message::createLevelFiveCompleteMessage();
            if (endMessage && scene) {
                endMessage->showMessage(scene, 300, 300);
            }
            emit levelFiveCompleted();
        }
        QTimer::singleShot(4000, this, [this]() {
            emit backToHome();
        });
    }
}
void MainWindow::updateGame()
{
    if (gamePaused) {
        return;
    }
    static bool updatingGame = false;
    if (updatingGame) {
        return;
    }
    updatingGame = true;

    if (player->y() > 600 || player->getHealth() <= 0) {
        showGameOver();  // Show game over message
        player->setPosition(100, 400);  // Reset player position
        updatingGame = false;  // Reset the flag
        return;
    }
    if (!player->hasFocus() && !player->isFrozenBySpaceship()) {
        player->setFocus(); //unfreeze
    }
    handlePlayerAttacks();
    bool canScroll = true; //closing scroll screen for certain levels
    // For level 3 and 4, check if enemies are still alive before activating scroll screen
    if (level && (level->getLevelNumber() == 3 || level->getLevelNumber() == 4 || level->getLevelNumber() == 5)) {
        int playerX = player->x();
        // Check for enemies in the player's current view section
        int sectionStart = (playerX / 800) * 800;
        int sectionEnd = sectionStart + 800;
        int enemiesInSection = 0;
        for (QGraphicsItem* item : scene->items()) {
            if (!item) continue;

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
                //limit how often we show messages using counter
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
            if (!item) continue;
            if (item == player || item == bg1 || item == bg2 ||
                item == healthOutline || item == healthBar) continue;
            item->moveBy(-scrollSpeed, 0);
                // moving all items and making sure they are not on the edge of the screen
            if (item->x() + item->boundingRect().width() < 0) {
                item->setX(scene->sceneRect().width());
            }
        }
    }
    //check coin collisions for all levels
    for (QGraphicsItem* item : scene->items()) {
        Coin* coin = dynamic_cast<Coin*>(item);
        if (coin) {
            coin->checkCollision(player);
        }
    }
    // Check rewards based on level
    if (level) {
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
                dynamic_cast<class Level5*>(level)->checkCavemanAttack();
                dynamic_cast<class Level5*>(level)->update();
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
    globalCoins = player->getCoins(); //coins are maintained through out the levels
    if (coinCountLabel) {
        coinCountLabel->setText(QString::number(globalCoins));
    }
    updatingGame = false;
}

void MainWindow::handlePlayerAttacks() {
    if (!player->isAttacking()) {
        return;
    }
    for (QGraphicsItem* item : scene->items()) {
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
        else if (level->getLevelNumber() == 3) {
            PolarBear* bear = dynamic_cast<PolarBear*>(item);
            if (bear && bear->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF enemyPos = bear->pos();
                qreal distance = QLineF(playerPos, enemyPos).length();
                if (distance < 150) {
                    bear->takeDamage(25);
                }
            }
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
    //hide all items
    QTimer::singleShot(3000, this, [=]() {
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
    player->resetDroplets(); // Reset all types of collectables
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
    int currentLevel = level->getLevelNumber();  // Get the current level
    globalCoins = player->getCoins();
    if (level) {
        delete level;
        level = nullptr;
    }

    if (currentLevel == 1) {
        setupLevel2();
        player->setCoins(globalCoins);
        Message* level2StartMessage = Message::createLevelTwoStartMessage();
        level2StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 2) {
        setupLevel3();
        player->setCoins(globalCoins);
        Message* level3StartMessage = Message::createLevelThreeStartMessage();
        level3StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 3) {
        setupLevel4();
        player->setCoins(globalCoins);
        Message* level4StartMessage = Message::createLevelFourStartMessage();
        level4StartMessage->showMessage(scene, 300, 300);
    }
    else if (currentLevel == 4) {
        setupLevel5();
        player->setCoins(globalCoins);
        Message* level5StartMessage = Message::createLevelFiveStartMessage();
        level5StartMessage->showMessage(scene, 300, 300);
    }
}

void MainWindow::handleLevelTwoComplete() {
    // switch to Level 3 after a small time delay
    QTimer::singleShot(4000, this, [this]() {
        switchToNextLevel();
    });
}

void MainWindow::handleLevelThreeComplete() {
    // switch to Level 4 after a small time delay
    QTimer::singleShot(4000, this, [this]() {
        switchToNextLevel();
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
void MainWindow::openStore() {
    if (!store || !player) {
        return;
    }
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
        if (level) {
            level->resetLevel();
            resetGameState();
        }
        break;

    case Qt::Key_E:
        gamePaused = false;
        QTimer::singleShot(100, this, [this]() {
            emit backToHome();
        });
        break;

    case Qt::Key_P:
        if (gamePaused) {
            resumeGame();
        } else {
            pauseGame();
        }
        break;

    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (player) {
        player->keyReleaseEvent(event);
    }
    QMainWindow::keyReleaseEvent(event);
}

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

    // dding a transparent background
    QGraphicsRectItem* background = new QGraphicsRectItem(pauseText->boundingRect());
    background->setBrush(QBrush(QColor(0, 0, 0, 150)));
    background->setPos(pauseText->pos());
    background->setData(0, "pauseBackground");
    background->setZValue(999); //  below text
    scene->addItem(background);
    pauseText->setZValue(1000); // Ensure text is on top
}

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
        for (QGraphicsItem* item : scene->items()) {
            if (item && item->data(0).toString() == "pauseBackground") {
                scene->removeItem(item);
                delete item;
                break;
            }
        }
    }
    //restarting existing timers
    QTimer* gameTimer = findChild<QTimer*>("gameTimer");
    if (!gameTimer) {
        // creates a new timer only if one doesn't exist
        gameTimer = new QTimer(this);
        gameTimer->setObjectName("gameTimer");
        connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    }
    gameTimer->start(16);
}
