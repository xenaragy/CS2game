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



Penguin::Penguin(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)  // We'll set pixmap manually below
{
    QPixmap pix(":/Enemies/penguin.png");
    setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Adjust size here

    setPos(x, y);

    startX = x;
    direction = 1;
    range = 100;         // Smaller patrol range than Tiger
    health = 50;         // Easier to defeat

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Penguin::patrol);
    movementTimer->start(70);  // Slower than Tiger

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}


Penguin::~Penguin() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer) damageTimer->stop();
}

void Penguin::patrol() {
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

void Penguin::move() {
    // already handled by patrol
}

bool Penguin::takeDamage(int damage) {
    if (!canTakeDamage()) return false;

    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        return true;
    }

    damageTimer->start(500);
    return false;
}

bool Penguin::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Penguin::isAlive() const {
    return health > 0;
}



PolarBear::PolarBear(int x, int y, QObject* parent)
    : Enemies(x,y+60, ":/Enemies/polarbear.png", parent) // offset Y so it's lower on the screen
{
    setScale(0.2);         // Slightly larger than penguin
    startX = x;
    direction = 1;
    range = 250;           // Larger patrol range than tiger
    health = 150;          // Stronger enemy

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &PolarBear::patrol);
    movementTimer->start(40);  // Faster than tiger

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}


PolarBear::~PolarBear() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer) damageTimer->stop();
}

void PolarBear::patrol() {
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

void PolarBear::move() {
    // Already handled by patrol
}

bool PolarBear::takeDamage(int damage) {
    if (!canTakeDamage()) return false;

    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        return true;
    }

    damageTimer->start(500);
    return false;
}

bool PolarBear::canTakeDamage() {
    return !damageTimer->isActive();
}

bool PolarBear::isAlive() const {
    return health > 0;
}

Alien::Alien(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)  // We'll set pixmap manually below
{
    QPixmap pix(":/Enemies/alien.png");
    setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Adjust size here

    setPos(x, y);

    startX = x;
    direction = 1;
    range = 100;         // Smaller patrol range than Tiger
    health = 50;         // Easier to defeat

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Alien::patrol);
    movementTimer->start(70);  // Slower than Tiger

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}


Alien::~Alien() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer) damageTimer->stop();
}

void Alien::patrol() {
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

void Alien::move() {
    // already handled by patrol
}

bool Alien::takeDamage(int damage) {
    if (!canTakeDamage()) return false;

    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        return true;
    }

    damageTimer->start(500);
    return false;
}

bool Alien::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Alien::isAlive() const {
    return health > 0;
}
