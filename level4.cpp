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
    remainingTime(90),  // 1 minute 30 seconds
    timerPaused(false)
{
    // Initialize asteroid timer
    asteroidTimer = new QTimer(this);
    connect(asteroidTimer, &QTimer::timeout, this, &Level4::spawnAsteroid);

    // Initialize level timer (updates every second)
    levelTimer = new QTimer(this);
    connect(levelTimer, &QTimer::timeout, this, &Level4::updateTimerDisplay);

    // Initialize spaceship timer but don't start it yet
    spaceshipTimer = new QTimer(this);
    connect(spaceshipTimer, &QTimer::timeout, this, &Level4::spawnSpaceship);
}

Level4::~Level4()
{
    if (asteroidTimer) {
        asteroidTimer->stop();
        delete asteroidTimer;
    }
    if (levelTimer) {
        levelTimer->stop();
        delete levelTimer;
    }
    if (spaceshipTimer) {
        spaceshipTimer->stop();
        delete spaceshipTimer;
    }
}

void Level4::setupLevel() {
    // Remove snowflakes
    for (auto* item : scene->items()) {
        if (dynamic_cast<Snowflake*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Clear previous obstacles
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();

    //Add coins
    for (int i = 0; i < 15; ++i) {
        int x = 200 + i * 150;
        int y = 80 + (i % 5) * 60;
        Coin* coin = new Coin(x, y);
        scene->addItem(coin);
    }

    // Setup timer display
    timerDisplay = new QGraphicsTextItem();
    timerDisplay->setDefaultTextColor(Qt::white);
    QFont font = timerDisplay->font();
    font.setPointSize(16);
    font.setBold(true);
    timerDisplay->setFont(font);
    timerDisplay->setPos(650, 20);  // Top right corner
    scene->addItem(timerDisplay);

    // Initial timer display
    updateTimerDisplay();

    // Start level timer
    levelTimer->start(1000);  // Update every second

    // Schedule spaceship appearances
    QTimer::singleShot(5000, this, &Level4::spawnSpaceship);

    // Reset counters
    remainingTime = 90;
    // Add stars
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Star* star = new Star(x, y);
        star->setZValue(10);
        scene->addItem(star);
    }

    const int groundY = 550;
    const int platformWidth = 520;  // Larger platforms as you specified
    const int platformHeight = 190; // Larger height as you specified
    const int platformSpacingY = 80;


    QVector<QPair<QPoint, QString>> platformLayout = {

        // Row 1 (slightly above ground)
        { QPoint(200, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(500, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(800, groundY - 3 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        // Row 2
        { QPoint(100, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(400, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(700, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(950, groundY - 4 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        // Row 3
        { QPoint(200, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(500, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(800, groundY - 5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        // Row 4 (higher)
        { QPoint(50, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(350, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },
        { QPoint(650, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform1.png" },
        { QPoint(900, groundY - 6.5 * platformSpacingY), ":/backgrounds/spaceplatform2.png" },

        // Additional platforms at various heights to match the screenshot
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
    scene->addItem(p1);
    p1->setPosition(50, groundY - 150);

    // Add level message
    Message* startMessage = Message::createLevelFourStartMessage();
    startMessage->showMessage(scene, 300, 350);

    // Add the alien enemy
    addEnemies();

    // Initialize asteroid attack variables
    asteroidAttackActive = false;
    asteroidTimer = new QTimer(this);
    connect(asteroidTimer, &QTimer::timeout, this, &Level4::spawnAsteroid);
}

void Level4::resetLevel() {
    // Static flag to prevent re-entry
    static bool resetting = false;

    if (resetting) {
        return;
    }

    resetting = true;

    // Stop all timers first
    stopAllTimers();

    // Reset player
    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);
    p1->resetStars(); // Reset collected stars

    // Make sure player is not frozen
    if (p1->isFrozenBySpaceship()) {
        p1->unfreezeFromSpaceship();
    }
    p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
    p1->setFocus();

    // Remove all obstacles and items safely
    QList<QGraphicsItem*> itemsToRemove = obstacles;
    obstacles.clear();

    for (auto* item : itemsToRemove) {
        if (item && scene->items().contains(item)) {
            scene->removeItem(item);
            delete item;
        }
    }
    obstacles.clear();

    // Remove timer display if it exists
    if (timerDisplay) {
        scene->removeItem(timerDisplay);
        delete timerDisplay;
        timerDisplay = nullptr;
    }

    // Safely remove any remaining asteroids and spaceships
    QList<QGraphicsItem*> allItems = scene->items();
    for (auto* item : allItems) {
        Asteroid* asteroid = dynamic_cast<Asteroid*>(item);
        Spaceship* spaceship = dynamic_cast<Spaceship*>(item);
        Star* star = dynamic_cast<Star*>(item);
        Alien* alien = dynamic_cast<Alien*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);
        if (asteroid || spaceship || star || alien || coin) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Reset state variables
    asteroidAttackActive = false;
    remainingTime = 90; // Reset to 1 minute 30 seconds

    // Now set up the level again
    setupLevel();

    // Start the timer after setup is complete
    if (levelTimer) {
        levelTimer->start(1000);
    }

    // Clear the flag
    resetting = false;
}

void Level4::addEnemies() {
    const int platformHeight = 550;
    const int AlienHeight = 50; // Approximate height of the penguin sprite
    const int AlienOffset = 5; // Slightly raised for visibility

    Alien* Alien1 = new Alien(380, 245 - AlienHeight - AlienOffset);
    scene->addItem(Alien1);

    Alien* Alien2 = new Alien(500, 120 - AlienHeight - AlienOffset);
    scene->addItem(Alien2);

    Alien* Alien3 = new Alien(570, 160 - AlienHeight - AlienOffset);
    scene->addItem(Alien3);

    // Monster* monster1 = new Monster(250, 400);
    // scene->addItem(monster1);

}

void Level4::startAsteroidAttack() {
    asteroidAttackActive = true;

    // Show a warning message
    Message* asteroidWarning = new Message("Asteroid Attack!", 2000);
    asteroidWarning->showMessage(scene, 350, 300);

    // Start spawning asteroids every 2 seconds
    asteroidTimer->start(2000);
}

void Level4::spawnAsteroid() {
    // Random x position at the top of the screen
    int startX = QRandomGenerator::global()->bounded(scene->width() - 100) + 800;
    int startY = 50; // Near the top of the screen

    Asteroid* asteroid = new Asteroid(startX, startY);
    scene->addItem(asteroid);
    asteroid->moveTimer->start(50); // Update position every 50ms
}

void Level4::checkAsteroidAttack()
{
    if (!asteroidAttackActive && p1->getCollectedStars() >= 10) {
        startAsteroidAttack();
    }
}

void Level4::spawnSpaceship() {
    // Create spaceship at random position above the screen
    int startX = QRandomGenerator::global()->bounded(scene->width() - 100);
    int startY = 0;
    Spaceship* ship = new Spaceship(startX, startY);
    scene->addItem(ship);

    // Set the player as the target
    ship->setData(1, QVariant::fromValue(static_cast<void*>(p1)));

    // No need to schedule next spaceship
    // If the timer is not active, start it for the next spaceship
    if (!spaceshipTimer->isActive()) {
        spaceshipTimer->start(30000); // 30 seconds
    }
}

void Level4::updateTimerDisplay() {
    // Always decrement time, even if player is frozen
    remainingTime--;

    // Update the timer display in MainWindow
    if (MainWindow* mainWindow = dynamic_cast<MainWindow*>(scene->parent())) {
        mainWindow->updateLevelTimer(remainingTime);
    }

    // Check if time is up
    if (remainingTime <= 0) {
        levelTimer->stop();

        // Create game over message
        Message* timeUpMessage = new Message("Time's Up! Game Over", 3000);
        timeUpMessage->showMessage(scene, 300, 300);

        // Reset level after 2 seconds
        QTimer::singleShot(2000, [this]() {
            resetLevel();
        });
    }

    // Ensure player has focus if not frozen
    if (!p1->isFrozenBySpaceship()) {
        p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
        p1->setFocus();
    }
}
