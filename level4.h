#ifndef LEVEL4_H
#define LEVEL4_H

#include "Level.h"
#include <QTimer>
#include <QGraphicsTextItem>

class Level4 : public Level
{
    Q_OBJECT

public:
    Level4(QGraphicsScene* scene, Player* p1);
    ~Level4();

    void setupLevel() override;
    void resetLevel() override;
    void addEnemies() ;
    void checkAsteroidAttack();
    void startAsteroidAttack();
    void stopAllTimers();
    void initializeTimers();
    void cleanupAllTimerDisplays();

public slots:
    void updateTimerDisplay();
    void spawnAsteroid();
    void spawnSpaceship();
    void pauseTimer();
    void resumeTimer();

private:
    QTimer* levelTimer;
    QTimer* asteroidTimer;
    QTimer* spaceshipTimer;
    QGraphicsTextItem* timerDisplay;
    int remainingTime;
    bool timerPaused;
    bool asteroidAttackActive;
    bool timersInitialized;
};

#endif // LEVEL4_H
