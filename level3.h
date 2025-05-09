#ifndef LEVEL3_H
#define LEVEL3_H


#include "Level.h"
#include "rewards.h"    // Use lowercase if that's the actual filename
#include "message.h"  // Use lowercase if that's the actual filename
#include "enemies.h"

           class Level3 : public Level {
public:
    Level3(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;  // Added override keyword
};



#endif // LEVEL3_H
