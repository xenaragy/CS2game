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
    attacking = true; // Use 'attacking' instead of 'isAttacking'
    setPixmap(attackImage);

    QTimer::singleShot(500, this, [this]() {
        attacking = false; // Also here
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

void Player::takeDamagePercent(float percent)
{
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

void Player::applyGravity() {
    // Don't apply gravity if we're in the middle of a jump
    if (jumpTimer->isActive()) {
        return;
    }

    // Check if we're on solid ground
    if (isOnGround) {
        bool stillOnPlatform = false;

        // If we're at the ground level, we're definitely on solid ground
        if (y() == groundY) {
            stillOnPlatform = true;
        } else {
            // Otherwise, check if we're on a platform
            QList<QGraphicsItem*> colliding = collidingItems();
            for (QGraphicsItem* item : colliding) {
                if (item == this) continue;  // Skip self-collision

                // Check for platform data tag
                if (item->data(0) == "platform") {
                    stillOnPlatform = true;
                    break;
                }
            }
        }

        // If we're not on anything solid, start falling
        if (!stillOnPlatform) {
            isOnGround = false;
            velocityY = 0;
        } else {
            // We're on solid ground, no need to apply gravity
            return;
        }
    }

    // Apply gravity if we're not on ground
    velocityY += gravity;
    if (velocityY > 15) velocityY = 15;  // Terminal velocity cap
    setPos(x(), y() + velocityY);

    // Check if we've landed on a platform
    QList<QGraphicsItem*> colliding = collidingItems();
    for (QGraphicsItem* item : colliding) {
        if (item == this) continue;

        if (item->data(0) == "platform" && velocityY > 0 && y() + boundingRect().height() >= item->y() - 2) {
            setY(item->y() - boundingRect().height());
            isOnGround = true;
            velocityY = 0;
            return;
        }
    }

    // Check if we've hit the ground level
    if (y() >= groundY) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }

    // Safety check - if we fall too far, reset position
    if (y() > 1000) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }
}
void Player::resetPlayer() {
    // Reset movement states
    isJumping = false;
    isCrouching = false;
    attacking = false;
    isRight = false;
    isLeft = false;
    isOnGround = true;

    // Reset velocity and position
    velocityY = 0;
    setPos(100, 400);  // Reset to the initial starting position for Level 2

    // Reset health and collected items if necessary
    health = 100;  // Or your max health value
    collectedApples = 0;  // Or reset other variables related to the game
}

