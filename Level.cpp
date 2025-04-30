#include "Level.h"
#include "obstacle.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "waterdroplet.h"


    Level::Level(int number, QGraphicsScene* scene, Player* p1)
    : scene(scene), p1(p1), levelNumber(number)
{
}

void Level::setupLevel()
{
    // Clear previous obstacles
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();

    // Create water droplets in the initial screens
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;  // Regular spacing
        int y = 100 + (i % 3) * 80;  // Vary the height
        WaterDroplet* droplet = new WaterDroplet(x, y);
        scene->addItem(droplet);
    }

    const int groundY = 550;
    const int platformWidth = 250;
    const int platformHeight = 100;
    const int platformSpacingY = 80;

    if (levelNumber == 1) {
        QVector<QPoint> platformPositions = {
                                             {100, groundY - 2 * platformSpacingY},
                                             {300, groundY - 3 * platformSpacingY},
                                             {500, groundY - 4 * platformSpacingY},
                                             {200, groundY - 5 * platformSpacingY},
                                             {400, groundY - 6 * platformSpacingY},
                                             {100, groundY - 4 * platformSpacingY},
                                             };

        QRandomGenerator *randomGen = QRandomGenerator::global();

        // Add platforms to the scene
        for (const QPoint& pos : platformPositions) {
            QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(":/backgrounds/brownbricks.png").scaled(platformWidth, platformHeight));
            platform->setPos(pos);
            platform->setData(0, "platform");
            scene->addItem(platform);
            obstacles.append(platform); // Track platform in the obstacles list
        }

        QVector<int> floorPositions = {250, 600, 950, 1300, 1650};

        for (int pos : floorPositions) {
            int randObstacle = randomGen->bounded(3); // Random number between 0 and 2

            // Place fire obstacle
            if (randObstacle == 0) {
                Fire* fire = new Fire(pos, groundY - 50);  // Place on the ground
                addObstacle(fire);
            }
            // Place cactus obstacle
            else if (randObstacle == 1) {
                Cactus* cactus = new Cactus(pos, groundY - 50);  // Place on the ground
                addObstacle(cactus);
            }
            // Place quicksand obstacle
            else if (randObstacle == 2) {
                Quicksand* quicksand = new Quicksand(pos, groundY-50);  // Slightly into the ground
                quicksand->setLevel(this);
                addObstacle(quicksand);
            }
        }
    }

    // Add player
    scene->addItem(p1);
    p1->setPosition(50, groundY - 100); // Start just above the ground
}


void Level::addObstacle(QGraphicsItem* obstacle) {
    // Set obstacle data tag for proper identification
    obstacle->setData(0, "obstacle");

    obstacles.push_back(obstacle);
    scene->addItem(obstacle);
}


void Level::resetLevel()
{
    p1->setHealth(100);

    // Set player position to starting point
    const int groundY = 550;
    p1->setPosition(50, groundY - 100);

    // Reset player's collected droplets count
    // Note: This assumes you might want to reset the count when resetting level
    // If you want droplets to accumulate across level resets, remove this line
    p1->setDropletsCollected(0);

    // Remove and recreate level elements
    setupLevel();
}


void Level::nextLevel()
{
    levelNumber++;
    setupLevel();
}

int Level::getLevelNumber() const
{
    return levelNumber;
}
