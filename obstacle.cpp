#include "obstacle.h"
#include "Player.h"


Obstacle::Obstacle(const QPixmap& pixmap, int x, int y, ObstacleType type, int damage, bool isMovable)
    : QGraphicsPixmapItem(pixmap), obstacleType(type), damage(damage), movable(isMovable), speed(0)
{
    setPos(x, y);
}

void Obstacle::setType(ObstacleType type) {
    obstacleType = type;
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

void Obstacle::move() {
    if (movable) {
        // Implement movement logic here (e.g., moving left-right)
        setPos(x() + speed, y());  // Simple left-right movement
        if (x() < 100 || x() > 600) { // Bouncing between boundaries
            speed = -speed;
        }
    }
}

void Obstacle::handleCollision(Player* player) {
    // Default behavior for a collision, can be overridden by subclasses
}

Fire::Fire(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/firepit.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 10) // Fire deals -10 damage
{}

void Fire::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();  // Call collidingItems as a function
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {

            player->takeDamage(10);  // Fire damages the player by 10

            player->takeDamagePercent(0.10f);  // Fire damages the player by 10

        }
    }
}

Cactus::Cactus(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/cactus.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 20) // Cactus deals -20 damage
{}

void Cactus::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();  // Call collidingItems as a function
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {

            player->takeDamage(20);  // Cactus damages the player by 20

          player->takeDamagePercent(0.05f); // Cactus damages the player by 5

        }
    }
}

Quicksand::Quicksand(int x, int y)
    : Obstacle(QPixmap(":/Obstacles/quicksand.png").scaled(50, 50), x, y, Obstacle::ObstacleType::Hazard, 0)
{}


void Quicksand::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();  // Call collidingItems as a function
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            player->takeDamagePercent(0.05f);
            // Reset the level when player collides with quicksand
            level->resetLevel();
        }
    }
}



void Quicksand::setLevel(Level* l) {
    level = l;  // Set the level pointer
}
