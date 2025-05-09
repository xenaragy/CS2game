#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QElapsedTimer>

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
    void incrementEarths();
    int getCollectedEarths() const;
    void resetEarths();

    // Utility
    bool canTakeDamage(int cooldownMs = 1000);
    void resetPlayer();
    bool isAttacking() { return attacking; }

    void applyGravity();
    QTimer* gravityTimer;
    bool isOnGround;
    float gravity = 1.0f;

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
    int collectedEarths = 0;

    QPixmap standingImage;
    QPixmap runningRightImage;
    QPixmap runningLeftImage;
    QPixmap crouchImage;
    QPixmap attackImage;
    QElapsedTimer damageTimer;
};

#endif // PLAYER_H
