#ifndef LEVEL3_H
#define LEVEL3_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"

class Level3 : public Level {
public:
    Level3(QGraphicsScene* scene, Player* p1);
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;

};



#endif // LEVEL3_H
