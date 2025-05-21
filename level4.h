#ifndef LEVEL4_H
#define LEVEL4_H

#include "Level.h"
#include "rewards.h"
#include "message.h"
#include "enemies.h"

class Level4 : public Level {
public:
    Level4(QGraphicsScene* scene, Player* p1);
    ~Level4();
    void setupLevel() override;
    void addEnemies();
    void resetLevel() override;
    void startAsteroidAttack();
    void spawnAsteroid();
    QTimer* asteroidTimer;
    bool asteroidAttackActive;
    void checkAsteroidAttack();
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
    void stopAllTimers() {
        // Stop level timer
        if (levelTimer && levelTimer->isActive()) {
            levelTimer->stop();
        }

        // Stop asteroid timer
        if (asteroidTimer && asteroidTimer->isActive()) {
            asteroidTimer->stop();
        }

        // Stop spaceship timer
        if (spaceshipTimer && spaceshipTimer->isActive()) {
            spaceshipTimer->stop();
        }

        // Stop any other potential timers used in level 4

        // Set remaining time to ensure proper reset
        remainingTime = 90;
    }

private:
    // Timer related variables
    QTimer* levelTimer;
    int remainingTime;  // in seconds
    QGraphicsTextItem* timerDisplay;
    // Spaceship related variables
    QTimer* spaceshipTimer;
    void spawnSpaceship();
    bool timerPaused;

    // Update function
    void updateTimerDisplay();
};
#endif // LEVEL4_H
