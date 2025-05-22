#include "level4.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "mainwindow.h"
#include <QTimer>
#include "message.h"
#include "enemies.h"
#include "rewards.h"
#include "obstacle.h"

Level4::Level4(QGraphicsScene* scene, Player* p1)
    : Level(4, scene, p1),
    asteroidAttackActive(false),
    remainingTime(90),
    timerPaused(false),
    timersInitialized(false),
    asteroidTimer(nullptr),
    levelTimer(nullptr),
    spaceshipTimer(nullptr),
    timerDisplay(nullptr)
{
    initializeTimers();
}

Level4::~Level4() {

    stopAllTimers();

    // Clean up timer display
    if (timerDisplay) {
        if (scene && scene->items().contains(timerDisplay)) {
            scene->removeItem(timerDisplay);
        }
        delete timerDisplay;
        timerDisplay = nullptr;
    }
}

void Level4::initializeTimers() {
    if (timersInitialized) {
        return; // Prevent double initialization
    }

    // Initialize level timer
    levelTimer = new QTimer(this);
    levelTimer->setSingleShot(false);
    connect(levelTimer, &QTimer::timeout, this, &Level4::updateTimerDisplay);

    // Initialize asteroid timer
    asteroidTimer = new QTimer(this);
    asteroidTimer->setSingleShot(false);
    connect(asteroidTimer, &QTimer::timeout, this, &Level4::spawnAsteroid);

    // Initialize spaceship timer
    spaceshipTimer = new QTimer(this);
    spaceshipTimer->setSingleShot(false);
    connect(spaceshipTimer, &QTimer::timeout, this, &Level4::spawnSpaceship);

    timersInitialized = true;
}

void Level4::cleanupAllTimerDisplays() {

    QList<QGraphicsItem*> itemsToRemove;

    for (auto* item : scene->items()) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);
        if (textItem) {
            QString text = textItem->toPlainText();
            if (text.contains("Time:") || text.contains("Timer:") ||
                text.startsWith("Time") || text.contains("90") ||
                text.contains("89") || text.contains("88") ||
                (text.length() < 20 && (text.contains(":") || text.contains("s")))) {
                itemsToRemove.append(textItem);
            }
        }
    }

    for (auto* item : itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }

    timerDisplay = nullptr;

}

