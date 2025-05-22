#include "obstacle.h"
#include "Player.h"
#include "mainwindow.h"
#include "message.h"
#include <QTimer>

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
            player->takeDamage(40);
            player->takeDamagePercent(0.02f);
        }
    }
}

Snowman::Snowman(int x, int y, QGraphicsItem* parent)
    : Obstacle(QPixmap(":/images/snowman.png"), x, y, Obstacle::ObstacleType::Hazard, 10, false),
    scene(nullptr),
    shootTimer(new QTimer(this))
{
    setPos(x, y);
    connect(shootTimer, &QTimer::timeout, this, &Snowman::shootSnowball);
}

Snowman::~Snowman()
{
    if (shootTimer) {
        shootTimer->stop();
        delete shootTimer;
    }
}

void Snowman::startShooting(QGraphicsScene* scene)
{
    this->scene = scene;
    shootTimer->start(2000);  // Shoot every 2 seconds
}

void Snowman::shootSnowball()
{
    if (!scene)
        return;
    SnowBall* ball = new SnowBall(x(), y() + boundingRect().height()-130); //creating a new snowball from the snowman's position
    scene->addItem(ball);
}

QRectF Snowman::boundingRect() const
{
    return QRectF(0, 0, 90, 110);
}

void Snowman::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(boundingRect().toRect(), QPixmap(":/Obstacles/snowman.png"));
}

void Snowman::handleCollision(Player* player) {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(10);
            player->takeDamagePercent(0.02f);
        }
    }
}

SnowBall::SnowBall(int startX, int startY)
    : QGraphicsPixmapItem(QPixmap(":/Obstacles/snowball.png").scaled(30, 30)),
    speed(5)
{
    setPos(startX, startY);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &SnowBall::move);
    moveTimer->start(50); //every 50 ms a new ball is created
}

SnowBall::~SnowBall()
{
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
}

void SnowBall::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void SnowBall::move()
{
    setPos(x() - speed, y()); //move the ball to the left
    if (!scene() || x() > scene()->width() || x() < 0 || y() > scene()->height() || y() < 0) { //check if out of bounds
        // If out of bounds, remove it
        if (scene())
            scene()->removeItem(this);
        deleteLater();
        return;
    }
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Player* player = dynamic_cast<Player*>(item);
        if (player) {
            if (player->canTakeDamage(1000)) {
                player->takeDamage(10);
                player->takeDamagePercent(0.01f);
            }
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }
}

Asteroid::Asteroid(int startX, int startY)
    : QGraphicsPixmapItem(QPixmap(":/Obstacles/asteroid.png").scaled(90, 90)),
    speed(7)
{
    setPos(startX, startY);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Asteroid::move);
}

Asteroid::~Asteroid()
{
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
}

void Asteroid::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void Asteroid::move()
{
    // Move diagonally down-left
    setPos(x() - speed, y() + speed);

    // Check if asteroid is out of scene bounds
    if (!scene() || x() < -50 || y() > scene()->height()) {
        if (scene())
            scene()->removeItem(this);
        deleteLater();
        return;
    }
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Player* player = dynamic_cast<Player*>(item);
        if (player) {
            handleCollision(player);
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }
}

void Asteroid::handleCollision(Player* player)
{
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(50);
            player->takeDamagePercent(0.02f);
        }
    }
}

Spaceship::Spaceship(int x, int y)
    : QGraphicsPixmapItem(),
    moveTimer(new QTimer()),
    freezeTimer(new QTimer()),
    targetPlayer(nullptr),
    isActive(false),
    shadow(nullptr),
    playerFrozen(false),
    freezeCheckTimer(nullptr)
{
    QPixmap originalPixmap(":/Enemies/spaceship.png");
    QPixmap resizedPixmap = originalPixmap.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(resizedPixmap);
    setPos(x, y);
    setZValue(100);
    connect(moveTimer, &QTimer::timeout, this, &Spaceship::move);
    connect(freezeTimer, &QTimer::timeout, this, &Spaceship::releasePlayer);
    moveTimer->start(50);
}

Spaceship::~Spaceship()
{
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
        moveTimer = nullptr;
    }

    if (freezeTimer) {
        freezeTimer->stop();
        delete freezeTimer;
        freezeTimer = nullptr;
    }

    if (freezeCheckTimer) {
        freezeCheckTimer->stop();
        delete freezeCheckTimer;
        freezeCheckTimer = nullptr;
    }
    if (targetPlayer && playerFrozen) {
        if (targetPlayer->scene()) {
            targetPlayer->unfreezeFromSpaceship();
        }
    }

    targetPlayer = nullptr;
    playerFrozen = false;

    if (shadow) {
        if (scene()) {
            scene()->removeItem(shadow);
        }
        delete shadow;
        shadow = nullptr;
    }
}
void Spaceship::activate(Player* player)
{
    if (!player || isActive)
        return;
    targetPlayer = player;
    isActive = true;
    playerFrozen = false;

    // Create a very visible shadow
    shadow = new QGraphicsRectItem(0, 0, 60, 500);
    QLinearGradient gradient(30, 0, 30, 500); //fill the shadow and color it
    gradient.setColorAt(0, QColor(0, 255, 255, 200)); // Bright cyan at top
    gradient.setColorAt(1, QColor(0, 100, 255, 50));  // Fading blue at bottom
    // Apply the gradient to the shadow
    QBrush shadowBrush(gradient);
    static_cast<QGraphicsRectItem*>(shadow)->setBrush(shadowBrush);
    static_cast<QGraphicsRectItem*>(shadow)->setPen(Qt::NoPen);
    // Position shadow under the spaceship
    shadow->setPos(x() + boundingRect().width()/2 - 30, y() + boundingRect().height());
    shadow->setZValue(5); // Ensure it is visible
    scene()->addItem(shadow);
    // timer that constantly checks on player position
    if (!freezeCheckTimer) {
        freezeCheckTimer = new QTimer(this);
        connect(freezeCheckTimer, &QTimer::timeout, this, &Spaceship::checkPlayerInShadow);
        freezeCheckTimer->start(100); // Check 10 times per second
    }
    QTimer::singleShot(10000, this, &Spaceship::releasePlayer); //stays 10 seconds
}

