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
//cleaning up the level
    for (auto* item : scene->items()) {
        if (dynamic_cast<Apple*>(item) || dynamic_cast<WaterDroplet*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();
//adding coind
    for (int i = 0; i < 15; ++i) {
        int x = 200 + i * 150;
        int y = 80 + (i % 5) * 60;
        Coin* coin = new Coin(x, y);
        scene->addItem(coin);
    }
//adding flakes
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

    QVector<QPair<QPoint, QString>> platformLayout = {
        { QPoint(100, groundY - 6 * platformSpacingY), ":/backgrounds/Iceplat1.png" },
        { QPoint(200, groundY - 4 * platformSpacingY), ":/backgrounds/Iceplat2.png" },
        { QPoint(700, groundY - 6 * platformSpacingY), ":/backgrounds/Iceplat3.png" },
        { QPoint(250, groundY - 5 * platformSpacingY), ":/backgrounds/Iceplat1.png" },
        { QPoint(550, groundY - 6 * platformSpacingY), ":/backgrounds/Iceplat2.png" }
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

    Snowman* snowman = new Snowman(600, groundY - 120, nullptr);
    scene->addItem(snowman);
    snowman->startShooting(scene);
    obstacles.append(snowman);
    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    Message* startMessage = Message::createLevelThreeStartMessage();
    startMessage->showMessage(scene, 300, 350);
    addEnemies();

}


void Level3::resetLevel() {
    p1->setHealth(100);
    p1->resetSnowflakes();
    p1->setPosition(50, 550 - 100);
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();
    QList<QGraphicsItem*> allItems = scene->items();
    for (auto* item : allItems) {
        PolarBear* polarbear = dynamic_cast<PolarBear*>(item);
        Penguin* penguin = dynamic_cast<Penguin*>(item);
        Snowflake* snowflake = dynamic_cast<Snowflake*>(item);
        Snowman* snowman = dynamic_cast<Snowman*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);
        SnowBall* snowball = dynamic_cast<SnowBall*>(item);

            if (polarbear || penguin || snowflake || snowman ||snowball || coin) {
                scene->removeItem(item);
                delete item;
            }
    }
    setupLevel();
}

void Level3::addEnemies() {
    const int platformHeight = 150;
    const int penguinHeight = 50; // accounting for penguin height
    const int penguinOffsetY = 5; // making it slightly raised up so it does not look as if its inside not on top of the platform
    Penguin* penguin1 = new Penguin(380, 250 - penguinHeight - penguinOffsetY);
    scene->addItem(penguin1);
    Penguin* penguin2 = new Penguin(270, 150 - penguinHeight - penguinOffsetY);
    scene->addItem(penguin2);
    Penguin* penguin3 = new Penguin(570, 150 - penguinHeight - penguinOffsetY);
    scene->addItem(penguin3);
    PolarBear* bear = new PolarBear(700, 500 - 140);
    scene->addItem(bear);
}

