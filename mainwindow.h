#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QLabel>
#include "Player.h"
#include "store.h"
#include "Level.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum GameState {
    Level1,
    Level2,
    Level3,
    Level4,
    GameOver
};

class Level;
class Level2;
class Level3;
class Level4;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showGameOver();
    void resetGameState();
    void updateScore();
    void updateGame();
    void switchToNextLevel();
    void setLevel(int level);
    void nextLevel();
    void updateLevelTimer(int seconds);
    Level* getLevel() const { return level; }
    Player* previousPlayer;
    static int globalCoins;
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *bg1;
    QGraphicsPixmapItem *bg2;
    Player *player;
    class Level *level;
    QGraphicsRectItem* healthOutline;
    QLabel *healthLabel;
    QLabel *levelText;
    QLabel *scoreText;
    QLabel *waterIcon;
    QLabel *AppleIcon;
    QLabel *FlakeIcon;
    QLabel *StarIcon;
    QLabel *DiamondIcon;
    QGraphicsRectItem *healthBar;
    void setupLevel1();
    void setupLevel2();
    void setupLevel3();
    void setupLevel4();
    void setupLevel5();
    void updateHealthBar();
    QLabel* levelTimerLabel;
    bool levelFinished = false;
    GameState currentState = Level1;
    void handlePlayerAttacks();
    void resetAfterGameOver();
    QLabel* coinIcon;
    QLabel* coinCountLabel;
    Store* store;
    QPushButton* storeButton;
    QLabel* speedBoostIcon;
    QLabel* shieldIcon;
    QLabel* superAttackIcon;
    bool gamePaused = false;
    void pauseGame();
    void resumeGame();
    QGraphicsTextItem* pauseText = nullptr;
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
private slots:
    void goToHome();
    void handleLevelTwoComplete();
    void handleLevelThreeComplete();
    void openStore();
signals:
    void levelOneCompleted();
    void backToHome();
    void levelTwoCompleted();
    void levelThreeCompleted();
    void levelFourCompleted();
    void levelFiveCompleted();
};
#endif // MAINWINDOW_H
