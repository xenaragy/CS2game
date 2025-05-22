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
    : Level(5, scene, p1) {
    // Initialize all pointers to nullptr
    caveman = nullptr;
    troll = nullptr;
    fightTimer = nullptr;

    // Initialize battle state flags
    cavemanAttackActive = false;
    battleStarted = false;
    cinematicPlaying = false;
    trollAttackActive = false;
    batSpawned = false;
}

void Level5::setupLevel() {

    for (auto* item : scene->items()) {
        if (dynamic_cast<Star*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

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

    // Add Diamonds
    for (int i = 0; i < 40; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Diamond* diamond = new Diamond(x, y);
        scene->addItem(diamond);
    }

    const int groundY = 550;
    const int platformWidth = 400;
    const int platformHeight = 200;
    const int platformSpacingY = 80;

    QVector<QPair<QPoint, QString>> platformLayout = {
        // Position, Texture path
        { QPoint(100, groundY - 2.5 * platformSpacingY), ":/backgrounds/caveplatform1.png" },
        { QPoint(300, groundY - 4.5 * platformSpacingY), ":/backgrounds/caveplatform1.png" },
        { QPoint(700, groundY - 6 * platformSpacingY), ":/backgrounds/caveplatform1.png"}
    };

    for (const auto& platformInfo : platformLayout) {
        QPoint pos = platformInfo.first;
        QString texturePath = platformInfo.second;

        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(texturePath).scaled(platformWidth, platformHeight));
        platform->setPos(pos);
        platform->setData(0, "platform");
        scene->addItem(platform);
        obstacles.append(platform);
    }
    // Add player
    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    // Add level message
    Message* startMessage = Message::createLevelFiveStartMessage();
    startMessage->showMessage(scene, 300, 350);

    // Add the enemy
    addEnemies();

    cavemanAttackActive = false;
    battleStarted = false;
    cinematicPlaying = false;
    trollAttackActive = false;
}

void Level5::addEnemies() {
    const int creatureHeight = 50;
    const int creatureOffset = 5;
    CaveCreature* creature1 = new CaveCreature(300, 130 - creatureHeight - creatureOffset);
    scene->addItem(creature1);
    CaveCreature* creature3 = new CaveCreature(100, 295 - creatureHeight - creatureOffset);
    scene->addItem(creature3);
    caveman = nullptr;
    troll = nullptr;
}

void Level5::checkTrollAttack() {
    if (!trollAttackActive && p1->getCollectedDiamonds() >= 20) {
        startTrollAttack();
    }
}

void Level5::checkBatAttack() {
    if (!batSpawned && p1->getCollectedDiamonds() >= 10) {
        batSpawned = true;

        Bat* bat = new Bat(p1->x() + 300, p1->y() - 100);  // Adjust spawn position as needed
        scene->addItem(bat);
    }

    for (QGraphicsItem* item : scene->items()) {
        Bat* bat = dynamic_cast<Bat*>(item);
        if (bat) {
            bat->handleCollision(p1);
        }
    }
}

void Level5::startCavemanAttack() {
    cavemanAttackActive = true;

    Message* warn = new Message("Caveman Incoming!", 2000);
    warn->showMessage(scene, 350, 280);

    int spawnX = scene->width() - 150;
    int groundY = 550;
    int spawnY = groundY - 150;

    for (QGraphicsItem* item : scene->items()) {
        if (Caveman* oldCaveman = dynamic_cast<Caveman*>(item)) {
            scene->removeItem(oldCaveman);
            delete oldCaveman;
        }
    }
    caveman = new Caveman(spawnX, spawnY);
    scene->addItem(caveman);
}

void Level5::checkCavemanAttack() {
    if (!cavemanAttackActive && p1->getCollectedDiamonds() >= 30) {
        startCavemanAttack();
    }
}

void Level5::startTrollAttack() {
        trollAttackActive = true;

        Message* warn = new Message("Troll Incoming!", 2000);
        warn->showMessage(scene, 350, 280);

        // Spawn near player but always at a fixed height
        int spawnX = scene->width() - 100; // More consistent spawn location
        int spawnY = 200; // Fixed height relative to ground

        // Remove any existing trolls first
        for (QGraphicsItem* item : scene->items()) {
            if (Troll* oldTroll = dynamic_cast<Troll*>(item)) {
                scene->removeItem(oldTroll);
                delete oldTroll;
            }
        }

        troll = new Troll(spawnX, spawnY);
        scene->addItem(troll);
}

void Level5::update() {
    checkCavemanAttack();
    checkTrollAttack();
    checkBatAttack();
}

void Level5::resetLevel() {
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);
    p1->resetDiamonds();
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();
    QList<QGraphicsItem*> allItems = scene->items();
    for (auto* item : allItems) {
        Caveman* caveman = dynamic_cast<Caveman*>(item);
        CaveCreature* creature = dynamic_cast<CaveCreature*>(item);
        Troll* troll = dynamic_cast<Troll*>(item);
        Bat* bat = dynamic_cast<Bat*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);
        Diamond* diamond = dynamic_cast<Diamond*>(item);

        if (caveman || creature || troll ||bat || coin || diamond) {
            scene->removeItem(item);
            delete item;
        }
    }
    setupLevel();
}

void Level5::killTroll() {
    for (QGraphicsItem* item : scene->items()) {
        if (Troll* trollItem = dynamic_cast<Troll*>(item)) {

            trollItem->health = 0;
            trollItem->setVisible(false);

            checkCavemanAttack();
            return;
        }
    }
}
