#include "obstacle.h"
#include "Player.h"
#include "MainWindow.h"


Obstacle::Obstacle(const QPixmap& pixmap, int x, int y, ObstacleType type, int damage, bool isMovable)
    : QGraphicsPixmapItem(pixmap), obstacleType(type), damage(damage), movable(isMovable), speed(0)
{
    setPos(x, y);
}


Obstacle::ObstacleType Obstacle::getType() const {
    return obstacleType;
}

void Obstacle::setDamage(int dmg) {
    damage = dmg;
}

int Obstacle::getDamage() const {
    return damage;
}

void Obstacle::setMovable(bool isMovable) {
    movable = isMovable;
}

bool Obstacle::isMovable() const {
    return movable;
}


void Obstacle::handleCollision(Player* player) {} //overriding function

Fire::Fire(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/firepit.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 10)
{}

void Fire::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(10);
            player->takeDamagePercent(0.02f);
        }
    }
}

Cactus::Cactus(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/cactus.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 10)
{}

void Cactus::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(10);
            player->takeDamagePercent(0.02f);
        }
    }
}

Quicksand::Quicksand(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/quicksand.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 0)
{}


void Quicksand::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(100);
            player->takeDamagePercent(0.02f);
        }
    }
}


void Quicksand::setLevel(Level* l) {
    level = l;
}



Mushroom :: Mushroom(int x, int y) :
Obstacle(QPixmap(":/Obstacles/poisonMushroom.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 10)
{}

void Mushroom::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(10);
            player->takeDamagePercent(0.02f);
        }
    }
}

Waterpond::Waterpond(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/waterpond.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 0)
{}
void Waterpond::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(100);
            player->takeDamagePercent(0.02f);
        }
    }
}


void Waterpond::setLevel(Level* l) {
    level = l;
}
