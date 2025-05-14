#include "mainwindow.h"
#include "level.h"
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
    setupLevel1();
    setWindowTitle("Desert Adventure Game");
    setFixedSize(800, 600);

    scene = nullptr;
    view = nullptr;
    player = nullptr;
    level = nullptr;
    healthBar = nullptr;
    healthOutline = nullptr;
    bg1 = nullptr;
    bg2 = nullptr;

    // Initialize other variables
    levelFinished = false;
}

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

}

void MainWindow::setLevel(int levelNumber) {

    // Clean up previous level if needed
    if (scene) {
        scene->clear();
        delete scene;
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
    }else if (levelNumber == 5) {
        setupLevel5();
    }


    // Reset level state
    levelFinished = false;

    // Show the window
    show();
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

    scoreText = new QLabel("0/20", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    levelFinished = false;

    QTimer* timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

}

void MainWindow::setupLevel2() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (!view) {
        view = new QGraphicsView(scene, this);
        view->setFixedSize(800, 600);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    } else {
        view->setScene(scene);
    }

    QPixmap bgPixmap(":/backgrounds/ForrestBackground.png");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
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

    waterIcon = new QLabel(this);
    waterIcon->setPixmap(QPixmap(":/Rewards/apple.png").scaled(30, 30));
    waterIcon->move(175, 30);
    waterIcon->resize(30, 30);

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

    // Make sure timer is properly set up with fixed interval
    QTimer* timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    qDebug() << "Level 2 setup complete!";
}

