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


void Waterpond::setLevel(Level* l) {
    level = l;
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
    SnowBall* ball = new SnowBall(x(), y() + boundingRect().height()-130);
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
    moveTimer->start(50);
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
    // Move the snowball horizontally (you can adjust direction as needed)
    setPos(x() - speed, y());
    // Check if snowball is out of scene bounds
    if (!scene() || x() > scene()->width() || x() < 0 || y() > scene()->height() || y() < 0) {
        // If out of bounds, remove it from the scene and delete it
        if (scene())
            scene()->removeItem(this);
        deleteLater();
        return;
    }

    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Player* player = dynamic_cast<Player*>(item);
        if (player) {
            // Handle player collision
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
    speed(7) // Moderate diagonal speed
{
    setPos(startX, startY);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Asteroid::move);
    // Timer starts when triggered, not on creation
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

    // Check for collision with player
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
    // Load original spaceship image and resize it
    QPixmap originalPixmap(":/Enemies/spaceship.png");
    QPixmap resizedPixmap = originalPixmap.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(resizedPixmap);

    // Position it
    setPos(x, y);

    // Make sure it's visible
    setZValue(100);

    // Connect timers
    connect(moveTimer, &QTimer::timeout, this, &Spaceship::move);
    connect(freezeTimer, &QTimer::timeout, this, &Spaceship::releasePlayer);

    // Start movement timer
    moveTimer->start(50);
}

Spaceship::~Spaceship()
{
    // Stop timers first to prevent any callbacks
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

    // Make sure to release player if destroyed while active
    if (targetPlayer && playerFrozen) {
        // Check if player is still valid by checking if it's in a scene
        if (targetPlayer->scene()) {
            // Only then try to unfreeze the player
            targetPlayer->unfreezeFromSpaceship();
        }
    }

    // Clear the player reference
    targetPlayer = nullptr;
    playerFrozen = false;

    // Clean up shadow
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
    playerFrozen = false; // Reset frozen state

    qDebug() << "Spaceship activated, tracking player position";

    // Create a very visible shadow using QGraphicsRectItem
    shadow = new QGraphicsRectItem(0, 0, 60, 500);

    // Create a nice gradient fill
    QLinearGradient gradient(30, 0, 30, 500);
    gradient.setColorAt(0, QColor(0, 255, 255, 200)); // Bright cyan at top
    gradient.setColorAt(1, QColor(0, 100, 255, 50));  // Fading blue at bottom

    // Apply the gradient to the shadow
    QBrush shadowBrush(gradient);
    static_cast<QGraphicsRectItem*>(shadow)->setBrush(shadowBrush);
    static_cast<QGraphicsRectItem*>(shadow)->setPen(Qt::NoPen);

    // Position shadow under the spaceship
    shadow->setPos(x() + boundingRect().width()/2 - 30, y() + boundingRect().height());
    shadow->setZValue(5); // Ensure visibility

    scene()->addItem(shadow);

    // Start a timer to check player position frequently
    if (!freezeCheckTimer) {
        freezeCheckTimer = new QTimer(this);
        connect(freezeCheckTimer, &QTimer::timeout, this, &Spaceship::checkPlayerInShadow);
        freezeCheckTimer->start(100); // Check 10 times per second
    }

    // Set up departure after 10 seconds
    QTimer::singleShot(10000, this, &Spaceship::releasePlayer);
}

void Spaceship::releasePlayer()
{
    // Explicit unfreeze for safety
    if (targetPlayer && playerFrozen) {
        targetPlayer->unfreezeFromSpaceship();
        playerFrozen = false;

        // Visual feedback
        Message* releaseMessage = new Message("UFO releasing control!", 1500);
        releaseMessage->showMessage(scene(), 300, 300);
    }

    // Stop freeze checker
    if (freezeCheckTimer) {
        freezeCheckTimer->stop();
        delete freezeCheckTimer;
        freezeCheckTimer = nullptr;
    }

    // Remove shadow
    if (shadow) {
        scene()->removeItem(shadow);
        delete shadow;
        shadow = nullptr;
    }

    // Fly away
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

    // Stop the normal movement timer
    if (moveTimer) {
        moveTimer->stop();
    }

    isActive = false;
}
void Spaceship::checkPlayerInShadow()
{
    if (!isActive || !targetPlayer || !shadow)
        return;

    // Update shadow position with spaceship
    shadow->setPos(x() + boundingRect().width()/2 - 30, shadow->y());

    // Get actual screen positions
    QRectF shadowRect = shadow->sceneBoundingRect();
    QRectF playerRect = targetPlayer->sceneBoundingRect();

    // Check if ANY part of the player intersects with the beam
    bool isInShadow = shadowRect.intersects(playerRect);

    // Use player velocity to predict the next position
    if (!isInShadow && targetPlayer->isMoving()) {
        // Get player's velocity
        qreal velX = 0;
        if (targetPlayer->isMovingRight()) velX = 25; // Use the actual movement speed
        if (targetPlayer->isMovingLeft()) velX = -25;

        // Create a slightly expanded player rect that accounts for movement
        QRectF extendedPlayerRect = playerRect;
        extendedPlayerRect.adjust(-velX, 0, velX, 0);

        // Check if this extended rect would intersect
        isInShadow = shadowRect.intersects(extendedPlayerRect);
        qDebug() << "Extended check for moving player:" << isInShadow;
    }

    // Freeze player if in shadow and not already frozen
    if (isInShadow && !playerFrozen) {
        targetPlayer->freezeBySpaceship();
        playerFrozen = true;

        // Visual effect or message
        Message* freezeMessage = new Message("Caught in UFO Beam!", 1500);
        freezeMessage->showMessage(scene(), 300, 300);
    }
    // Unfreeze player if out of shadow and frozen
    else if (!isInShadow && playerFrozen) {
        targetPlayer->unfreezeFromSpaceship();
        playerFrozen = false;

        // Visual effect or message
        Message* unfreezeMessage = new Message("Escaped the Beam!", 1500);
        unfreezeMessage->showMessage(scene(), 300, 300);
    }
}

void Spaceship::move()
{
    // Find player if not already set
    if (!targetPlayer) {
        QVariant playerData = data(1);
        if (playerData.isValid()) {
            targetPlayer = static_cast<Player*>(playerData.value<void*>());
        } else if (scene()) {
            // Try to find player in scene
            for (QGraphicsItem* item : scene()->items()) {
                Player* p = dynamic_cast<Player*>(item);
                if (p) {
                    targetPlayer = p;
                    break;
                }
            }
        }
    }

    // Move horizontally to track the player's position
    if (targetPlayer) {
        // Calculate target X position (center above player)
        qreal targetX = targetPlayer->x() + targetPlayer->boundingRect().width()/2 - boundingRect().width()/2;

        // Move towards target position
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
    : QGraphicsPixmapItem(QPixmap(":/Enemies/bat4.png").scaled(100, 100)),
    speed(5)
{
    setPos(startX, startY);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Bat::move);
    moveTimer->start(50); // start when created
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
    setPos(x() - speed, y() + speed / 2);  // Down-left

    if (!scene() || x() < -100 || y() < -100) {
        if (scene()) scene()->removeItem(this);
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

void Bat::handleCollision(Player* player)
{
    if (player && player->canTakeDamage(500)) {
        player->takeDamage(10);               // Light fixed damage
        player->takeDamagePercent(0.05f);     // Or percent-based damage (optional)
    }
}

