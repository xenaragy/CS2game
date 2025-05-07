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
    if (m_isCollected) return;  // Skip if already collected

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            // Mark as collected and increment the player's count
            player->incrementDroplets();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}
