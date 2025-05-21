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
    bool takeDamage(int damage);
    bool canTakeDamage();
    bool isAlive() const;

private slots:
    void patrol();
    void respawn();  // Add respawn slot

private:
    int startX;
    int startY;     // Add Y position for respawn
    int direction;
    int range;
    int health;
    QTimer* movementTimer;
    QTimer* damageTimer;
    QTimer* respawnTimer;  // Add respawn timer
};

class Penguin : public Enemies {
    Q_OBJECT
public:
    Penguin(int x, int y, QObject* parent = nullptr);
    ~Penguin();
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

class PolarBear : public Enemies {
    Q_OBJECT
public:
    PolarBear(int x, int y, QObject* parent = nullptr);
    ~PolarBear();
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

class Alien : public Enemies {
    Q_OBJECT
public:
    Alien(int x, int y, QObject* parent = nullptr);
    ~Alien();
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
class CaveCreature : public Enemies {
    Q_OBJECT
public:
    CaveCreature(int x, int y, QObject* parent = nullptr);
    ~CaveCreature();
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

class Caveman : public Enemies {
    Q_OBJECT
public:
    Caveman(int x, int y, QObject* parent = nullptr);
    ~Caveman() override;

    bool takeDamage(int damage) ;
    bool canTakeDamage() ;
    bool isAlive() const ;
    void patrol();
    void setx(int x){startX = x;}
    void setrange(int r){range = r;}
    int     health;
private:
    QTimer* movementTimer;
    QTimer* damageTimer;
    int     startX;
    int     direction;
    int     range;
};

class Troll : public Enemies {
    Q_OBJECT
public:
    Troll(int x, int y, QObject* parent = nullptr);
    ~Troll() override;

    bool takeDamage(int damage);
    bool canTakeDamage();
    bool isAlive() const;
    int     health;

private slots:
    void patrol();

private:
    QTimer* movementTimer;
    QTimer* damageTimer;
    int     startX;
    int     direction;
    int     range;
};

#endif // ENEMIES_H  // Make sure this line exists!
