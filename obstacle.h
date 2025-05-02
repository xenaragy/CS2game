#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "Player.h"
#include "Level.h"



 class Obstacle : public QGraphicsPixmapItem {

public:
    enum class ObstacleType { Platform, Hazard, Moving };
    Obstacle(const QPixmap& pixmap, int x, int y, ObstacleType type, int damage = 0, bool isMovable = false);
  //  void setType(ObstacleType type);
    ObstacleType getType() const;
    void setDamage(int damage);
    int getDamage() const;
    void setMovable(bool isMovable);
    bool isMovable() const;
    //void move();
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


#endif // OBSTACLE_H
