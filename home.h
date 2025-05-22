#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void startLevel1();
    void unlockLevel2();
    void startLevel2();
    void unlockLevel3();
    void startLevel3();
    void unlockLevel4();
    void startLevel4();
    void unlockLevel5();
    void startLevel5();
    void startLevel(int levelNumber);

signals:
    void levelOneCompleted();
    void backToHome();
    void levelTwoCompleted();
    void levelThreeCompleted();
    void levelFourCompleted();
    void levelFiveCompleted();

private:
    void setupUI();
    QLabel* backgroundLabel;
    QPushButton* level2Btn = nullptr;
    QPushButton* level3Btn = nullptr;
    QPushButton* level4Btn = nullptr;
    QPushButton* level5Btn = nullptr;
    QLabel* level2Text;
    QLabel* level3Text;
    QLabel* level4Text;
    QLabel* level5Text;
    MainWindow* gameWindow = nullptr;
};

#endif // HOME_H
