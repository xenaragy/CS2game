#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include "Player.h"
#include "rewards.h"

class Level : public QObject{
        Q_OBJECT
protected:
    QGraphicsScene* scene;
    QVector<QGraphicsItem*> obstacles;
    QVector<QGraphicsItem*> enemies;
    QVector<WaterDroplet*> waterDroplets;
    Player* p1;
    int levelNumber;
    const int brickWidth = 128;
    const int brickHeight = 64;
    const int screenWidth = 800;
public:
    Level(int number, QGraphicsScene* scene, Player* p1);
    virtual void setupLevel();
    void addEnemy(QGraphicsItem* enemy);
    virtual void resetLevel();
    void nextLevel();
    int getLevelNumber() const;
    void addObstacle(QGraphicsItem* obstacle);
    void addWaterDroplet(WaterDroplet* droplet);
    int getTotalDroplets() const;
    QGraphicsPixmapItem* createBrick(int x, int y);
};

#endif // LEVEL_H