void MainWindow::setupLevel3() {
    qDebug() << "Setting up Level 3...";

    // If scene already exists, clean it up
    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    // If view already exists, clean it up - otherwise create a new one
    if (!view) {
        view = new QGraphicsView(scene, this);
        view->setFixedSize(800, 600);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    } else {
        view->setScene(scene);
    }

    QPixmap bgPixmap(":/backgrounds/snowbackground.png");
    bgPixmap = bgPixmap.scaled(800, 680);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
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

    waterIcon = new QLabel(this);
    waterIcon->setPixmap(QPixmap(":/Rewards/snowflake.png").scaled(30, 30));
    waterIcon->move(175, 30);
    waterIcon->resize(30, 30);

    scoreText = new QLabel("0/25", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->setHealth(100);

    // Make sure timer is properly set up with fixed interval
    QTimer* timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    qDebug() << "Level 3 setup complete!";
}

void MainWindow::setupLevel4() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (!view) {
        view = new QGraphicsView(scene, this);
        view->setFixedSize(800, 600);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    } else {
        view->setScene(scene);
    }

    QPixmap bgPixmap(":/backgrounds/spacebackground.jpg");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
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
    levelText->setStyleSheet("color: black; font-weight: bold;");
    levelText->move(50, 30);
    levelText->resize(250, 30);

    waterIcon = new QLabel(this);
    waterIcon->setPixmap(QPixmap(":/Rewards/earth.png").scaled(30, 30));
    waterIcon->move(175, 30);
    waterIcon->resize(30, 30);

    scoreText = new QLabel("0/30", this);
    scoreText->setStyleSheet("color: black; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetEarths();
    player->setHealth(100);

    // Make sure timer is properly set up with fixed interval
    QTimer* timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    qDebug() << "Level 4 setup complete!";
}

void MainWindow::setupLevel5() {

    if (scene) {
        scene->clear();
        delete scene;
    }

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    if (!view) {
        view = new QGraphicsView(scene, this);
        view->setFixedSize(800, 600);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    } else {
        view->setScene(scene);
    }

    QPixmap bgPixmap(":/backgrounds/cavebackground.jpg");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    player = new Player();
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

    waterIcon = new QLabel(this);
    waterIcon->setPixmap(QPixmap(":/Rewards/diamond.png").scaled(30, 30));
    waterIcon->move(175, 30);
    waterIcon->resize(30, 30);

    scoreText = new QLabel("0/40", this);
    scoreText->setStyleSheet("color: white; font-weight: bold;");
    scoreText->move(200, 30);
    scoreText->resize(100, 30);

    // Reset player state explicitly
    levelFinished = false;
    player->resetDroplets();
    player->resetApples();
    player->resetSnowflakes();
    player->resetEarths();
    player->resetDiamonds();
    player->setHealth(100);

    // Make sure timer is properly set up with fixed interval
    QTimer* timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

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
    QGraphicsTextItem* gameOver = scene->addText("GAME OVER");
    gameOver->setDefaultTextColor(Qt::red);
    QFont font = gameOver->font();
    font.setPointSize(30);
    font.setBold(true);
    gameOver->setFont(font);
    gameOver->setPos(width()/2 - 100, height()/2 - 50);

    QTimer::singleShot(1000, [this, gameOver]() {
        scene->removeItem(gameOver);
        delete gameOver;
        level->resetLevel();  // Reset level settings
        player->resetPlayer();  // Reset the player state after game over
    });
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
        collected = player->getCollectedEarths();
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
    if (player->y() > 600) {
        level->resetLevel();
        resetGameState();
        return;
    }
    if (!player->hasFocus()) {
        player->setFocus();
    }
    if (player->getHealth() <= 0) {
        showGameOver();
        level->resetLevel();
        resetGameState();
        return;
    }
    const int scrollSpeed = 3;
    if (player->x() >= 600 && player->isMovingRight()) {
        bg1->moveBy(-scrollSpeed, 0);
        bg2->moveBy(-scrollSpeed, 0);
        if (bg1->x() + 800 <= 0) {
            bg1->setX(bg2->x() + 800);
        }
        if (bg2->x() + 800 <= 0) {
            bg2->setX(bg1->x() + 800);
        }
        for (QGraphicsItem* item : scene->items()) {
            Obstacle* obs = dynamic_cast<Obstacle*>(item);
            if (obs) {
                obs->handleCollision(player);
            }
        }
        updateHealthBar();
        for (QGraphicsItem* item : scene->items()) {
            if (item == player || item == bg1 || item == bg2 ||
                item == healthOutline || item == healthBar) continue;
            item->moveBy(-scrollSpeed, 0);
            if (item->x() + item->boundingRect().width() < 0) {
                item->setX(scene->sceneRect().width());
            }
        }
    }
    if (level->getLevelNumber() == 1) {
        for (QGraphicsItem* item : scene->items()) {
            WaterDroplet* droplet = dynamic_cast<WaterDroplet*>(item);
            if (droplet) {
                droplet->checkCollision(player);
            }
        }
    } else if (level->getLevelNumber() == 2) {
        for (QGraphicsItem* item : scene->items()) {
            Apple* apple = dynamic_cast<Apple*>(item);
            if (apple) {
                apple->checkCollision(player);
            }
        }
    } else if (level->getLevelNumber() == 3) {
        for (QGraphicsItem* item : scene->items()) {
            Snowflake* flake = dynamic_cast<Snowflake*>(item);
            if (flake) {
                flake->checkCollision(player);
            }
        }
    } else if (level->getLevelNumber() == 4) {
        for (QGraphicsItem* item : scene->items()) {
            Earth* earth = dynamic_cast<Earth*>(item);
            if (earth) {
                earth->checkCollision(player);
            }
        }
    } else if (level->getLevelNumber() == 5) {
        for (QGraphicsItem* item : scene->items()) {
            Diamond* diamond = dynamic_cast<Diamond*>(item);
            if (diamond) {
                diamond->checkCollision(player);
            }
        }
    }

    // Handle player attacks on enemies
    if (player->isAttacking()) {
        for (QGraphicsItem* item : scene->items()) {
            // Level 2 - Tiger
            if (level->getLevelNumber() == 2) {
                Tiger* tiger = dynamic_cast<Tiger*>(item);
                if (tiger && tiger->isAlive()) {
                    QPointF playerPos = player->pos();
                    QPointF enemyPos = tiger->pos();
                    qreal distance = QLineF(playerPos, enemyPos).length();
                    if (distance < 150) {
                        // Call takeDamage and check the return value
                        // We don't need to remove/delete the tiger anymore as it will respawn itself
                        tiger->takeDamage(25);
                    }
                }
            }     // Level 3 - Polar Bear
            else if (level->getLevelNumber() == 3) {
                PolarBear* bear = dynamic_cast<PolarBear*>(item);
                if (bear && bear->isAlive()) {
                    QPointF playerPos = player->pos();
                    QPointF enemyPos = bear->pos();
                    qreal distance = QLineF(playerPos, enemyPos).length();
                    if (distance < 150) {
                        bear->takeDamage(25);
                        // Note: Assuming you've updated PolarBear to handle its own death/respawn
                        // If not, you'll need similar code as used for Tiger
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
                        alien->takeDamage(25);
                        // Note: Assuming you've updated Alien to handle its own death/respawn
                        // If not, you'll need similar code as used for Tiger
                    }
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
}



void MainWindow::resetGameState() {
    levelFinished = false;
    player->resetDroplets(); // Reset all types
    player->resetApples();
    player->resetSnowflakes();
    player -> resetEarths();
    player -> resetDiamonds();
    scoreText->setText("0/20"); // Optional: update to reflect current level requirement
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

    if (level) {
        delete level;  // Clean up current level resources
        level = nullptr;
    }

    if (currentLevel == 1) {
        setupLevel2();
        Message* level2StartMessage = Message::createLevelTwoStartMessage();
        level2StartMessage->showMessage(scene, 300, 300);
    } else if (currentLevel == 2) {
        setupLevel3();
        Message* level3StartMessage = Message::createLevelThreeStartMessage();
        level3StartMessage->showMessage(scene, 300, 300);

    }else if (currentLevel == 3) {
        setupLevel4();
        Message* level4StartMessage = Message::createLevelFourStartMessage();
        level4StartMessage->showMessage(scene, 300, 300);
    } else if (currentLevel == 4) {
        setupLevel5();
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
