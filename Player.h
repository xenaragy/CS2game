#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QElapsedTimer>
#include "message.h"

           class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player();
    ~Player();

    void moveForward();
    void moveBackward();
    void jump();
    void crouch();
    void attack();
    void setPosition(int x, int y);
    void takeDamage(int damage);
    void takeDamagePercent(float percent);
    void heal(int healthPoints);
    int getHealth() const;
    void setHealth(int health);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    bool isMovingRight() const { return isRight; }
    bool isMovingLeft() const { return isLeft; }
    void incrementCoins(int amount = 1);
    int getCoins() const;
    void setCoins(int amount);
    void spendCoins(int amount);
    void resetCoins();

    // Level 1
    void collectDroplet();
    void incrementDroplets();
    int getCollectedDroplets() const;
    void setDropletsCollected(int count);
    void resetDroplets();

    // Level 2
    void incrementApples();
    int getCollectedApples() const;
    void resetApples();

    // Level 3
    void incrementSnowflakes();
    int getCollectedSnowflakes() const;
    void resetSnowflakes();

    // Level 4
    void incrementStars();
    int getCollectedStars() const;
    void resetStars();
    bool isMoving() const { return isMovingRight() || isMovingLeft(); }

    // Level 5
    void incrementDiamonds();
    int getCollectedDiamonds() const;
    void resetDiamonds();
    static bool bossBattle;
    bool canMove = true;

    // Utility
    bool canTakeDamage(int cooldownMs = 1000);
    void resetPlayer();
    bool isAttacking() { return attacking; }

    void applyGravity();
    QTimer* gravityTimer;
    bool isOnGround;
    float gravity = 1.0f;
    void unfreezeFromSpaceship();
    void freezeBySpaceship();
    bool isFrozenBySpaceship() const;

    // Special ability methods
    void enableSpeedBoost(int seconds);
    void enableShield(int seconds);
    void enableSuperAttack(int seconds);
    // For super attack visual effects
    QGraphicsPixmapItem* swordFlame;
    void removeSwordFlame();
    void addSwordFlame();


    bool hasSpeedBoost() const;
    bool hasShield() const;
    bool hasSuperAttack() const;
    void toggleShield();
    void setPaused(bool isPaused) { paused = isPaused; }
    bool isPaused() const { return paused; }

private:
    int health = 100;
    int coins;
    bool isJumping;
    bool isCrouching;
    bool attacking;
    bool isRight, isLeft;
    float totalDistanceMoved = 0.0f;
    QTimer* jumpTimer;
    int velocityY;
    int groundY;
    int maxHealth = 100;
    int dropletsCollected = 0;
    int collectedApples = 0;
    int collectedSnowflakes = 0;
    int collectedStars = 0;
    int collectedDiamonds = 0;
    QPixmap standingImage;
    QPixmap runningRightImage;
    QPixmap runningLeftImage;
    QPixmap crouchImage;
    QPixmap attackImage;
    QPixmap speedRightImage;
    QPixmap speedJumpImage;
    QPixmap shieldImage;
    QPixmap shieldLeftImage;
    QPixmap speedLeftImage;
    bool paused = false;
    QElapsedTimer damageTimer;
    bool frozenBySpaceship = false;
    bool wasFocusable = true;

    // Special abilities
    bool speedBoostEnabled;
    bool shieldEnabled;
    bool superAttackEnabled;
    bool shieldDisplayed;

    // Special ability timers
    QTimer* speedBoostTimer;
    QTimer* shieldTimer;
    QTimer* superAttackTimer;

    // Original movement parameters
    int normalMoveSpeed;
    int speedBoostMoveSpeed;
    int normalJumpVelocity;
    int speedBoostJumpVelocity;
};

#endif // PLAYER_H
