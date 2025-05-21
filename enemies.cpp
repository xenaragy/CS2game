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

Tiger::Tiger(int x, int y, QObject* parent)
    : Enemies(x, y, ":/Enemies/tiger.png", parent) {
    setScale(0.8);  // Adjust scale as needed
    startX = x;
    startY = y;     // Store Y position for respawn
    direction = 1;
    range = 300;
    health = 100;  // Initialize with 100 health
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Tiger::patrol);
    movementTimer->start(10);
    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);

    // Add respawn timer
    respawnTimer = new QTimer(this);
    respawnTimer->setSingleShot(true);
    connect(respawnTimer, &QTimer::timeout, this, &Tiger::respawn);
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
    if (respawnTimer) {  // Add cleanup for respawn timer
        respawnTimer->stop();
        delete respawnTimer;
    }
}

void Tiger::patrol() {
    // Patrol logic: Tiger moves left and right within a fixed range
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }
    setPos(x() + 2 * direction, y());

    if (scene()) {
        Player* player = dynamic_cast<Player*>(scene()->focusItem());
        if (player) {
            checkCollision(player);
        }
    }
}
// Example for Tiger class in enemies.cpp
bool Tiger::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    // Check if player has super attack
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2; // Double damage with super attack
    }
    health -= damage;
    if (health <= 0) {
        // Tiger is defeated
        health = 0;
        movementTimer->stop();  // Stop movement
        setVisible(false);

        // Reward coins when defeated
        if (player) {
            // Bonus coins for super attack kill
            if (player->hasSuperAttack()) {
                player->incrementCoins(3);
                Message* bonusMsg = new Message("+3 Coins: Super Kill Bonus!", 1500);
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(1);
                Message* coinMsg = new Message("+1 Coin", 1000);
                coinMsg->showMessage(scene(), 300, 200);
            }
        }

        respawnTimer->start(3000);  // 3 second respawn delay
        return true;
    }
    damageTimer->start(500);
    return false;
}

bool Tiger::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Tiger::isAlive() const {
    return health > 0;
}

void Tiger::respawn() {
    health = 100;
    setPos(startX, startY);

    setVisible(true);
    movementTimer->start(20);
}

Penguin::Penguin(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)
{
    QPixmap pix(":/Enemies/penguin.png");
    setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Adjust size here

    setPos(x, y);

    startX = x;
    direction = 1;
    range = 30;         // Smaller patrol range than Tiger
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


bool Penguin::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    // Check if player has super attack
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2; // Double damage with super attack
    }
    health -= damage;
    if (health <= 0) {
        // Enemy is defeated
        health = 0;
        movementTimer->stop();  // Stop movement
        setVisible(false);

        // Reward coins when defeated
        if (player) {
            // Bonus coins for super attack kill
            if (player->hasSuperAttack()) {
                player->incrementCoins(3);
                Message* bonusMsg = new Message("+3 Coins: Super Kill Bonus!", 1500);
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(1);
                Message* coinMsg = new Message("+1 Coin", 1000);
                coinMsg->showMessage(scene(), 300, 200);
            }
        }
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
    : Enemies(x, y + 60, ":/Enemies/polarbear.png", parent)
{
    setScale(0.2);
    startX = x;
    direction = 1;
    range = 250;
    health = 150;

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &PolarBear::patrol);
    movementTimer->start(40);

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

bool PolarBear::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    // Check if player has super attack
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2; // Double damage with super attack
    }
    health -= damage;
    if (health <= 0) {
        // Enemy is defeated
        health = 0;
        movementTimer->stop();  // Stop movement
        setVisible(false);

        // Reward coins when defeated
        if (player) {
            // Bonus coins for super attack kill
            if (player->hasSuperAttack()) {
                player->incrementCoins(3);
                Message* bonusMsg = new Message("+3 Coins: Super Kill Bonus!", 1500);
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(1);
                Message* coinMsg = new Message("+1 Coin", 1000);
                coinMsg->showMessage(scene(), 300, 200);
            }
        }
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
    setPixmap(pix.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Adjust size here

    setPos(x, y);

    startX = x;
    direction = 1;
    range = 30;         // Smaller patrol range than Tiger
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

bool Alien::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    // Check if player has super attack
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2; // Double damage with super attack
    }
    health -= damage;
    if (health <= 0) {
        // Enemy is defeated
        health = 0;
        movementTimer->stop();  // Stop movement
        setVisible(false);

        // Reward coins when defeated
        if (player) {
            // Bonus coins for super attack kill
            if (player->hasSuperAttack()) {
                player->incrementCoins(3);
                Message* bonusMsg = new Message("+3 Coins: Super Kill Bonus!", 1500);
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(1);
                Message* coinMsg = new Message("+1 Coin", 1000);
                coinMsg->showMessage(scene(), 300, 200);
            }
        }
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
CaveCreature::CaveCreature(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)  // pixmap set below
{
    QPixmap pix(":/Enemies/caveCreature.png");
    setPixmap(pix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    setPos(x, y);

    startX    = x;
    direction = 1;
    range     = 30;   // patrol range
    health    = 50;   // hit points

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &CaveCreature::patrol);
    movementTimer->start(70);  // patrol speed

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}

CaveCreature::~CaveCreature() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer)   damageTimer->stop();
}

