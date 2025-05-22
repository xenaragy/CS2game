#ifndef LEVEL2_H
#define LEVEL2_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"

class Level2 : public Level {
public:
    Level2(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;
};

#endif // LEVEL2_H
