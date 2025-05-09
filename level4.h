#ifndef LEVEL4_H
#define LEVEL4_H

#include "Level.h"
#include "rewards.h"    // Use lowercase if that's the actual filename
#include "message.h"  // Use lowercase if that's the actual filename
#include "enemies.h"

           class Level4 : public Level {
public:
    Level4(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;  // Added override keyword
};
#endif // LEVEL4_H
