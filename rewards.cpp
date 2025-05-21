#include "rewards.h"
#include <QGraphicsScene>

              Rewards::Rewards(int x, int y, const QString& imagePath, int width, int height, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(), m_isCollected(false)
{
    QPixmap rewardPixmap(imagePath);
    setPixmap(rewardPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
}

Rewards::~Rewards()
{}




WaterDroplet::WaterDroplet(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Obstacles/waterdroplet.png", 30, 30, parent)
{
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

Apple::Apple(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Rewards/apple.png", 30, 30, parent)
{
    setData(0, "apple");
}

Apple::~Apple()
{}

void Apple::checkCollision(Player* player)
{
    if (m_isCollected) return;

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            m_isCollected = true;
            player->incrementApples();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}

Snowflake::Snowflake(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Rewards/snowflake.png", 30, 30, parent)
{
    setData(0, "snowflake");
}

Snowflake::~Snowflake()
{}

void Snowflake::checkCollision(Player* player)
{
    if (m_isCollected) return;

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            m_isCollected = true;
            player->incrementApples();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}

Star::Star(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Rewards/star.png", 30, 30, parent)
{
    setData(0, "star");
}

Star::~Star()
{}

void Star::checkCollision(Player* player)
{
    if (m_isCollected) return;

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            m_isCollected = true;
            player->incrementStars();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}

Diamond::Diamond(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Rewards/diamond.png", 30, 30, parent)
{
    setData(0, "diamond");
}

Diamond::~Diamond()
{}

void Diamond::checkCollision(Player* player)
{
    if (m_isCollected) return;

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            m_isCollected = true;
            player->incrementDiamonds();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}
Coin::Coin(int x, int y, QObject* parent)
    : Rewards(x, y, ":/Rewards/coin.png", 30, 30, parent)
{
    setData(0, "coin");
}

Coin::~Coin()
{}

void Coin::checkCollision(Player* player)
{
    if (m_isCollected) return;

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (item == player) {
            m_isCollected = true;
            player->incrementCoins();
            scene()->removeItem(this);
            delete this;
            break;
        }
    }
}
