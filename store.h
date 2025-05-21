#ifndef STORE_H
#define STORE_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include "Player.h"

class Store : public QDialog
{
    Q_OBJECT

public:
    Store(Player* player, QWidget* parent = nullptr);
    void updateDisplay();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void purchaseSpeedBoost();
    void purchaseShield();
    void purchaseSuperAttack();

private:
    Player* player;
    QPushButton* speedBoostButton = nullptr;
    QPushButton* shieldButton = nullptr;
    QPushButton* superAttackButton = nullptr;
    QLabel* coinsLabel = nullptr;

    void updateButtonStates();
    QLabel* coinIcon;
    QLabel* coinCountLabel;
};

#endif // STORE_H
