#ifndef LEVEL4_H
#define LEVEL4_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"
#include <QTimer>
#include <QGraphicsTextItem>

class Level4 : public Level {
    Q_OBJECT

public:
    Level4(QGraphicsScene* scene, Player* p1);
    ~Level4();

    void setupLevel() override;
    void resetLevel() override;
    void addEnemies();

    // Asteroid related methods
    void startAsteroidAttack();
    void spawnAsteroid();
    void checkAsteroidAttack();

    // Timer control methods
    void stopLevelTimer() {
        if (levelTimer && levelTimer->isActive()) {
            levelTimer->stop();
        }
        remainingTime = 90; // Reset to initial time
    }

    void pauseTimer() {
        if (levelTimer && levelTimer->isActive()) {
            levelTimer->stop();
            timerPaused = true;
        }
    }

    void resumeTimer() {
        if (levelTimer && !levelTimer->isActive() && timerPaused) {
            levelTimer->start(1000);
            timerPaused = false;
        }
    }

    void stopAllTimers();

private:
    // Timer related variables (declared once only)
    QTimer* asteroidTimer;
    QTimer* levelTimer;
    QTimer* spaceshipTimer;
    QGraphicsTextItem* timerDisplay;

    // State variables
    bool asteroidAttackActive;
    int remainingTime;  // in seconds
    bool timerPaused;

    // Private methods
    void spawnSpaceship();
    void updateTimerDisplay();
};

#endif // LEVEL4_H
