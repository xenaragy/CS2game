#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QObject>
#include <QPixmap>
#include <QTimer>

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

    void takeDamagePercent(float percent);  // for percentage-based damage


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

private:

    int health = 100;

    int coins;
    bool isJumping;
    bool isCrouching;
    bool isAttacking;
    bool isRight, isLeft;
    float totalDistanceMoved = 0.0f;
    QTimer* jumpTimer;        // Added jumpTimer declaration
    int velocityY;
    int groundY;
    int dropletsCollected = 0;



    int maxHealth = 100;


    QPixmap standingImage;
    QPixmap runningRightImage;
    QPixmap runningLeftImage;
    QPixmap crouchImage;
    QPixmap attackImage;
};

#endif
