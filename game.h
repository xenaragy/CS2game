// #ifndef GAME_H
// #define GAME_H

// #include <QGraphicsView>
// #include <QGraphicsScene>
// #include <QTimer>
// #include <QKeyEvent>
// #include <QLabel>
// #include "Player.h"
// #include "Level.h"

//          class Game : public QGraphicsView {
//     Q_OBJECT

// private:
//     QGraphicsScene* scene;
//     Player* player;
//     Level* currentLevel;
//     QTimer* gameTimer;
//     QLabel* healthLabel;
//     QLabel* levelLabel;
//     int score;
//     bool gameOver;

// public:
//     Game(QWidget* parent = nullptr);
//     ~Game();

//     void startGame();
//     void pauseGame();
//     void resumeGame();
//     void restartGame();
//     void gameOverScreen();
//     void updateUI();
//     void checkCollisions();
//     void advanceLevel();

// protected:
//     void keyPressEvent(QKeyEvent* event) override;
//     void keyReleaseEvent(QKeyEvent* event) override;

// private slots:
//     void update();
// };

// #endif // GAME_H
