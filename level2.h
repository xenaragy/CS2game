#ifndef LEVEL2_H
#define LEVEL2_H

#include "Level.h"
#include "rewards.h"    // Use lowercase if that's the actual filename
#include "message.h"  // Use lowercase if that's the actual filename
#include "enemies.h"

class Level2 : public Level {
public:
    Level2(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;  // Added override keyword
};

#endif // LEVEL2_H
