#ifndef WATERDROPLET_H
#define WATERDROPLET_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>

#include "Player.h"

class WaterDroplet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    WaterDroplet(int x, int y, QObject* parent = nullptr);
    ~WaterDroplet();

    void checkCollision(Player* player);

private:
    QTimer* animationTimer;
    int yOffset;
    int yDirection;
    bool m_isCollected;
};

#endif // WATERDROPLET_H
