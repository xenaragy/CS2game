#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "Player.h"
#include "Level.h"
#include <QObject>
#include <QTimer>



class Obstacle : public QGraphicsPixmapItem {

public:
    enum class ObstacleType { Platform, Hazard, Moving };
    Obstacle(const QPixmap& pixmap, int x, int y, ObstacleType type, int damage = 0, bool isMovable = false);
    ObstacleType getType() const;
    void setDamage(int damage);
    int getDamage() const;
    void setMovable(bool isMovable);
    bool isMovable() const;
    virtual void handleCollision(Player* player) = 0;

private:
    ObstacleType obstacleType;
    int damage;
    bool movable;
    int speed;
};

class Fire : public Obstacle {
public:
    Fire(int x, int y);
    void handleCollision(Player* player) override;
};

class Cactus : public Obstacle {
public:
    Cactus(int x, int y);
    void handleCollision(Player* player) override;
};

class Quicksand : public Obstacle {

public:
    Quicksand(int x, int y);
    void handleCollision(Player* player) override;
    void setLevel(Level* level);

private:
    Level* level;
};

class Mushroom : public Obstacle {
public:
    Mushroom(int x, int y);
    void handleCollision(Player* player) override;
};

class Waterpond : public Obstacle {

public:
    Waterpond(int x, int y);
    void handleCollision(Player* player) override;
    void setLevel(Level* level);

private:
    Level* level;
};


class Snowman : public QObject, public Obstacle {
    Q_OBJECT

public:
    explicit Snowman(int x, int y, QGraphicsItem* parent = nullptr);
    ~Snowman();

    void startShooting(QGraphicsScene* scene);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private slots:
    void shootSnowball();

private:
    QGraphicsScene* scene;
    QTimer* shootTimer;
    void handleCollision(Player* player) override;
};

class SnowBall : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    SnowBall(int startX, int startY);
    ~SnowBall();

    void setSpeed(int newSpeed);

private slots:
    void move();

private:
    int speed;
    QTimer* moveTimer;
};

class Asteroid : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Asteroid(int startX, int startY);
    ~Asteroid();

    void setSpeed(int newSpeed);
    void handleCollision(Player* player);
    QTimer* moveTimer;

private slots:
    void move();

private:
    int speed;

};

class Spaceship : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Spaceship(int startX, int startY);
    ~Spaceship();

    void activate(Player* player);

private slots:
    void move();
    void releasePlayer();
    void checkPlayerInShadow();
private:

    QTimer* moveTimer;
    QTimer* freezeTimer;
    Player* targetPlayer;
    bool isActive;
    QGraphicsItem* shadow;
    bool playerFrozen;
    QTimer* freezeCheckTimer;
};

class Bat : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Bat(int startX, int startY);
    ~Bat();

    void setSpeed(int newSpeed);
    void handleCollision(Player* player);
    QTimer* moveTimer;

private slots:
    void move();

private:
    int speed;
};

#endif // OBSTACLE_H
