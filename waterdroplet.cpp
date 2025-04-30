#include "waterdroplet.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

WaterDroplet::WaterDroplet(int x, int y, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem()  // Initialize both base classes
    , m_isCollected(false)
{
    // Load and scale the water droplet image
    QPixmap dropletPixmap(":/Obstacles/waterdroplet.png");
    // Set pixmap with proper scaling
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
            // Increment player's droplet count
            player->incrementDroplets();
            // Remove the droplet from the scene
            scene()->removeItem(this);
            delete this;

            break;
        }
    }
}

