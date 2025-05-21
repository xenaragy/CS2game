#include "Level.h"
#include "obstacle.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "rewards.h"
#include "mainwindow.h"
#include <QTimer>
#include "Message.h"


Level::Level(int number, QGraphicsScene* scene, Player* p1)
    : scene(scene), p1(p1), levelNumber(number) {}

void Level::setupLevel()
{
    for (auto* item : scene->items()) {
        if (dynamic_cast<WaterDroplet*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        WaterDroplet* droplet = new WaterDroplet(x, y);
        scene->addItem(droplet);
    }
    for (int i = 0; i < 15; ++i) {
        int x = 200 + i * 150;
        int y = 80 + (i % 5) * 60;
        Coin* coin = new Coin(x, y);
        scene->addItem(coin);
    }

    const int groundY = 550;
    const int platformWidth = 250;
    const int platformHeight = 100;
    const int platformSpacingY = 80;

    if (levelNumber == 1) {
        QVector<QPoint> platformPositions = {
                                             {100, groundY - 2 * platformSpacingY},
                                             {300, groundY - 3 * platformSpacingY},
                                             {500, groundY - 4 * platformSpacingY},
                                             {200, groundY - 5 * platformSpacingY},
                                             {400, groundY - 6 * platformSpacingY},
                                             {100, groundY - 4 * platformSpacingY},
                                             };

        QRandomGenerator *randomGen = QRandomGenerator::global();

        for (const QPoint& pos : platformPositions) {
            QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(":/backgrounds/brownbricks.png").scaled(platformWidth, platformHeight));
            platform->setPos(pos);
            platform->setData(0, "platform");
            scene->addItem(platform);
            obstacles.append(platform);
        }

        //adding random obstacles
        QVector<int> floorPositions = {250, 600, 950, 1300, 1650};
        for (int pos : floorPositions) {
            int randObstacle = randomGen->bounded(3);
            if (randObstacle == 0) {
                Fire* fire = new Fire(pos, groundY - 50);
                addObstacle(fire);
            }
            else if (randObstacle == 1) {
                Cactus* cactus = new Cactus(pos, groundY - 50);
                addObstacle(cactus);
            }
            else if (randObstacle == 2) {
                Quicksand* quicksand = new Quicksand(pos, groundY-50);
                quicksand->setLevel(this);
                addObstacle(quicksand);
            }
        }
    }
    scene->addItem(p1);
    p1->setPosition(50, groundY - 100);
    if (levelNumber == 1) {
        Message* startMessage = Message::createLevelOneStartMessage();
        startMessage->showMessage(scene, 300, 350);
    }
}


void Level::addObstacle(QGraphicsItem* obstacle) {
    obstacle->setData(0, "obstacle");
    obstacles.push_back(obstacle);
    scene->addItem(obstacle);
}


void Level::resetLevel() {
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);
    p1->setDropletsCollected(0);
    for (auto* item : scene->items()) {
        Fire* fire = dynamic_cast<Fire*>(item);
        Cactus* cactus = dynamic_cast<Cactus*>(item);
        WaterDroplet* droplet = dynamic_cast<WaterDroplet*>(item);
        Quicksand* sand = dynamic_cast<Quicksand*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);

        if (fire || cactus || droplet || sand || coin) {
            scene->removeItem(item);
            delete item;
        }
    }
    obstacles.clear();
    setupLevel();
}


void Level::nextLevel()
{
    levelNumber++;
    setupLevel();
}

int Level::getLevelNumber() const
{
    return levelNumber;
}


