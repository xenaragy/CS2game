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
    : Level(5, scene, p1) {}

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
    for (int i = 0; i < 30; ++i) {
        int x = 150 + i * 100;
        int y = 100 + (i % 3) * 80;
        Diamond* diamond = new Diamond(x, y);
        scene->addItem(diamond);
    }

    const int groundY = 550;
    const int platformWidth = 400;
    const int platformHeight = 200;
    const int platformSpacingY = 80;

    // Setup platforms
    QVector<QPoint> platformPositions = {
        {100, groundY - 2 * platformSpacingY},
        {300, groundY - 3 * platformSpacingY},
        {500, groundY - 4 * platformSpacingY},
        {200, groundY - 5 * platformSpacingY},
        {400, groundY - 6 * platformSpacingY}
    };

    // Add platforms
    QRandomGenerator* randGen = QRandomGenerator::global();
    QStringList spaceTextures = {
        ":/backgrounds/caveplatform1.png",
        ":/backgrounds/caveplatform2.png",
        ":/backgrounds/CavePlatform3.png"
    };

    for (const QPoint& pos : platformPositions) {
        QString selectedTexture = spaceTextures[randGen->bounded(spaceTextures.size())];
        QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(QPixmap(selectedTexture).scaled(platformWidth, platformHeight));
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
    CaveCreature* creature1 = new CaveCreature(380, 245 - creatureHeight - creatureOffset);
    scene->addItem(creature1);
    CaveCreature* creature2 = new CaveCreature(500, 120 - creatureHeight - creatureOffset);
    scene->addItem(creature2);
    CaveCreature* creature3 = new CaveCreature(570, 160 - creatureHeight - creatureOffset);
    scene->addItem(creature3);
    caveman = nullptr;
    troll = nullptr;
}
void Level5::checkCavemanAttack() {
    if (!cavemanAttackActive && p1->getCollectedDiamonds() >= diamondsThreshold) {
        startCavemanAttack();
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
            QPointF playerPos = p1->pos();
            QPointF batPos = bat->pos();
            qreal distance = QLineF(playerPos, batPos).length();
            if (distance < 100) {
                p1->takeDamage(5);
            }
        }
    }
}



void Level5::startCavemanAttack() {
    cavemanAttackActive = true;
    cinematicPlaying = true;

    Message* warn = new Message("Caveman Attack!", 2000);
    warn->showMessage(scene, 350, 300);

    int spawnX = scene->width() + 50;

    int cavemanHeight = 100; // Adjust based on sprite size
    int spawnY = p1->pos().y() + p1->boundingRect().height() - cavemanHeight;

    caveman = new Caveman(spawnX, spawnY);
    scene->addItem(caveman);

    p1->canMove = true;
    cinematicPlaying = false;
}

void Level5::startTrollAttack() {
    trollAttackActive = true;

    Message* warn = new Message("Troll Incoming!", 2000);
    warn->showMessage(scene, 350, 280);

    int spawnX = scene->width() + 50;
    int trollHeight = 100;
    int spawnY = p1->pos().y() + p1->boundingRect().height() - trollHeight;

    troll = new Troll(spawnX, spawnY);
    scene->addItem(troll);
}

void Level5::update() {
    checkCavemanAttack();
    checkTrollAttack();

    if (cavemanAttackActive && caveman && !cinematicPlaying && !battleStarted) {
        qreal distance = QLineF(p1->pos(), caveman->pos()).length();
        if (distance < 200) {
            startBattle();
        }
    }
    if (trollAttackActive && troll && !battleStarted && troll->isAlive()) {
        qreal distance = QLineF(p1->pos(), troll->pos()).length();
        if (distance < 200) {
            startTrollAttack();
        }
    }
}

void Level5::startBattle() {
    battleStarted = true;
    cinematicPlaying = true;
    Player::bossBattle = true;

    p1->canMove = false;

    auto views = scene->views();
    if (views.isEmpty()) return;
    QGraphicsView* view = views.first();

    origTransform = view->transform();

    QPointF playerPos = p1->pos();
    QPointF cavemanPos = caveman->pos();
    QPointF midPoint = (playerPos + cavemanPos) / 2;

    view->centerOn(midPoint);

    // Construct the zoom transform with proper center of scaling
    QTransform zoomTransform;
    zoomTransform.translate(midPoint.x(), midPoint.y());
    zoomTransform.scale(2.0, 2.0);
    zoomTransform.translate(-midPoint.x(), -midPoint.y());

    zoomInAnim = new QPropertyAnimation(view, "transform", this);
    zoomInAnim->setDuration(500);
    zoomInAnim->setStartValue(origTransform);
    zoomInAnim->setEndValue(zoomTransform);

    connect(zoomInAnim, &QPropertyAnimation::finished, this, [this]() {
        fightTimer = new QTimer(this);
        connect(fightTimer, &QTimer::timeout, this, &Level5::autoFight);
        fightTimer->start(300);
    });

    zoomInAnim->start();
}


