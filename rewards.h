#ifndef REWARDS_H
#define REWARDS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "Player.h"

            class Rewards : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Rewards(int x, int y, const QString& imagePath, int width, int height, QObject* parent = nullptr);
    virtual ~Rewards();
    virtual void checkCollision(Player* player) = 0;
protected:
    bool m_isCollected;
};

class WaterDroplet : public Rewards {
    Q_OBJECT
public:
    WaterDroplet(int x, int y, QObject* parent = nullptr);
    ~WaterDroplet() override;
    void checkCollision(Player* player) override;
};

class Apple : public Rewards {
    Q_OBJECT
public:
    Apple(int x, int y, QObject* parent = nullptr);
    ~Apple() override;
    void checkCollision(Player* player) override;
};

class Snowflake : public Rewards {
    Q_OBJECT
public:

    Snowflake(int x, int y, QObject* parent = nullptr);
    ~Snowflake() override;
    void checkCollision(Player* player) override;
};

class Earth : public Rewards {
    Q_OBJECT
public:
    Earth(int x, int y, QObject* parent = nullptr);
    ~Earth() override;
    void checkCollision(Player* player) override;
};


#endif // REWARDS_H
