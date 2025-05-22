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
    setScale(0.8);  //scales down tiger to preference
    startX = x;    //store original x coordinate
    startY = y;     //store original y coordinate
    direction = 1; //initial movement to the right
    range = 300;  //range of its movement
    health = 100;  // starts with 100 health
    movementTimer = new QTimer(this); //timer that calls patrol every 10 ms to make sure tiger's is smooth
    connect(movementTimer, &QTimer::timeout, this, &Tiger::patrol);
    movementTimer->start(10);
    damageTimer = new QTimer(this); //this timer is needed to give time between every time a tiger can take damage
    damageTimer->setSingleShot(true);
    respawnTimer = new QTimer(this); //this timer calls respawn after the tiger dies
    respawnTimer->setSingleShot(true);
    connect(respawnTimer, &QTimer::timeout, this, &Tiger::respawn);
}

Tiger::~Tiger() {
    //stopping all timers
    if (movementTimer) {
        movementTimer->stop();
        delete movementTimer;
    }
    if (damageTimer) {
        damageTimer->stop();
        delete damageTimer;
    }
    if (respawnTimer) {
        respawnTimer->stop();
        delete respawnTimer;
    }
}

void Tiger::patrol() {
    // tiger moves back and forth within a given range
    if (x() >= startX + range) { //300 pixels to the right its starts moving left, and vice versa
        direction = -1;
    } else if (x() <= startX) {
        direction = 1;
    }
    setPos(x() + 2 * direction, y()); //moving 2 left or right depening on direction

    if (scene()) { //checking if tiger is touching the player
        Player* player = dynamic_cast<Player*>(scene()->focusItem()); //dynamic cast is used to check if it is actually a player
        if (player) {
            checkCollision(player);
        }
    }
}

bool Tiger::takeDamage(int damage) {
    if (!canTakeDamage()) return false;

    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2; // this ensures double damage if player has super attack
    }
    health -= damage;
    if (health <= 0) { //tiger is killed
        health = 0;
        movementTimer->stop();  // Stop everything
        setVisible(false);

        if (player) {
            if (player->hasSuperAttack()) {
                player->incrementCoins(3);
                Message* bonusMsg = new Message("+3 Coins: Super Kill Bonus!", 1500); //if killed by super attack you take 3 coins
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(1);
                Message* coinMsg = new Message("+1 Coin", 1000); //if killed normally 2 coin extra
                coinMsg->showMessage(scene(), 300, 200);
            }
        }

        respawnTimer->start(3000);  // the tger respawns every 3 seconds
        return true;
    }
    damageTimer->start(500); //0.5 seconds between each damage
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
    movementTimer->start(20); //tiger every 20 millisecconds
}

Penguin::Penguin(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)
{
    QPixmap pix(":/Enemies/penguin.png");
    setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));  //maintaining proportions and quality
    setPos(x, y);
    startX = x;
    direction = 1;
    range = 30;
    health = 50;
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Penguin::patrol);
    movementTimer->start(70);  //calls patrol function every 70 ms
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
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2;
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);

        if (player) {
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
    damageTimer->start(500);//0.5 seconds between each damage
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
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2;
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        if (player) {
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
    damageTimer->start(500);//0.5 seconds between each damage
    return false;
}

bool PolarBear::canTakeDamage() {
    return !damageTimer->isActive();
}

bool PolarBear::isAlive() const {
    return health > 0;
}

Alien::Alien(int x, int y, QObject* parent)
    : Enemies(x, y, "", parent)
{
    QPixmap pix(":/Enemies/alien.png");
    setPixmap(pix.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    startX = x;
    direction = 1;
    range = 30;
    health = 50;
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Alien::patrol);
    movementTimer->start(70);  //calls patrol function every 70 ms
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
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2;
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        if (player) {
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
    : Enemies(x, y, "", parent)
{
    QPixmap pix(":/Enemies/caveCreature.png");
    setPixmap(pix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    setZValue(10);
    startX = x;
    direction = 1;
    range = 30;
    health = 50;
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &CaveCreature::patrol);
    movementTimer->start(70);
    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
}

CaveCreature::~CaveCreature() {
    if (movementTimer) {
        movementTimer->stop();
        delete movementTimer;
        movementTimer = nullptr;
    }

    if (damageTimer) {
        damageTimer->stop();
        delete damageTimer;
        damageTimer = nullptr;
    }
}

void CaveCreature::patrol() {
    if (health <= 0) return;
    if (x() >= startX + range) direction = -1;
    else if (x() <= startX) direction = 1;
    setPos(x() + 2 * direction, y());
    if (scene()) {
        Player* player = dynamic_cast<Player*>(scene()->focusItem());
        if (player) {
            checkCollision(player);
        }
    }
}

bool CaveCreature::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    Player* player = nullptr;
    if (scene()) {
        player = dynamic_cast<Player*>(scene()->focusItem());
        if (player && player->hasSuperAttack()) {
            damage *= 2;
        }
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        if (player) {
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

bool CaveCreature::canTakeDamage() {
    return !damageTimer->isActive();
}

bool CaveCreature::isAlive() const {
    return health > 0;
}

Troll::Troll(int x, int y, QObject* parent)
    : Enemies(x, y + 60, ":/Enemies/caveMonster.png", parent)
{
    QPixmap pix(":/Enemies/caveMonster.png");
    setPixmap(pix.scaled(240, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    startX    = x;
    direction = 1;
    range     = 40;
    health    = 80;
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

bool Troll::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2;
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        if (player) {
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

bool Troll::canTakeDamage() {
    return !damageTimer->isActive();
}

bool Troll::isAlive() const {
    return health > 0;
}

Caveman::Caveman(int x, int y, QObject* parent)
    : Enemies(x, y, ":/Enemies/caveman.png", parent)
{
    QPixmap pix(":/Enemies/caveman.png");
    setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    startX = x;
    direction = -1;
    range = 170;
    health = 100;
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Caveman::patrol);
    movementTimer->start(50);
    damageTimer = new QTimer(this);
    damageTimer->setSingleShot(true);
    setZValue(10);
}

Caveman::~Caveman() {
    if (movementTimer) {
        movementTimer->stop();
        delete movementTimer;
    }
    if (damageTimer) {
        damageTimer->stop();
        delete damageTimer;
    }
}

void Caveman::patrol() {
    if (x() >= startX + range) {
        direction = -1;
    } else if (x() <= startX - range) {
        direction = 1;
    }
    setPos(x() + 3 * direction, y());
    if (scene()) {
        Player* player = dynamic_cast<Player*>(scene()->focusItem());
        if (player) {
            checkCollision(player);
        }
    }
}

bool Caveman::takeDamage(int damage) {
    if (!canTakeDamage()) return false;
    Player* player = dynamic_cast<Player*>(scene()->focusItem());
    if (player && player->hasSuperAttack()) {
        damage *= 2;
    }

    health -= damage;
    if (health <= 0) {
        health = 0;
        movementTimer->stop();
        setVisible(false);
        if (player) {
            if (player->hasSuperAttack()) {
                player->incrementCoins(5);
                Message* bonusMsg = new Message("+5 Coins: Super Kill Bonus!", 1500);
                bonusMsg->showMessage(scene(), 300, 200);
            } else {
                player->incrementCoins(3);
                Message* coinMsg = new Message("+3 Coins", 1000);
                coinMsg->showMessage(scene(), 300, 200);
            }
            player->heal(20);
            Message* healthMsg = new Message("+20 Health: Boss Defeated!", 1500);
            healthMsg->showMessage(scene(), 300, 250);
        }
        return true;
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