void Spaceship::releasePlayer()
{
    if (targetPlayer && playerFrozen) {
        targetPlayer->unfreezeFromSpaceship();
        playerFrozen = false;
        Message* releaseMessage = new Message("UFO releasing control!", 1500);
        releaseMessage->showMessage(scene(), 300, 300);
    }

    if (freezeCheckTimer) {
        freezeCheckTimer->stop();
        delete freezeCheckTimer;
        freezeCheckTimer = nullptr;
    }

    if (shadow) {
        scene()->removeItem(shadow);
        delete shadow;
        shadow = nullptr;
    }

    // Fly away animation
    QTimer* departureTimer = new QTimer();
    connect(departureTimer, &QTimer::timeout, [this, departureTimer]() {
        setY(y() - 5);
        if (y() < -100) {
            if (scene())
                scene()->removeItem(this);
            departureTimer->stop();
            departureTimer->deleteLater();
            deleteLater();
        }
    });
    departureTimer->start(50);
    if (moveTimer) {
        moveTimer->stop();
    }

    isActive = false;
}
void Spaceship::checkPlayerInShadow()
{
    if (!isActive || !targetPlayer || !shadow)
        return;
    shadow->setPos(x() + boundingRect().width()/2 - 30, shadow->y());
    QRectF shadowRect = shadow->sceneBoundingRect();
    QRectF playerRect = targetPlayer->sceneBoundingRect();
    // Check if ANY part of the player intersects with the shadow
    bool isInShadow = shadowRect.intersects(playerRect);
    if (!isInShadow && targetPlayer->isMoving()) {
        // Get player's velocity
        qreal velX = 0;
        if (targetPlayer->isMovingRight()) velX = 25; // Use the actual movement speed
        if (targetPlayer->isMovingLeft()) velX = -25;
        // extedned player rect that accounts for movement
        QRectF extendedPlayerRect = playerRect;
        extendedPlayerRect.adjust(-velX, 0, velX, 0);
        // Check if this extended rect would intersect with player
        isInShadow = shadowRect.intersects(extendedPlayerRect);
    }

    if (isInShadow && !playerFrozen) {
        targetPlayer->freezeBySpaceship();
        playerFrozen = true;
        Message* freezeMessage = new Message("Caught in UFO Beam!", 1500);
        freezeMessage->showMessage(scene(), 300, 300);
    }
    else if (!isInShadow && playerFrozen) {
        targetPlayer->unfreezeFromSpaceship();
        playerFrozen = false;
        Message* unfreezeMessage = new Message("Escaped the Beam!", 1500);
        unfreezeMessage->showMessage(scene(), 300, 300);
    }
}

void Spaceship::move()
{
    if (!targetPlayer) {
        QVariant playerData = data(1);
        if (playerData.isValid()) {
            targetPlayer = static_cast<Player*>(playerData.value<void*>());
        } else if (scene()) {
            for (QGraphicsItem* item : scene()->items()) {
                Player* p = dynamic_cast<Player*>(item);
                if (p) {
                    targetPlayer = p;
                    break;
                }
            }
        }
    }

    if (targetPlayer) {
        qreal targetX = targetPlayer->x() + targetPlayer->boundingRect().width()/2 - boundingRect().width()/2;
        //move towards target position of player
        if (x() < targetX)
            setX(x() + 3);
        else if (x() > targetX)
            setX(x() - 3);
        // Check if directly above player and not active yet
        if (abs(x() - targetX) < 10 && !isActive) {
            activate(targetPlayer);
        }
    }
    // Update shadow position if active
    if (isActive && shadow) {
        // Center shadow beneath spaceship
        shadow->setPos(x() + boundingRect().width()/2 - 30, shadow->y());
    }
}

Bat::Bat(int startX, int startY)
    : QGraphicsPixmapItem(QPixmap(":/Enemies/bat4.png").scaled(150, 150)),
    speed(5)
{
    setPos(startX, startY);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Bat::move);
    moveTimer->start(50);
}

Bat::~Bat()
{
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
}

void Bat::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void Bat::move()
{
    setPos(x() - speed, y() + speed / 2);

    if (!scene() || x() < -100 || y() < -100) {
        if (scene()) scene()->removeItem(this);
        deleteLater();
        return;
    }

}

void Bat::handleCollision(Player* player)
{
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player && player->canTakeDamage(1000)) {
            player->takeDamage(10);
            player->takeDamagePercent(0.02f);
        }
    }
}
