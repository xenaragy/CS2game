
#ifndef LEVEL5_H
#define LEVEL5_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"
#include <QtCore/qpropertyanimation.h>
#include <QTimer>
#include <QGraphicsItem>
#include <QTransform>

class Level5 : public Level {
    Q_OBJECT

public:
    explicit Level5(QGraphicsScene* scene, Player* p1);

    // Overrides
    void setupLevel() override;
    void resetLevel() override;
    void update();

    // Custom methods
    void addEnemies();
    void checkCavemanAttack();
    void startCavemanAttack();
    void checkTrollAttack();
    void startTrollAttack();
    void checkBatAttack();
    void startBatAttack();
    void spawnBat();
    void startTrollBattle();
    void killTroll();

private slots:
    //void autoFight(); // Connected to fight timer
private:
    QTimer* batTimer;
    bool batAttackActive;
    bool trollAttackActive;
    bool batSpawned = false;


    // State flags
    bool cavemanAttackActive = false;
    bool battleStarted = false;
    bool cinematicPlaying = false;

    // Pointers to dynamic objects
    Caveman* caveman = nullptr;
    Troll* troll = nullptr;
    QTimer* fightTimer = nullptr;

    // Animation and view transform
    QPropertyAnimation* zoomInAnim = nullptr;
    QPropertyAnimation* zoomOutAnim = nullptr;
    QTransform origTransform;

    // Threshold for starting caveman and troll attack
    const int diamondsThreshold = 30;
    const int trollThreshold = 20;

    // Obstacles list for management
    QList<QGraphicsItem*> obstacles;
};

#endif // LEVEL5_H
