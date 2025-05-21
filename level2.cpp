#include "level2.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "mainwindow.h"
#include <QTimer>
#include "message.h"
#include "enemies.h"
#include "rewards.h"
#include "obstacle.h"

Level2::Level2(QGraphicsScene* scene, Player* p1)
    : Level(2, scene, p1) {}

void Level2::setupLevel() {
    // Remove water droplets
    for (auto* item : scene->items()) {
        if (dynamic_cast<WaterDroplet*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Clear previous obstacles
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();

    //Add coins
    for (int i = 0; i < 15; ++i) {
        int x = 200 + i * 150;
        int y = 80 + (i % 5) * 60;
        Coin* coin = new Coin(x, y);
        scene->addItem(coin);
    }
    // Add apples
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Apple* apple = new Apple(x, y);
        scene->addItem(apple);
    }

    const int groundY = 550;
    const int platformWidth = 250;
    const int platformHeight = 100;
    const int platformSpacingY = 80;

    // Setup platforms
    QVector<QPoint> platformPositions = {
        {100, groundY - 2 * platformSpacingY},
        {300, groundY - 3 * platformSpacingY},
        {500, groundY - 4 * platformSpacingY},
        {200, groundY - 5 * platformSpacingY},
        {400, groundY - 6 * platformSpacingY}
    };

    // Add platforms
    for (const QPoint& pos : platformPositions) {
        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(":/backgrounds/brownbricks.png").scaled(platformWidth, platformHeight));
        platform->setPos(pos);
        platform->setData(0, "platform");
        scene->addItem(platform);
        obstacles.append(platform);
    }

    // Add random obstacles
    QRandomGenerator *randomGen = QRandomGenerator::global();
    QVector<int> floorPositions = {250, 600, 950, 1300, 1650};

    for (int pos : floorPositions) {
        int randObstacle = randomGen->bounded(2);
        if (randObstacle == 0) {
            Mushroom* mushroom = new Mushroom(pos, groundY - 100);
            addObstacle(mushroom);
        }
        else {
            Waterpond* pond = new Waterpond(pos, groundY - 100);
            addObstacle(pond);
        }
    }

    // Add player
    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    // Add level message
    Message* startMessage = Message::createLevelTwoStartMessage();
    startMessage->showMessage(scene, 300, 350);

    // Add the tiger enemy
    addEnemies();
}

void Level2::resetLevel() {
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);

    for (auto* item : scene->items()) {
        Tiger* tiger = dynamic_cast<Tiger*>(item);
        Apple* apple = dynamic_cast<Apple*>(item);
        Waterpond* waterpond = dynamic_cast<Waterpond*>(item);
        Mushroom* mushroom = dynamic_cast<Mushroom*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);

        if (tiger || apple || waterpond || mushroom || coin) {
            scene->removeItem(item);
            delete item;
        }
    }
    obstacles.clear();

    setupLevel();
}
void Level2::addEnemies() {
    const int groundY = 550;

    Tiger* tiger = new Tiger(400, groundY - 150);  // Position at x=400
    tiger->setData(0, "obstacle");
    scene->addItem(tiger);
    obstacles.append(tiger);
}
