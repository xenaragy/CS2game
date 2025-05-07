#include "apple.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

Apple::Apple(int x, int y, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem()
    , m_isCollected(false)
{
    QPixmap applePixmap(":/Rewards/apple.png");
    setPixmap(applePixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    setData(0, "apple");
}

Apple::~Apple()
{}

void Apple::checkCollision(Player* player)
{
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            player->incrementApples();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}

