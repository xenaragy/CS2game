#include "enemies.h"
#include <QTimer>
#include <QGraphicsScene>

Enemies::Enemies(int x, int y, const QString& spritePath, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(QPixmap(spritePath)) {
    setPos(x, y);
}

Enemies::~Enemies() {}

void Enemies::checkCollision(Player* player) {
    if (collidesWithItem(player)) {
        if (!player->canTakeDamage(1000)) {
            return;
        }
        player->takeDamage(10);
        player->takeDamagePercent(0.05f);
    }
}

// Tiger implementation
Tiger::Tiger(int x, int y, QObject* parent)
    : Enemies(x, y, ":/Enemies/tiger.png", parent) {
    setScale(0.8);  // Adjust scale as needed
    startX = x;
    direction = 1;
    range = 200;
    health = 100;  // Initialize with 100 health

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Tiger::patrol);
    movementTimer->start(50);

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}

Tiger::~Tiger() {
    if (movementTimer) {
        movementTimer->stop();
        delete movementTimer;
    }
    if (damageTimer) {
        damageTimer->stop();
        delete damageTimer;
    }
}

void Tiger::move() {
    // This is your existing movement code
    // If you already have a move implementation, keep it here
}


void Tiger::patrol() {
    // Patrol logic: Tiger moves left and right within a fixed range
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}


// New methods for tiger health and damage
bool Tiger::takeDamage(int damage) {
    if (!canTakeDamage()) return false;

    health -= damage;
    if (health <= 0) {
        // Tiger is defeated
        health = 0;
        movementTimer->stop();  // Stop movement
        setVisible(false);
        return true;
    }

    // Tiger was damaged but not defeated
    damageTimer->start(500);  // Cooldown between damage
    return false;
}

bool Tiger::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Tiger::isAlive() const {
    return health > 0;
}