void Level5::autoFight() {
    if (!caveman) return;

    // First condition: Check if caveman takes damage and dies
    caveman->takeDamage(25);
    if (!caveman->isAlive()) {
        if (fightTimer) {
            fightTimer->stop();
            fightTimer->deleteLater();
            fightTimer = nullptr;
        }

        caveman->setPixmap(QPixmap(":/Enemies/deadcaveman.png").scaled(caveman->boundingRect().size().toSize()));
        caveman->setVisible(true);

        endBattle();
        return;
    }

    // Second condition: Player takes damage and check health afterward
    p1->takeDamage(10);
    if (p1->getHealth() <= 0) {
        if (fightTimer) {
            fightTimer->stop();
            fightTimer->deleteLater();
            fightTimer = nullptr;
        }

        p1->canMove = false;

        Message* gameOverMsg = new Message("Game Over", 2000);
        gameOverMsg->showMessage(scene, 300, 350);

        QTimer::singleShot(2000, this, [this]() {
            resetLevel();
        });

        return;
    }
}

void Level5::autoTrollFight() {
    if (!troll) return;

    troll->takeDamage(25);
    if (!troll->isAlive()) {
        if (fightTimer) {
            fightTimer->stop();
            fightTimer->deleteLater();
            fightTimer = nullptr;
        }

        troll->setPixmap(QPixmap(":/Enemies/deadcaveman.png").scaled(troll->boundingRect().size().toSize()));
        troll->setVisible(true);

        endBattle();
        return;
    }

    p1->takeDamage(10);
    if (p1->getHealth() <= 0) {
        if (fightTimer) {
            fightTimer->stop();
            fightTimer->deleteLater();
            fightTimer = nullptr;
        }

        p1->canMove = false;

        Message* gameOverMsg = new Message("Game Over", 2000);
        gameOverMsg->showMessage(scene, 300, 350);

        QTimer::singleShot(2000, this, [this]() {
            resetLevel();
        });

        return;
    }
}

void Level5::endBattle() {
    battleStarted = false;
    cinematicPlaying = false;
    Player::bossBattle = false;

    p1->canMove = true;

    auto views = scene->views();
    if (views.isEmpty()) return;
    QGraphicsView* view = views.first();

    zoomOutAnim = new QPropertyAnimation(view, "transform", this);
    zoomOutAnim->setDuration(500);
    zoomOutAnim->setStartValue(view->transform());
    zoomOutAnim->setEndValue(origTransform);

    connect(zoomOutAnim, &QPropertyAnimation::finished, this, [this]() {
        // Zoom out finished
    });

    zoomOutAnim->start();
}

void Level5::resetLevel() {
    static bool resetting = false;
    if (resetting) return;
    resetting = true;

    if (fightTimer) {
        fightTimer->stop();
        fightTimer->deleteLater();
        fightTimer = nullptr;
    }

    QList<QGraphicsItem*> itemsToRemove;
    for (auto* item : scene->items()) {
        if (dynamic_cast<Caveman*>(item) || dynamic_cast<Troll*>(item) || dynamic_cast<CaveCreature*>(item) || dynamic_cast<Diamond*>(item) || obstacles.contains(item)) {
            itemsToRemove.append(item);
        }
    }

    for (auto* item : itemsToRemove) {
        if (scene->items().contains(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    obstacles.clear();

    p1->setHealth(100);
    p1->setPosition(50, 400);
    p1->resetDiamonds();
    p1->canMove = true;

    if (p1->isFrozenBySpaceship()) {
        p1->unfreezeFromSpaceship();
    }
    p1->setFlag(QGraphicsItem::ItemIsFocusable, true);
    p1->setFocus();

    cavemanAttackActive = false;
    trollAttackActive = false;
    battleStarted = false;
    cinematicPlaying = false;

    setupLevel();

    resetting = false;
}
