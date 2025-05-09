#include "Player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QDebug>

             Player::Player() : health(100), coins(0), isJumping(false), isCrouching(false),
attacking(false), isRight(false), isLeft(false), isOnGround(true) {
    QPixmap standingPixmap(":/Character/playerstanding.png");
    QPixmap runningRightPixmap(":/Character/runningright.png");
    QPixmap runningLeftPixmap(":/Character/runningleft.png");
    QPixmap crouchPixmap(":/Character/playercrouch.png");
    QPixmap attackPixmap(":/Character/playersword.png");

    health = maxHealth;

    int width = 150;
    int height = 200;
    standingImage = standingPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    runningRightImage = runningRightPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    runningLeftImage = runningLeftPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    crouchImage = crouchPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    attackImage = attackPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(standingImage);
    setPos(100, 400);
    dropletsCollected = 0;

    velocityY = 0;
    jumpTimer = new QTimer(this);
    groundY = y();

    connect(jumpTimer, &QTimer::timeout, this, [this]() {
        setPos(x(), y() - velocityY);
        velocityY -= 1;
        if (velocityY <= 0) {
            jumpTimer->stop();
        }
    });
    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Player::applyGravity);
    gravityTimer->start(25);
}

Player::~Player() {}

void Player::moveForward() {
    isRight = true;
    setPixmap(runningRightImage);
    if (x() < 600) setPos(x() + 25, y());
}

void Player::moveBackward() {
    isLeft = true;
    setPixmap(runningLeftImage);
    if (x() > 100) setPos(x() - 25, y());
}

void Player::jump() {
    if (isOnGround) {
        isJumping = true;
        isOnGround = false;
        velocityY = 15;
        jumpTimer->start(20);
    }
}

void Player::crouch() {
    isCrouching = true;
    setPixmap(crouchImage);
}

void Player::attack() {
    attacking = true;
    setPixmap(attackImage);

    QTimer::singleShot(500, this, [this]() {
        attacking = false;
        setPixmap(standingImage);
    });
}

void Player::setPosition(int x, int y) {
    setPos(x, y);
    groundY = y;
    isOnGround = true;
    velocityY = 0;

    if (jumpTimer->isActive()) {
        isJumping = false;
        jumpTimer->stop();
    }
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::takeDamagePercent(float percent) {
    int dmg = static_cast<int>(percent * 100);
    health -= dmg;
    if (health < 0) health = 0;
}

void Player::heal(int healthPoints) {
    health += healthPoints;
    if (health > 100) health = 100;
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int h) {
    health = h;
}

void Player::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Right) moveForward();
    else if (event->key() == Qt::Key_Left) moveBackward();
    else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) jump();
    else if (event->key() == Qt::Key_Down) crouch();
    else if (event->key() == Qt::Key_A) attack();
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_A ||
        event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        isCrouching = false;
        attacking = false;
        isRight = false;
        isLeft = false;
        setPixmap(standingImage);
    }
}

void Player::incrementDroplets() {
    dropletsCollected++;
}

int Player::getCollectedDroplets() const {
    return dropletsCollected;
}

void Player::setDropletsCollected(int count) {
    dropletsCollected = count;
}

bool Player::canTakeDamage(int cooldownMs) {
    if (!damageTimer.isValid()) {
        damageTimer.start();
        return true;
    }

    if (damageTimer.elapsed() >= cooldownMs) {
        damageTimer.restart();
        return true;
    }

    return false;
}

void Player::incrementApples() {
    collectedApples++;
}

int Player::getCollectedApples() const {
    return collectedApples;
}

void Player::resetDroplets() {
    dropletsCollected = 0;
}

void Player::resetApples() {
    collectedApples = 0;
}

void Player::incrementSnowflakes() {
    collectedSnowflakes++;
}

int Player::getCollectedSnowflakes() const {
    return collectedApples;
}

void Player::resetSnowflakes() {
    collectedSnowflakes = 0;
}

void Player::applyGravity() {
    if (jumpTimer->isActive()) {
        return;
    }

    if (isOnGround) {
        bool stillOnPlatform = false;

        if (y() == groundY) {
            stillOnPlatform = true;
        } else {
            QList<QGraphicsItem*> colliding = collidingItems();
            for (QGraphicsItem* item : colliding) {
                if (item == this) continue;
                if (item->data(0) == "platform") {
                    stillOnPlatform = true;
                    break;
                }
            }
        }

        if (!stillOnPlatform) {
            isOnGround = false;
            velocityY = 0;
        } else {
            return;
        }
    }

    velocityY += gravity;
    if (velocityY > 15) velocityY = 15;
    setPos(x(), y() + velocityY);

    QList<QGraphicsItem*> colliding = collidingItems();
    for (QGraphicsItem* item : colliding) {
        if (item == this) continue;
        if (item->data(0) == "platform" && velocityY > 0 && y() + boundingRect().height() >= item->y() - 2) {
           // setY(item->y() - boundingRect().height());
            isOnGround = true;
            velocityY = 0;
            return;
        }
    }

    if (y() >= groundY) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }

    if (y() > 1000) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }
}

void Player::resetPlayer() {
    isJumping = false;
    isCrouching = false;
    attacking = false;
    isRight = false;
    isLeft = false;
    isOnGround = true;

    velocityY = 0;
    setPos(100, 400);

    health = 100;
    collectedApples = 0;
    collectedSnowflakes = 0;
    dropletsCollected = 0;
}
