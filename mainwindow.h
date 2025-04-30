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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *bg1;
    QGraphicsPixmapItem *bg2;
    Player *player;
    Level *level;
    QGraphicsRectItem* healthOutline;

    QLabel *healthLabel;
    QLabel *levelText;
    QLabel *scoreText;
    QLabel *waterIcon;

    QGraphicsRectItem *healthBar;

    void setupGame();
    void updateGame();
    void updateHealthBar();
    void updateScore();
    void goToHome();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
