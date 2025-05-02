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
#include "home.h"

  MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGame();
    setWindowTitle("Desert Adventure Game");
    setFixedSize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete view;
    delete player;
    delete level;
}

void MainWindow::setupGame()
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
        level->resetLevel();
    });
}


void MainWindow::updateScore() {
    if (levelFinished) return;
    int collected = player->getCollectedDroplets();
    scoreText->setText(QString::number(collected) + "/20");
    if (collected == 20) {
        levelFinished = true;
        QTimer::singleShot(0, this, SLOT(goToHome()));
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
    }
    const int scrollSpeed = 5;
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
    for (QGraphicsItem* item : scene->items()) {
        WaterDroplet* droplet = dynamic_cast<WaterDroplet*>(item);
        if (droplet) {
            droplet->checkCollision(player);
        }
    }

    int margin = 10;
    int viewRight = this->width();
    healthOutline->setPos(viewRight - 230, margin + 30);
    healthBar->setPos(viewRight - 230, margin + 30);
    updateHealthBar();
    updateScore();
}

void MainWindow::goToHome()
{
    player->setVisible(false);
    for (QGraphicsItem* item : scene->items()) {
        if (item != healthOutline && item != healthBar && item != bg1 && item != bg2 && item != player) {
            item->setVisible(false);
        }
    }

    levelText->hide();
    scoreText->hide();
    waterIcon->hide();
    QPushButton* portalButton = new QPushButton(this);
    portalButton->setIcon(QIcon(":/backgrounds/portal.png"));
    portalButton->setIconSize(QSize(300, 300));
    portalButton->setFlat(true);
    portalButton->setGeometry(350, 200, 100, 100);
    portalButton->show();

    Home* homeWindow = new Home();
    homeWindow->show();
    this->close();
}


void MainWindow::resetGameState() {
    levelFinished = false;
    player->resetDroplets();
    scoreText->setText("0/20");
    level->resetLevel();
}



// void MainWindow::keyPressEvent(QKeyEvent *event)
// {
//     // player->keyPressEvent(event);
//     // if (event->key() == Qt::Key_R) {
//     //     level->resetLevel();
//     // } else if (event->key() == Qt::Key_N) {
//     //     level->nextLevel();
//     // }
//     QMainWindow::keyPressEvent(event);
// }

// void MainWindow::keyReleaseEvent(QKeyEvent *event)
// {
//     player->keyReleaseEvent(event);
//     QMainWindow::keyReleaseEvent(event);
// }