void CaveCreature::patrol() {
    if      (x() >= startX + range) direction = -1;
    else if (x() <= startX)         direction =  1;

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

bool CaveCreature::takeDamage(int dmg) {
    if (!canTakeDamage()) return false;

    health -= dmg;

    if (health <= 0) {
        health = 0;

        // Remove from scene on death
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();  // Schedule deletion
        return true;
    }

    damageTimer->start(500);
    return false;
}


bool CaveCreature::canTakeDamage() {
    return !damageTimer->isActive();
}

bool CaveCreature::isAlive() const {
    return health > 0;
}

Caveman::Caveman(int x, int y, QObject* parent)
    : Enemies(x, y + 60, ":/Enemies/caveman.png", parent)
{
    QPixmap pix(":/Enemies/caveman.png");
    setPixmap(pix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    startX    = x;
    direction = 1;
    range     = 30;
    health    = 50;

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Caveman::patrol);
    movementTimer->start(70);

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}

Caveman::~Caveman() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer)   damageTimer->stop();
}

void Caveman::patrol() {
    if      (x() >= startX + range) direction = -1;
    else if (x() <= startX)         direction =  1;

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

bool Caveman::takeDamage(int dmg) {
    if (!canTakeDamage()) return false;

    health -= dmg;

    if (health <= 0) {
        health = 0;
        movementTimer->stop();

        // Switch to dead sprite
        QPixmap deadPixmap(":/Enemies/deadCaveMan.png");
        setPixmap(deadPixmap.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        return true;  // indicate caveman was killed
    }

    damageTimer->start(500);
    return false;
}

bool Caveman::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Caveman::isAlive() const {
    return health > 0;
}

Troll::Troll(int x, int y, QObject* parent)
    : Enemies(x, y + 60, ":/Enemies/caveMonster.png", parent)
{
    QPixmap pix(":/Enemies/caveMonster.png");
    setPixmap(pix.scaled(240, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    startX    = x;
    direction = 1;
    range     = 40;   // Troll moves wider than Caveman
    health    = 80;   // Troll has more health

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Troll::patrol);
    movementTimer->start(70);

    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}

Troll::~Troll() {
    if (movementTimer) movementTimer->stop();
    if (damageTimer)   damageTimer->stop();
}

void Troll::patrol() {
    if      (x() >= startX + range) direction = -1;
    else if (x() <= startX)         direction =  1;

    setPos(x() + 2 * direction, y());
    checkCollision(dynamic_cast<Player*>(scene()->focusItem()));
}

bool Troll::takeDamage(int dmg) {
    if (!canTakeDamage()) return false;
    health -= dmg;
    if (health <= 0) {
        movementTimer->stop();
        return true;
    }
    damageTimer->start(500);
    return false;
}

bool Troll::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Troll::isAlive() const {
    return health > 0;
}
