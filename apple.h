// Apple.h
#ifndef APPLE_H
#define APPLE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "Player.h"

class Apple : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Apple(int x, int y, QObject* parent = nullptr);
    ~Apple();
    void checkCollision(Player* player);

private:
    bool m_isCollected;
};

#endif // APPLE_H
