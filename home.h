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


signals:
    void levelOneCompleted();

private:
    void setupUI();

    QLabel* backgroundLabel;
    QPushButton* level2Btn;
    MainWindow* gameWindow = nullptr;
};

#endif // HOME_H