void Level4::setupLevel() {

    // Ensure timers are initialized
    if (!timersInitialized) {
        initializeTimers();
    }

    if (levelTimer && levelTimer->isActive()) {
        levelTimer->stop();
    }
    if (asteroidTimer && asteroidTimer->isActive()) {
        asteroidTimer->stop();
    }
    if (spaceshipTimer && spaceshipTimer->isActive()) {
        spaceshipTimer->stop();
    }

    cleanupAllTimerDisplays();

    for (auto* item : scene->items()) {
        if (dynamic_cast<Snowflake*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    for (auto* item : obstacles) {
        if (scene->items().contains(item)) {
            scene->removeItem(item);
        }
        delete item;
    }
    obstacles.clear();

    remainingTime = 90;
    timerPaused = false;
    asteroidAttackActive = false;

    timerDisplay = new QGraphicsTextItem();
    timerDisplay->setDefaultTextColor(Qt::white);
    QFont font = timerDisplay->font();
    font.setPointSize(16);
    font.setBold(true);
    timerDisplay->setFont(font);

    timerDisplay->setPos(650, 15);
    timerDisplay->setZValue(2000);
    timerDisplay->setData(0, "level4_timer");
    scene->addItem(timerDisplay);
    QString timerText = QString("Level Timer: %1s").arg(remainingTime);
    timerDisplay->setPlainText(timerText);

    // Add coins
    for (int i = 0; i < 15; ++i) {
        int x = 200 + i * 150;
        int y = 80 + (i % 5) * 60;
        Coin* coin = new Coin(x, y);
        scene->addItem(coin);
    }

    // Add stars
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Star* star = new Star(x, y);
        star->setZValue(10);
        scene->addItem(star);
    }

    const int groundY = 550;
    const int platformWidth = 520;
    const int platformHeight = 190;
    const int platformSpacingY = 80;

    QVector<QPair<QPoint, QString>> platformLayout = {
        { QPoint(200, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(500, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(800, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        { QPoint(100, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(400, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(700, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(950, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        { QPoint(200, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(500, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(800, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        { QPoint(50, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(350, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(650, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(900, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        { QPoint(250, groundY - 4.2 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(600, groundY - 4.2 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(150, groundY - 6 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(450, groundY - 6 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(750, groundY - 6.2 * platformSpacingY), ":/backgrounds/spaceplatform1.png" }
    };

    for (const auto& platformInfo : platformLayout) {
        QPoint pos = platformInfo.first;
        QString texturePath = platformInfo.second;

        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(texturePath).scaled(platformWidth, platformHeight));
        platform->setPos(pos);
        platform->setData(0, "platform");
        scene->addItem(platform);
        obstacles.append(platform);
    }

    // Add player
    if (!scene->items().contains(p1)) {
        scene->addItem(p1);
    }
    p1->setPosition(50, groundY - 150);
    p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
    p1->setFocus();

    // Add level message
    Message* startMessage = Message::createLevelFourStartMessage();
    startMessage->showMessage(scene, 300, 350);

    addEnemies();

    if (levelTimer) {
        levelTimer->start(1000); // Update every second
    }

    QTimer::singleShot(5000, this, [this]() {
        if (scene && spaceshipTimer) {
            spaceshipTimer->start(30000); // Every 30 seconds
            spawnSpaceship(); // Spawn first one immediately
        }
    });
}

void Level4::resetLevel() {
    // Stop all timers
    if (levelTimer) {
        levelTimer->stop();
    }
    if (asteroidTimer) {
        asteroidTimer->stop();
    }
    if (spaceshipTimer) {
        spaceshipTimer->stop();
    }

    cleanupAllTimerDisplays();

    if (p1) {
        p1->setHealth(100);
        p1->setPosition(50, 550 - 100);
        p1->resetStars();
        // Make sure player is not frozen
        if (p1->isFrozenBySpaceship()) {
            p1->unfreezeFromSpaceship();
        }
        p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
        p1->setFocus();
    }

    // Clean up scene items
    QList<QGraphicsItem*> itemsToRemove;
    for (auto* item : scene->items()) {
        if (item != p1) {
            if (dynamic_cast<Coin*>(item) ||
                dynamic_cast<Star*>(item) ||
                dynamic_cast<Alien*>(item) ||
                dynamic_cast<Asteroid*>(item) ||
                dynamic_cast<Spaceship*>(item)) {
                itemsToRemove.append(item);
            }
        }
    }

    for (auto* item : itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }

    for (auto* item : obstacles) {
        if (scene->items().contains(item)) {
            scene->removeItem(item);
        }
        delete item;
    }
    obstacles.clear();

    asteroidAttackActive = false;
    remainingTime = 90;
    timerPaused = false;

    if (!timersInitialized) {
        initializeTimers();
    }

    setupLevel();
}

void Level4::addEnemies() {
    const int platformHeight = 550;
    const int AlienHeight = 50;
    const int AlienOffset = 5;

    Alien* Alien1 = new Alien(380, 245 - AlienHeight - AlienOffset);
    scene->addItem(Alien1);

    Alien* Alien2 = new Alien(500, 120 - AlienHeight - AlienOffset);
    scene->addItem(Alien2);

    Alien* Alien3 = new Alien(570, 160 - AlienHeight - AlienOffset);
    scene->addItem(Alien3);
}

void Level4::startAsteroidAttack() {
    asteroidAttackActive = true;

    Message* asteroidWarning = new Message("Asteroid Attack!", 2000);
    asteroidWarning->showMessage(scene, 350, 300);

    if (asteroidTimer) {
        asteroidTimer->start(2000);
    }
}

void Level4::spawnAsteroid() {
    if (!scene) {
        return;
    }

    int startX = QRandomGenerator::global()->bounded(scene->width() - 100) + 800;
    int startY = 50;

    Asteroid* asteroid = new Asteroid(startX, startY);
    scene->addItem(asteroid);
    if (asteroid->moveTimer) {
        asteroid->moveTimer->start(50);
    }
}

void Level4::checkAsteroidAttack() {
    if (!asteroidAttackActive && p1 && p1->getCollectedStars() >= 10) {
        startAsteroidAttack();
    }
}

void Level4::spawnSpaceship() {
    if (!scene || !p1) {
        return;
    }

    int startX = QRandomGenerator::global()->bounded(scene->width() - 100);
    int startY = 0;
    Spaceship* ship = new Spaceship(startX, startY);
    scene->addItem(ship);

    ship->setData(1, QVariant::fromValue(static_cast<void*>(p1)));

    // If the timer is not active, start it for the next spaceship
    if (spaceshipTimer && !spaceshipTimer->isActive()) {
        spaceshipTimer->start(30000); // 30 seconds
    }
}

void Level4::updateTimerDisplay() {

    // Don't update if paused
    if (timerPaused) {
        return;
    }

    // Update display text FIRST
    if (timerDisplay) {
        QString timerText = QString("Level Timer: %1s").arg(remainingTime);
        timerDisplay->setPlainText(timerText);
    } else {
        // Try to recreate timer display
        timerDisplay = new QGraphicsTextItem();
        timerDisplay->setDefaultTextColor(Qt::white);
        QFont font = timerDisplay->font();
        font.setPointSize(16);
        font.setBold(true);
        timerDisplay->setFont(font);
        timerDisplay->setPos(650, 20);
        timerDisplay->setZValue(2000);
        timerDisplay->setData(0, "level4_timer");
        scene->addItem(timerDisplay);

        QString timerText = QString("Level Timer: %1s").arg(remainingTime);
        timerDisplay->setPlainText(timerText);
    }

    if (!asteroidAttackActive && p1 && p1->getCollectedStars() >= 10) {
        startAsteroidAttack();
    }
    remainingTime--;

    if (MainWindow* mainWindow = qobject_cast<MainWindow*>(parent())) {
        mainWindow->updateLevelTimer(remainingTime);
    }

    if (remainingTime <= 0) {

        if (levelTimer) {
            levelTimer->stop();
        }
        Message* timeUpMessage = new Message("Time's Up! Game Over", 3000);
        timeUpMessage->showMessage(scene, 300, 300);
        QTimer::singleShot(3000, this, [this]() {
            resetLevel();
        });
        return;
    }
    if (p1 && !p1->isFrozenBySpaceship()) {
        p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
        p1->setFocus();
    }
}

void Level4::pauseTimer() {
    timerPaused = true;
}

void Level4::resumeTimer() {
    timerPaused = false;
}

void Level4::stopAllTimers() {
    if (asteroidTimer) {
        asteroidTimer->stop();
    }

    if (levelTimer) {
        levelTimer->stop();
    }

    if (spaceshipTimer) {
        spaceshipTimer->stop();
    }

    // Stop any timers
    QList<QTimer*> childTimers = findChildren<QTimer*>();
    for (QTimer* timer : childTimers) {
        if (timer && timer != levelTimer && timer != asteroidTimer && timer != spaceshipTimer) {
            timer->stop();
            timer->deleteLater();
        }
    }

}
