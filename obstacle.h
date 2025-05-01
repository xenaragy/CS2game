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

    // Setters and getters for obstacle properties
    void setType(ObstacleType type);
    ObstacleType getType() const;

    void setDamage(int damage);
    int getDamage() const;

    void setMovable(bool isMovable);
    bool isMovable() const;

    void move(); // For moving obstacles

    // Handles collisions with the player
    virtual void handleCollision(Player* player);

private:
    ObstacleType obstacleType;
    int damage;
    bool movable;  // Whether the obstacle is movable
    int speed; // Speed for moving obstacles (e.g., left-right)
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

    void setLevel(Level* level);  // Declare setLevel method


private:
    Level* level;  // Store the reference to Level object
};


#endif // OBSTACLE_H
