#include "level5.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "mainwindow.h"
#include <QTimer>
#include "message.h"
#include "enemies.h"
#include "rewards.h"
#include "obstacle.h"

Level5::Level5(QGraphicsScene* scene, Player* p1)
    : Level(5, scene, p1) {}

void Level5::setupLevel() {
    // Remove snowflakes
    for (auto* item : scene->items()) {
        if (dynamic_cast<Earth*>(item)) {
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

    // Add Diamonds
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Diamond* diamond = new Diamond(x, y);
        scene->addItem(diamond);
    }

    const int groundY = 550;
    const int platformWidth = 400;
    const int platformHeight = 200;
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
    QRandomGenerator* randGen = QRandomGenerator::global();
    QStringList spaceTextures = {
        ":/backgrounds/caveplatform1.png",
        ":/backgrounds/caveplatform2.png",
        ":/backgrounds/CavePlatform3.png"
    };

    for (const QPoint& pos : platformPositions) {
        QString selectedTexture = spaceTextures[randGen->bounded(spaceTextures.size())];
        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(selectedTexture).scaled(platformWidth, platformHeight));
        platform->setPos(pos);
        platform->setData(0, "platform");
        scene->addItem(platform);
        obstacles.append(platform);
    }

    // Add random obstacles
    /* QRandomGenerator *randomGen = QRandomGenerator::global();
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
    */

    // Add player
    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    // Add level message
    Message* startMessage = Message::createLevelFiveStartMessage();
    startMessage->showMessage(scene, 300, 350);

    // Add the tiger enemy
    addEnemies();
}

void Level5::resetLevel() {
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);

    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();

    setupLevel();
}
void Level5::addEnemies() {
    const int groundY = 550;

    // // Add a single tiger enemy
    // Alien* alien = new Alien(400, groundY - 130);  // Position at x=400
    // alien->setData(0, "obstacle");
    // scene->addItem(alien);
    // obstacles.append(alien);
}
