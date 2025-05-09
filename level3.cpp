#include "level3.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "mainwindow.h"
#include <QTimer>
#include "message.h"
#include "enemies.h"
#include "rewards.h"
#include "obstacle.h"

Level3::Level3(QGraphicsScene* scene, Player* p1)
    : Level(3, scene, p1) {}

void Level3::setupLevel() {
    // Remove apples or water droplets
    for (auto* item : scene->items()) {
        if (dynamic_cast<Apple*>(item) || dynamic_cast<WaterDroplet*>(item)) {
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

    // Add snowflakes (or your level 3 reward)
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Snowflake* flake = new Snowflake(x, y);
        scene->addItem(flake);
    }


    const int groundY = 550;
    const int platformWidth = 400;
    const int platformHeight = 150;
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
    // Add platforms
    QRandomGenerator* randGen = QRandomGenerator::global();
    QStringList iceTextures = {
        ":/backgrounds/Iceplat1.png",
        ":/backgrounds/Iceplat2.png",
        ":/backgrounds/Iceplat3.png"
    };

    for (const QPoint& pos : platformPositions) {
        QString selectedTexture = iceTextures[randGen->bounded(iceTextures.size())];
        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(selectedTexture).scaled(platformWidth, platformHeight));
        platform->setPos(pos);
        platform->setData(0, "platform");
        scene->addItem(platform);
        obstacles.append(platform);
    }



    QRandomGenerator *randomGen = QRandomGenerator::global();
    QVector<int> floorPositions = {250, 600, 950, 1300, 1650};

    for (int pos : floorPositions) {
        int randObstacle = randomGen->bounded(3);
        if (randObstacle == 0) {
            Snowman* snowman = new Snowman(pos, groundY - 100);
            addObstacle(snowman);
        }
        else if (randObstacle == 1) {
            Hole* hole = new Hole(pos, groundY - 100);
            addObstacle(hole);
        }
        else {
            SnowBall* ball = new SnowBall(pos, groundY - 100);
            addObstacle(ball);
        }
    }

    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    Message* startMessage = Message::createLevelThreeStartMessage();
    startMessage->showMessage(scene, 300, 350);

    addEnemies();
}

void Level3::resetLevel() {
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);

    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();

    setupLevel();
}

void Level3::addEnemies() {
    const int groundY = 500;

    Penguin* penguin = new Penguin(550, groundY - 90);
    penguin->setData(0, "obstacle");
    scene->addItem(penguin);
    obstacles.append(penguin);

    PolarBear* bear = new PolarBear(700, groundY - 140);
    bear->setData(0, "obstacle");
    scene->addItem(bear);
    obstacles.append(bear);
}
