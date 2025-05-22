#include "store.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "message.h"
#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QApplication>

Store::Store(Player* player, QWidget* parent)
    : QDialog(parent), player(player)
{
    speedBoostButton = nullptr;
    shieldButton = nullptr;
    superAttackButton = nullptr;
    coinCountLabel = nullptr;
    coinsLabel = nullptr;

    setWindowTitle("Adventure Shop");
    setFixedSize(800, 600);
    setModal(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // No margins
    setLayout(mainLayout);

    QWidget* buttonPanel = new QWidget(this);
    buttonPanel->setGeometry(0, 0, 800, 600);
    buttonPanel->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buttonPanel->show();
    buttonPanel->raise(); // Ensure it's on top

    // Set background image
    QPixmap backgroundPixmap(":/backgrounds/shopBackground.png");

    // Safely check if the image was loaded
    if (!backgroundPixmap.isNull()) {
        // Scale the pixmap to fit the window size
        QPixmap scaledBackground = backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, scaledBackground);
        setAutoFillBackground(true);
        setPalette(palette);

        // Background label on a lower z-order
        QLabel* backgroundLabel = new QLabel(this);
        backgroundLabel->setPixmap(scaledBackground);
        backgroundLabel->setGeometry(0, 0, 800, 600);
        backgroundLabel->lower();
    }

    QLabel* coinIcon = new QLabel(buttonPanel);
    QPixmap coinPixmap(":/Rewards/coin.png");
    if (!coinPixmap.isNull()) {
        coinIcon->setPixmap(coinPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        coinIcon->setGeometry(710, 50, 40, 40);
    }

    coinsLabel = new QLabel(buttonPanel);
    if (player) {
        coinsLabel->setText(QString::number(player->getCoins()));
    } else {
        coinsLabel->setText("0");
    }
    coinsLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");
    coinsLabel->setGeometry(760, 50, 60, 40);

    int buttonWidth = 170;
    int buttonHeight = 55;

    int leftButtonX = 75;
    int middleButtonX = 315;
    int rightButtonX = 565;
    int buttonY = 400;

    speedBoostButton = new QPushButton("5 COINS", buttonPanel);
    if (speedBoostButton) {
        speedBoostButton->setGeometry(leftButtonX, buttonY, buttonWidth, buttonHeight);
        speedBoostButton->setStyleSheet(
            "QPushButton { background-color: #32CD32; color: white; " // Lime Green
            "border: 2px solid #006400; border-radius: 10px; font-size: 18px; font-weight: bold; }"
            "QPushButton:hover { background-color: #3CB371; }" // Medium Sea Green (darker on hover)
            "QPushButton:disabled { background-color: #90EE90; color: #E0E0E0; }"); // Light Green when disabled
        speedBoostButton->setCursor(Qt::PointingHandCursor);
        speedBoostButton->show();
        connect(speedBoostButton, &QPushButton::clicked, this, &Store::purchaseSpeedBoost);
        connect(speedBoostButton, &QPushButton::clicked, [this]() {
        });
    }

    shieldButton = new QPushButton("10 COINS", buttonPanel);
    if (shieldButton) {
        shieldButton->setGeometry(middleButtonX, buttonY, buttonWidth, buttonHeight);
        shieldButton->setStyleSheet(
            "QPushButton { background-color: #32CD32; color: white; " // Lime Green
            "border: 2px solid #006400; border-radius: 10px; font-size: 18px; font-weight: bold; }"
            "QPushButton:hover { background-color: #3CB371; }" // Medium Sea Green (darker on hover)
            "QPushButton:disabled { background-color: #90EE90; color: #E0E0E0; }"); // Light Green when disabled
        shieldButton->setCursor(Qt::PointingHandCursor);
        shieldButton->show();
        connect(shieldButton, &QPushButton::clicked, this, &Store::purchaseShield);
        connect(shieldButton, &QPushButton::clicked, [this]() {
        });
    }

    superAttackButton = new QPushButton("15 COINS", buttonPanel);
    if (superAttackButton) {
        superAttackButton->setGeometry(rightButtonX, buttonY, buttonWidth, buttonHeight);
        superAttackButton->setStyleSheet(
            "QPushButton { background-color: #32CD32; color: white; " // Lime Green
            "border: 2px solid #006400; border-radius: 10px; font-size: 18px; font-weight: bold; }"
            "QPushButton:hover { background-color: #3CB371; }" // Medium Sea Green (darker on hover)
            "QPushButton:disabled { background-color: #90EE90; color: #E0E0E0; }"); // Light Green when disabled
        superAttackButton->setCursor(Qt::PointingHandCursor);
        superAttackButton->show();
        connect(superAttackButton, &QPushButton::clicked, this, &Store::purchaseSuperAttack);

        // Debug message to check if button gets clicked
        connect(superAttackButton, &QPushButton::clicked, [this]() {
            qDebug() << "Attack button clicked!";
        });
    }

    QWidget* speedArea = new QWidget(buttonPanel);
    speedArea->setGeometry(leftButtonX, buttonY, buttonWidth, buttonHeight);
    speedArea->setCursor(Qt::PointingHandCursor);
    speedArea->setStyleSheet("background-color: rgba(50, 205, 50, 150);"); // Semi-transparent green
    speedArea->installEventFilter(this);
    speedArea->setObjectName("speedArea");
    speedArea->show();

    QWidget* shieldArea = new QWidget(buttonPanel);
    shieldArea->setGeometry(middleButtonX, buttonY, buttonWidth, buttonHeight);
    shieldArea->setCursor(Qt::PointingHandCursor);
    shieldArea->setStyleSheet("background-color: rgba(50, 205, 50, 150);"); // Semi-transparent green
    shieldArea->installEventFilter(this);
    shieldArea->setObjectName("shieldArea");
    shieldArea->show();

    QWidget* attackArea = new QWidget(buttonPanel);
    attackArea->setGeometry(rightButtonX, buttonY, buttonWidth, buttonHeight);
    attackArea->setCursor(Qt::PointingHandCursor);
    attackArea->setStyleSheet("background-color: rgba(50, 205, 50, 150);"); // Semi-transparent green
    attackArea->installEventFilter(this);
    attackArea->setObjectName("attackArea");
    attackArea->show();

    // Place button labels
    QLabel* speedLabel = new QLabel("5 COINS", speedArea);
    speedLabel->setGeometry(0, 0, buttonWidth, buttonHeight);
    speedLabel->setAlignment(Qt::AlignCenter);
    speedLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background-color: transparent;");

    QLabel* shieldLabel = new QLabel("10 COINS", shieldArea);
    shieldLabel->setGeometry(0, 0, buttonWidth, buttonHeight);
    shieldLabel->setAlignment(Qt::AlignCenter);
    shieldLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background-color: transparent;");

    QLabel* attackLabel = new QLabel("15 COINS", attackArea);
    attackLabel->setGeometry(0, 0, buttonWidth, buttonHeight);
    attackLabel->setAlignment(Qt::AlignCenter);
    attackLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background-color: transparent;");

    // Close button
    QPushButton* closeButton = new QPushButton("CLOSE", buttonPanel);
    closeButton->setGeometry(330, 520, 170, 55);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #4A321F; color: white; "
        "border: 2px solid #6B4A2F; border-radius: 10px; font-size: 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #5D4329; }");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->show();
    connect(closeButton, &QPushButton::clicked, [this]() {
        this->hide();
    });

    // Focus order
    if (speedBoostButton && shieldButton && superAttackButton && closeButton) {
        setTabOrder(speedBoostButton, shieldButton);
        setTabOrder(shieldButton, superAttackButton);
        setTabOrder(superAttackButton, closeButton);
    }

    QTimer::singleShot(0, this, [this]() {
        updateButtonStates();
    });
}
bool Store::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {

        if (obj->objectName() == "speedArea") {
            purchaseSpeedBoost();
            return true;
        }
        else if (obj->objectName() == "shieldArea") {
            purchaseShield();
            return true;
        }
        else if (obj->objectName() == "attackArea") {
            purchaseSuperAttack();
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

void Store::updateDisplay() {
    // Safety check
    if (!player || !coinsLabel) {
        return;
    }

    coinsLabel->setText(QString::number(player->getCoins()));
    updateButtonStates();
}

void Store::updateButtonStates() {
    // First check if we have a valid player object
    if (!player) {
        return;  // Exit early if player is null
    }

    int coins = player->getCoins();

    // Safely check each button before manipulating it
    if (speedBoostButton) {
        speedBoostButton->setEnabled(coins >= 5);
    }

    if (shieldButton) {
        shieldButton->setEnabled(coins >= 10);
    }

    if (superAttackButton) {
        superAttackButton->setEnabled(coins >= 15);
    }
}

void Store::purchaseSpeedBoost() {
    // Safety check
    if (!player) {
        return;
    }
    if (!speedBoostButton) return;
    if (player->getCoins() >= 5) {
        player->spendCoins(5);
        player->enableSpeedBoost(30);
        MainWindow::globalCoins = player->getCoins();
        updateDisplay();

        QMessageBox* successMsg = new QMessageBox(QMessageBox::Information,
                                                  "Speed Boost Activated!",
                                                  "You now move 50% faster for 30 seconds!",
                                                  QMessageBox::Ok,
                                                  this);
        successMsg->setIconPixmap(QPixmap(":/Rewards/speed.png").scaled(64, 64, Qt::KeepAspectRatio));
        successMsg->setAttribute(Qt::WA_DeleteOnClose);
        successMsg->show(); // Use show() instead of exec()
    } else {
        QMessageBox::warning(this, "Not Enough Coins",
                             "You need 5 coins to purchase Speed Boost.");
    }
}

void Store::purchaseShield() {
    // Safety check
    if (!player) {
        return;
    }
    if (!shieldButton) return;
    if (player->getCoins() >= 10) {
        player->spendCoins(10);
        player->enableShield(30);
        MainWindow::globalCoins = player->getCoins();
        updateDisplay();

        QMessageBox* successMsg = new QMessageBox(QMessageBox::Information,
                                                  "Shield Activated!",
                                                  "You take 50% less damage for 30 seconds!",
                                                  QMessageBox::Ok,
                                                  this);
        successMsg->setIconPixmap(QPixmap(":/Rewards/shield.png").scaled(64, 64, Qt::KeepAspectRatio));
        successMsg->setAttribute(Qt::WA_DeleteOnClose);
        successMsg->show(); // Use show() instead of exec()
    } else {
        QMessageBox::warning(this, "Not Enough Coins",
                             "You need 10 coins to purchase Shield.");
    }
}

void Store::purchaseSuperAttack() {
    // Safety check
    if (!player) {
        return;
    }
    if (!superAttackButton) return;
    if (player->getCoins() >= 15) {
        player->spendCoins(15);
        player->enableSuperAttack(30);
        MainWindow::globalCoins = player->getCoins();
        updateDisplay();
        QMessageBox* successMsg = new QMessageBox(QMessageBox::Information,
                                                  "Super Attack Activated!",
                                                  "Your attacks now deal double damage for 30 seconds!",
                                                  QMessageBox::Ok,
                                                  this);
        successMsg->setIconPixmap(QPixmap(":/Rewards/superattack.png").scaled(64, 64, Qt::KeepAspectRatio));
        successMsg->setAttribute(Qt::WA_DeleteOnClose);
        successMsg->show(); // Use show() instead of exec()
    } else {
        QMessageBox::warning(this, "Not Enough Coins",
                             "You need 15 coins to purchase Super Attack.");
    }
}
// Add this method to catch any clicks anywhere
void Store::mousePressEvent(QMouseEvent *event)
{
    // Check if click is in button areas
    QRect speedRect(75, 400, 170, 55);
    QRect shieldRect(315, 400, 170, 55);
    QRect attackRect(565, 400, 170, 55);

    if (speedRect.contains(event->pos())) {
        purchaseSpeedBoost();
    }
    else if (shieldRect.contains(event->pos())) {
        purchaseShield();
    }
    else if (attackRect.contains(event->pos())) {
        purchaseSuperAttack();
    }
    else {
        QDialog::mousePressEvent(event);
    }
}
