#include "level5.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include "mainwindow.h"
#include <QTimer>
#include "message.h"
#include "enemies.h"
#include "rewards.h"
#include "obstacle.h"

Level5::Level5(QGraphicsScene* scene, Player* p1)
    : Level(5, scene, p1) {
    // Initialize all pointers to nullptr
    caveman = nullptr;
    troll = nullptr;
    fightTimer = nullptr;

    // Initialize battle state flags
    cavemanAttackActive = false;
    battleStarted = false;
    cinematicPlaying = false;
    trollAttackActive = false;
    batSpawned = false;
}

void Level5::setupLevel() {
    //Remove stars
    for (auto* item : scene->items()) {
        if (dynamic_cast<Star*>(item)) {
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

    // Add Diamonds
    for (int i = 0; i < 40; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Diamond* diamond = new Diamond(x, y);
        scene->addItem(diamond);
    }

    const int groundY = 550;
    const int platformWidth = 400;
    const int platformHeight = 200;
    const int platformSpacingY = 80;

    QVector<QPair<QPoint, QString>> platformLayout = {
        // Position, Texture path
        { QPoint(100, groundY - 2.5 * platformSpacingY), ":/backgrounds/caveplatform1.png" },
        { QPoint(300, groundY - 4.5 * platformSpacingY), ":/backgrounds/caveplatform1.png" },
        { QPoint(700, groundY - 6 * platformSpacingY), ":/backgrounds/caveplatform1.png"}
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
    Message* startMessage = Message::createLevelFiveStartMessage();
    startMessage->showMessage(scene, 300, 350);

    // Add the enemy
    addEnemies();

    cavemanAttackActive = false;
    battleStarted = false;
    cinematicPlaying = false;
    trollAttackActive = false;

}
void Level5::addEnemies() {
    const int creatureHeight = 50;
    const int creatureOffset = 5;
    CaveCreature* creature1 = new CaveCreature(300, 130 - creatureHeight - creatureOffset);
    scene->addItem(creature1);
    CaveCreature* creature3 = new CaveCreature(100, 295 - creatureHeight - creatureOffset);
    scene->addItem(creature3);
    caveman = nullptr;
    troll = nullptr;
}
void Level5::checkCavemanAttack() {
    qDebug() << "checkCavemanAttack called:";
    qDebug() << "  Caveman active: " << cavemanAttackActive;
    qDebug() << "  Diamond count: " << p1->getCollectedDiamonds();
    qDebug() << "  Troll exists: " << (troll != nullptr);


    if (p1->getCollectedDiamonds() ){
        startCavemanAttack();
    } else {
        qDebug() << "Conditions not met for caveman spawn";
    }
}
void Level5::checkTrollAttack() {
    if (!trollAttackActive && p1->getCollectedDiamonds() >= 20) {
        startTrollAttack();
    }
}

void Level5::checkBatAttack() {
    if (!batSpawned && p1->getCollectedDiamonds() >= 10) {
        batSpawned = true;

        Bat* bat = new Bat(p1->x() + 300, p1->y() - 100);  // Adjust spawn position as needed
        scene->addItem(bat);
    }

    // Check damage if Bat is already in scene
    for (QGraphicsItem* item : scene->items()) {
        Bat* bat = dynamic_cast<Bat*>(item);
        if (bat) {
            bat->handleCollision(p1);
        }
    }
}


// Modify startCavemanAttack to add visibility checks
void Level5::startCavemanAttack() {
    qDebug() << "===== STARTING CAVEMAN ATTACK =====";
    cavemanAttackActive = true;

    Message* warn = new Message("Caveman Incoming!", 2000);
    warn->showMessage(scene, 350, 280);

    int spawnX = scene->width() - 100;
    int spawnY = 200;

    // Remove any existing cavemen
    for (QGraphicsItem* item : scene->items()) {
        if (Caveman* oldCaveman = dynamic_cast<Caveman*>(item)) {
            scene->removeItem(oldCaveman);
            delete oldCaveman;
        }
    }

    // Create new caveman
    caveman = new Caveman(spawnX, spawnY, this);

    // Check if creation succeeded
    if (!caveman) {
        qDebug() << "ERROR: Failed to create caveman!";
        return;
    }

    // Check caveman properties before adding to scene
    qDebug() << "Caveman pixmap null? " << caveman->pixmap().isNull();
    qDebug() << "Caveman size: " << caveman->boundingRect().size();

    // Set explicit visibility
    caveman->setVisible(true);
    caveman->setZValue(10); // Ensure it's above other items

    // Add to scene
    scene->addItem(caveman);

    // Verify it was added
    qDebug() << "Caveman added to scene: " << scene->items().contains(caveman);
    qDebug() << "Caveman position: " << caveman->pos();
    qDebug() << "Caveman is visible: " << caveman->isVisible();

    // Add a red marker to show where the caveman should be
    QGraphicsRectItem* marker = new QGraphicsRectItem(0, 0, 20, 20);
    marker->setBrush(QBrush(Qt::red));
    marker->setPos(spawnX, spawnY);
    marker->setZValue(20); // Above everything else
    scene->addItem(marker);
    qDebug() << "Added red marker at caveman position";
}
void Level5::startTrollAttack() {
        trollAttackActive = true;

        Message* warn = new Message("Troll Incoming!", 2000);
        warn->showMessage(scene, 350, 280);

        // Spawn near player but always at a fixed height
        int spawnX = scene->width() - 100; // More consistent spawn location
        int spawnY = 200; // Fixed height relative to ground

        // Remove any existing trolls first
        for (QGraphicsItem* item : scene->items()) {
            if (Troll* oldTroll = dynamic_cast<Troll*>(item)) {
                scene->removeItem(oldTroll);
                delete oldTroll;
            }
        }

        // Create new troll at fixed position
        troll = new Troll(spawnX, spawnY);
        scene->addItem(troll);
        // Debug
        qDebug() << "Troll spawned at:" << spawnX << spawnY;
}

void Level5::update() {
    // Check for enemy spawning conditions - this is all you need
    checkCavemanAttack();  // Spawn caveman if player has 30+ diamonds and troll is dead
    checkTrollAttack();    // Spawn troll if player has 20+ diamonds
    checkBatAttack();      // Spawn bat if player has 10+ diamonds

    // No distance checking needed since you only want them to appear based on diamond count
}


// void Level5::startTrollBattle() {
//  //    if (battleStarted) return; // Don't start if already in battle

//  //    battleStarted = true;
//  // //   cinematicPlaying = true;

//  //    Message* battleMsg = new Message("Battle with Troll!", 2000);
//  //    battleMsg->showMessage(scene, 300, 300);

//  //    // fightTimer = new QTimer(this);
//  //    // fightTimer->start(300);
// }
// void Level5::startBattle() {
//     // if (!caveman) {
//     //     qDebug() << "No caveman to battle with!";
//     //     return;
//     // }

//     // qDebug() << "Starting battle with caveman!";
//     // battleStarted = true;
//     // Player::bossBattle = true;

//     // // Disable player movement during battle
//     // p1->canMove = false;

//     // // Display battle message
//     // Message* battleMsg = new Message("Battle with Caveman!", 2000);
//     // battleMsg->showMessage(scene, 300, 300);

//     // // Start simplified fight sequence
//     // fightTimer = new QTimer(this);
//     // connect(fightTimer, &QTimer::timeout, this, &Level5::autoFight);
//     // fightTimer->start(1000); // Slower pace for visibility
// }


// void Level5::autoFight() {
//     // if (!caveman) return;

//     // qDebug() << "Fighting round!";

//     // // Player attacks caveman
//     // caveman->takeDamage(25);
//     // qDebug() << "Player attacks caveman! Caveman health: " << caveman->health;

//     // // Check if caveman is defeated
//     // if (!caveman->isAlive()) {
//     //     qDebug() << "Caveman defeated!";
//     //     if (fightTimer) {
//     //         fightTimer->stop();
//     //         fightTimer->deleteLater();
//     //         fightTimer = nullptr;
//     //     }

//     //     // Change caveman appearance to defeated
//     //     caveman->setPixmap(QPixmap(":/Enemies/deadcaveman.png").scaled(100, 100));

//     //     // End battle sequence
//     //     QTimer::singleShot(2000, this, &Level5::endBattle);
//     //     return;
//     // }

//     // // Caveman attacks player
//     // p1->takeDamage(10);
//     // qDebug() << "Caveman attacks player! Player health: " << p1->getHealth();

//     // // Check if player is defeated
//     // if (p1->getHealth() <= 0) {
//     //     qDebug() << "Player defeated!";
//     //     if (fightTimer) {
//     //         fightTimer->stop();
//     //         fightTimer->deleteLater();
//     //         fightTimer = nullptr;
//     //     }

//     //     p1->canMove = false;

//     //     Message* gameOverMsg = new Message("Game Over", 2000);
//     //     gameOverMsg->showMessage(scene, 300, 350);

//     //     QTimer::singleShot(2000, this, &Level5::resetLevel);
//     // }
// }


// void Level5::endBattle() {
//     // qDebug() << "Battle ended!";
//     // battleStarted = false;
//     // Player::bossBattle = false;

//     // // Enable player movement
//     // p1->canMove = true;

//     // // Display victory message
//     // Message* victoryMsg = new Message("Victory!", 2000);
//     // victoryMsg->showMessage(scene, 300, 300);

//     // // Optionally remove caveman from scene
//     // if (caveman) {
//     //     QTimer::singleShot(3000, this, [this]() {
//     //         if (caveman) {
//     //             scene->removeItem(caveman);
//     //             delete caveman;
//     //             caveman = nullptr;
//     //         }
//     //     });
//     // }
// }

void Level5::resetLevel() {

    p1->setHealth(100);
    p1->setPosition(50, 550 - 100);
    p1->resetDiamonds();
    // Remove all obstacles including snowmen
    for (auto* item : obstacles) {
        scene->removeItem(item);
        delete item;
    }
    obstacles.clear();
    QList<QGraphicsItem*> allItems = scene->items();
    for (auto* item : allItems) {
        Caveman* caveman = dynamic_cast<Caveman*>(item);
        CaveCreature* creature = dynamic_cast<CaveCreature*>(item);
        Troll* troll = dynamic_cast<Troll*>(item);
        Bat* bat = dynamic_cast<Bat*>(item);
        Coin* coin = dynamic_cast<Coin*>(item);
        Diamond* diamond = dynamic_cast<Diamond*>(item);

        if (caveman || creature || troll ||bat || coin || diamond) {
            scene->removeItem(item);
            delete item;
        }
    }
    setupLevel();
}

void Level5::killTroll() {
    qDebug() << "KILLING TROLL NOW";

    // Find the troll in the scene
    for (QGraphicsItem* item : scene->items()) {
        if (Troll* trollItem = dynamic_cast<Troll*>(item)) {
            qDebug() << "Found troll to kill";

            // Set health to 0 and update state
            trollItem->health = 0;
            trollItem->setVisible(false);

            qDebug() << "Troll killed! isAlive() returns: " << trollItem->isAlive();
            qDebug() << "Calling checkCavemanAttack() directly";

            // Force check for caveman spawn
            checkCavemanAttack();
            return;
        }
    }

    qDebug() << "NO TROLL FOUND TO KILL";
}
