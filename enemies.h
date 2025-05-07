#ifndef ENEMIES_H
#define ENEMIES_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "Player.h"

class Enemies : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Enemies(int x, int y, const QString& spritePath, QObject* parent = nullptr);
    virtual ~Enemies();
    virtual void move() = 0;
    virtual void checkCollision(Player* player);
protected:
    int startX;
    int direction;
    int range;
};

class Tiger : public Enemies {
    Q_OBJECT
public:
    Tiger(int x, int y, QObject* parent = nullptr);
    ~Tiger();
    void move() override;
    bool takeDamage(int damage);
    bool isAlive() const;

private slots:
    void patrol();

private:
    QTimer* movementTimer;
    QTimer* damageTimer;
    int health;
    bool canTakeDamage();
};

#endif // ENEMIES_H  // Make sure this line exists!
