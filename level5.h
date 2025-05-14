#ifndef LEVEL5_H
#define LEVEL5_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"

class Level5 : public Level {
public:
    Level5(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;
};
#endif // LEVEL5_H
