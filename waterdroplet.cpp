#include "waterdroplet.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

WaterDroplet::WaterDroplet(int x, int y, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem()
    , m_isCollected(false)
{
    QPixmap dropletPixmap(":/Obstacles/waterdroplet.png");
    setPixmap(dropletPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    setData(0, "droplet");

}

WaterDroplet::~WaterDroplet()
{}

void WaterDroplet::checkCollision(Player* player)
{
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            player->incrementDroplets();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}

