#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QPixmap>
#include "obstacle.h"
#include "waterdroplet.h"
#include "home.h"
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QPen>
#include <QTimer>
#include "Level.h"
#include "message.h"
#include "apple.h"
#include "level2.h"
#include "enemies.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupLevel1();
    setWindowTitle("Desert Adventure Game");
    setFixedSize(800, 600);
    connect(this, &MainWindow::levelTwoCompleted, this, &MainWindow::handleLevelTwoComplete);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete view;
    delete player;
    delete level;
}

void MainWindow::setLevel(int level)
{
    if (level == 1) {
        setupLevel1();
    } else if (level == 2) {
        setupLevel2();
    }
}

void MainWindow::setupLevel1()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

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

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);
}

void MainWindow::setupLevel2() {
    // Remove existing items but don't delete the scene itself
    if (scene) {
        scene->clear();
    } else {
        scene = new QGraphicsScene(this);
    }
    scene->setSceneRect(0, 0, 800, 600);

    if (!view) {
        view = new QGraphicsView(scene, this);
        view->setFixedSize(800, 600);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        setCentralWidget(view);
    }

    // Create background
    QPixmap bgPixmap(":/backgrounds/ForrestBackground.png");
    bgPixmap = bgPixmap.scaled(800, 600);
    bg1 = scene->addPixmap(bgPixmap);
    bg2 = scene->addPixmap(bgPixmap);
    bg1->setPos(0, 0);
    bg2->setPos(800, 0);
    bg1->setZValue(-1);
    bg2->setZValue(-1);

    // Create player
    player = new Player();
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPosition(100, 400);

    // Create Level 2
    if (level) {
        delete level;
    }
    level = new class Level2(scene, player);
    level->setupLevel();

    // Health bar
    healthOutline = new QGraphicsRectItem(0, 0, 200, 20);
    healthOutline->setPen(QPen(Qt::black));
    scene->addItem(healthOutline);

    healthBar = new QGraphicsRectItem(0, 0, 200, 20);
    healthBar->setBrush(Qt::green);
    scene->addItem(healthBar);
    healthOutline->setPos(590, 40);
    healthBar->setPos(590, 40);

    // UI elements
    levelText->setText("Level: 2");
    waterIcon->setPixmap(QPixmap(":/Rewards/apple.png").scaled(30, 30));
    scoreText->setText("0/20");

    // Reset game state
    levelFinished = false;
    player->resetDroplets();
    player->setHealth(100);

    // Start game loop
    QTimer* timer = new QTimer(this);
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
    }

    // Update score display
    scoreText->setText(QString::number(collected) + "/" + QString::number(requiredAmount));

    // Check if level is completed
    if (collected >= requiredAmount) {
        levelFinished = true;

        // Show appropriate completion message
        if (level->getLevelNumber() == 1) {
            Message* endMessage = Message::createLevelOneCompleteMessage();
            endMessage->showMessage(scene, 300, 300);

            // Signal level completion
            emit levelOneCompleted();

            // After delay, go back to home screen
            QTimer::singleShot(4000, this, [this]() {
                emit backToHome();  // This should send you back to the home screen
            });
        } else if (level->getLevelNumber() == 2) {
            Message* endMessage = Message::createLevelTwoCompleteMessage();
            endMessage->showMessage(scene, 300, 300);

            // Signal level completion
            emit levelTwoCompleted();

            // After a delay, go back to home
            QTimer::singleShot(4000, this, [this]() {
                emit backToHome();
            });
        }
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
    }else if (level->getLevelNumber() == 2) {
        for (QGraphicsItem* item : scene->items()) {
            Apple* apple = dynamic_cast<Apple*>(item);
            if (apple) {
                apple->checkCollision(player);
            }
        }
    }

    if (player->isAttacking()) {
        for (QGraphicsItem* item : scene->items()) {
            Tiger* tiger = dynamic_cast<Tiger*>(item);
            if (tiger && tiger->isAlive()) {
                QPointF playerPos = player->pos();
                QPointF tigerPos = tiger->pos();
                qreal distance = QLineF(playerPos, tigerPos).length();
                if (distance < 150) {
                    if (tiger->takeDamage(25)) {
                        QTimer::singleShot(1000, this, [this, tiger]() {
                            scene->removeItem(tiger);
                            delete tiger;
                        });
                    }
                }
            }
        }
    }

    // Position UI elements
    int margin = 10;
    int viewRight = this->width();
    healthOutline->setPos(viewRight - 230, margin + 30);
    healthBar->setPos(viewRight - 230, margin + 30);
    updateHealthBar();
    updateScore();
}

void MainWindow::goToHome() {
    if (!levelFinished) {
        qDebug() << "Level not finished, returning early.";
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
    player->resetDroplets();
    scoreText->setText("0/20");
    level->resetLevel();
}

void MainWindow::nextLevel() {
    int currentLevel = level->getLevelNumber();
    if (currentLevel == 1) {
        level->nextLevel();
        setupLevel2();
    } else {
        // Add any additional logic for higher levels
    }

}
void MainWindow::switchToNextLevel() {
    // Don't create and close a new Home window
    // Just switch to Level 2 directly

    qDebug() << "Switching to Level 2...";

    // Clean up level 1 resources
    if (level) {
        delete level;
        level = nullptr;
    }

    // Setup Level 2
    setupLevel2();

    // Display message for Level 2 start
    Message* level2StartMessage = Message::createLevelTwoStartMessage();
    level2StartMessage->showMessage(scene, 300, 300);

    qDebug() << "Now playing Level 2";
}
void MainWindow::handleLevelTwoComplete() {

    // After a delay, go back to home
    QTimer::singleShot(5000, this, [this]() {
        emit backToHome();
    });
}

// void MainWindow::keyPressEvent(QKeyEvent *event)
// {
//     player->keyPressEvent(event);
//     if (event->key() == Qt::Key_R) {
//         level->resetLevel();
//     } else if (event->key() == Qt::Key_N) {
//         level->nextLevel();
//     }
//     QMainWindow::keyPressEvent(event);
// }

// void MainWindow::keyReleaseEvent(QKeyEvent *event)
// {
//     player->keyReleaseEvent(event);
//     QMainWindow::keyReleaseEvent(event);
// }
