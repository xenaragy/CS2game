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
    void collectDroplet();
    void incrementDroplets();
    int getCollectedDroplets() const;
    void setDropletsCollected(int count);
    bool canTakeDamage(int cooldownMs = 1000);
    void resetDroplets();
    void applyGravity();
    QTimer* gravityTimer;
    bool isOnGround;
    float gravity = 1.0f;
    void incrementApples();
    int getCollectedApples() const;
    void resetPlayer();
    bool isAttacking() { return attacking; }
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
    int dropletsCollected = 0;
    int maxHealth = 100;
    QPixmap standingImage;
    QPixmap runningRightImage;
    QPixmap runningLeftImage;
    QPixmap crouchImage;
    QPixmap attackImage;
    QElapsedTimer damageTimer;
    int collectedApples = 0;

};

#endif
