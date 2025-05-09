#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QLabel>
#include "Player.h"
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
    QGraphicsRectItem *healthBar;
    void setupLevel1();
    void setupLevel2();
    void setupLevel3();
    void setupLevel4();
    void updateHealthBar();
    bool levelFinished = false;
    GameState currentState = Level1;
private slots:
    void goToHome();
    void handleLevelTwoComplete();
    void handleLevelThreeComplete();

signals:
    void levelOneCompleted();
    void backToHome();
    void levelTwoCompleted();
    void levelThreeCompleted();
    void levelFourCompleted();

};
#endif // MAINWINDOW_H
