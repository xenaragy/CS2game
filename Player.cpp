#include "Player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPainter>

Player::Player() : health(100), coins(0), isJumping(false), isCrouching(false),
    attacking(false), isRight(false), isLeft(false), isOnGround(true),
    speedBoostEnabled(false), shieldEnabled(false), superAttackEnabled(false),
    shieldDisplayed(false),  swordFlame(nullptr) {

    // Load all pixmaps first
    QPixmap standingPixmap(":/Character/playerstanding.png");
    QPixmap runningRightPixmap(":/Character/runningright.png");
    QPixmap runningLeftPixmap(":/Character/runningleft.png");
    QPixmap crouchPixmap(":/Character/playercrouch.png");
    QPixmap attackPixmap(":/Character/playersword.png");
    QPixmap speedRightPixmap(":/Character/superRun.png");
    QPixmap speedLeftPixmap(":/Character/superRunLeft.png");
    QPixmap speedJumpPixmap(":/Character/superJump.png");
    QPixmap shieldPixmap(":/Character/playerShield.png");
    QPixmap shieldLeftPixmap(":/Character/playerShieldLeft.png");

    // Define the exact size ALL images must have
    int exactWidth = 150;
    int exactHeight = 100;

    // Scale all images to exactly the same dimensions - ignoring aspect ratio
    standingImage = standingPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    runningRightImage = runningRightPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    runningLeftImage = runningLeftPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    crouchImage = crouchPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    attackImage = attackPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    speedRightImage = speedRightPixmap.scaled(70, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    speedLeftImage = speedLeftPixmap.scaled(70, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    speedJumpImage = speedJumpPixmap.scaled(85, 105, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    shieldImage = shieldPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    shieldLeftImage = shieldLeftPixmap.scaled(exactWidth, exactHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Print debug info to verify all images are exactly the same size
    qDebug() << "ALL IMAGE SIZES:";
    qDebug() << "  Standing:" << standingImage.size();
    qDebug() << "  Running right:" << runningRightImage.size();
    qDebug() << "  Running left:" << runningLeftImage.size();
    qDebug() << "  Crouch:" << crouchImage.size();
    qDebug() << "  Attack:" << attackImage.size();
    qDebug() << "  Speed right:" << speedRightImage.size();
    qDebug() << "  Speed left:" << speedLeftImage.size();
    qDebug() << "  Speed jump:" << speedJumpImage.size();
    qDebug() << "  Shield:" << shieldImage.size();
    qDebug() << "  Shield left:" << shieldLeftImage.size();

    // Set initial appearance
    setPixmap(standingImage);

    setPos(100, 400);
    dropletsCollected = 0;

    // Set default movement parameters
    normalMoveSpeed = 25;        // Your current movement speed
    speedBoostMoveSpeed = 50;    // 50% faster
    normalJumpVelocity = 15;     // Your current jump velocity
    speedBoostJumpVelocity = 20; // 30% higher jump

    // Initialize ability timers
    speedBoostTimer = new QTimer(this);
    speedBoostTimer->setSingleShot(true);

    shieldTimer = new QTimer(this);
    shieldTimer->setSingleShot(true);

    superAttackTimer = new QTimer(this);
    superAttackTimer->setSingleShot(true);

    // Initialize jump and gravity timers
    velocityY = 0;
    jumpTimer = new QTimer(this);
    groundY = y();
    connect(jumpTimer, &QTimer::timeout, this, [this]() {
        setPos(x(), y() - velocityY);
        velocityY -= 1;
        if (velocityY <= 0) {
            jumpTimer->stop();
        }
    });

    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Player::applyGravity);
    gravityTimer->start(25);
}


Player::~Player() {  removeSwordFlame(); }

// Update the movement and state methods to handle shield display state:
void Player::moveForward() {
    isRight = true;
    // Use speed boost if enabled
    int moveSpeed = speedBoostEnabled ? speedBoostMoveSpeed : normalMoveSpeed;
    if (x() < 600) setPos(x() + moveSpeed, y());

    // Set the appropriate pixmap based on shield display state
    if (shieldDisplayed && shieldEnabled) {
        setPixmap(shieldImage);
    } else if (speedBoostEnabled) {
        setPixmap(speedRightImage);
    } else {
        setPixmap(runningRightImage);
    }
}

void Player::moveBackward() {
    isLeft = true;
    // Use speed boost if enabled
    int moveSpeed = speedBoostEnabled ? speedBoostMoveSpeed : normalMoveSpeed;
    if (x() > 100) setPos(x() - moveSpeed, y());

    // Set the appropriate pixmap based on shield display state
    if (shieldDisplayed && shieldEnabled) {
        setPixmap(shieldLeftImage);
    } else if (speedBoostEnabled) {
        setPixmap(speedLeftImage);
    } else {
        setPixmap(runningLeftImage);
    }
}

void Player::jump() {
    if (isOnGround) {
        isJumping = true;
        isOnGround = false;
        // Use boosted jump if speed boost is enabled
        velocityY = speedBoostEnabled ? speedBoostJumpVelocity : normalJumpVelocity;
        jumpTimer->start(20);

        // Set the appropriate pixmap based on shield display state
        if (shieldDisplayed && shieldEnabled) {
            if (isLeft) {
                setPixmap(shieldLeftImage);
            } else {
                setPixmap(shieldImage);
            }
        } else if (speedBoostEnabled) {
            setPixmap(speedJumpImage);
        } else {
            setPixmap(standingImage);
        }
    }
}

void Player::crouch() {
    isCrouching = true;
    setPixmap(crouchImage);
}

void Player::attack() {
    attacking = true;

    // If super attack is active, use a modified attack image or effect
    if (superAttackEnabled) {
        // Could use a different attack image for super attacks
        // For now, we'll just add a reddish tint to the attack image
        QPixmap redAttack = attackImage.copy();

        // Apply a red tint
        QPainter painter(&redAttack);
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(redAttack.rect(), QColor(255, 0, 0, 90));
        painter.end();

        setPixmap(redAttack);

        // Also add a message or sound effect for super attack
        if (scene()) {
            Message* attackMsg = new Message("SUPER ATTACK!", 500);
            attackMsg->showMessage(scene(), x(), y() - 50);
        }
    } else {
        // Regular attack
        setPixmap(attackImage);
    }

    QTimer::singleShot(500, this, [this]() {
        attacking = false;

        // Return to appropriate image
        if (isRight) {
            if (speedBoostEnabled) {
                setPixmap(speedRightImage);
            } else if (shieldEnabled) {
                setPixmap(shieldImage);
            } else {
                setPixmap(runningRightImage);
            }
        } else if (isLeft) {
            if (speedBoostEnabled) {
                setPixmap(speedLeftImage);
            } else if (shieldEnabled) {
                setPixmap(shieldLeftImage);
            } else {
                setPixmap(runningLeftImage);
            }
        } else {
            if (shieldEnabled) {
                setPixmap(shieldImage);
            } else {
                setPixmap(standingImage);
            }
        }
    });
}



void Player::setPosition(int x, int y) {
    setPos(x, y);
    groundY = y;
    isOnGround = true;
    velocityY = 0;

    if (jumpTimer->isActive()) {
        isJumping = false;
        jumpTimer->stop();
    }
}

void Player::takeDamage(int damage) {
    // Apply shield damage reduction if active
    if (shieldEnabled) {
        damage = damage / 2; // 50% damage reduction
    }
    health -= damage;
    if (health < 0) health = 0;
}

void Player::takeDamagePercent(float percent) {
    int dmg = static_cast<int>(percent * 100);
    // Apply shield damage reduction if active
    if (shieldEnabled) {
        dmg = dmg / 2; // 50% damage reduction
    }
    health -= dmg;
    if (health < 0) health = 0;
}

void Player::heal(int healthPoints) {
    health += healthPoints;
    if (health > 100) health = 100;
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int h) {
    health = h;
}

// Update the keyPressEvent method to handle the S key:
void Player::keyPressEvent(QKeyEvent* event) {
    // If game is paused, ignore movement keys
    if (paused) {
        return;
    }
    if (frozenBySpaceship) {
        return;  // Don't process key events if frozen
    }
    if (event->key() == Qt::Key_Right) moveForward();
    else if (event->key() == Qt::Key_Left) moveBackward();
    else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) jump();
    else if (event->key() == Qt::Key_Down) crouch();
    else if (event->key() == Qt::Key_A) attack();
    else if (event->key() == Qt::Key_S) toggleShield();  // New S key handler
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    if (paused) {
        return;
    }
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_A ||
        event->key() == Qt::Key_Left || event->key() == Qt::Key_Right
        || event->key()==Qt::Key_S) {
        isCrouching = false;
        attacking = false;
        shieldDisplayed = false;

        // If it was a movement key that was released
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
            isRight = false;
            isLeft = false;

            // Set standing image based on abilities
            if (shieldEnabled && shieldDisplayed) {
                setPixmap(shieldImage);
            } else {
                setPixmap(standingImage);
            }
        }
    }
}

void Player::incrementDroplets() {
    dropletsCollected++;
}

int Player::getCollectedDroplets() const {
    return dropletsCollected;
}

void Player::setDropletsCollected(int count) {
    dropletsCollected = count;
}

bool Player::canTakeDamage(int cooldownMs) {
    if (!damageTimer.isValid()) {
        damageTimer.start();
        return true;
    }

    if (damageTimer.elapsed() >= cooldownMs) {
        damageTimer.restart();
        return true;
    }

    return false;
}

void Player::incrementApples() {
    collectedApples++;
}

int Player::getCollectedApples() const {
    return collectedApples;
}

void Player::resetDroplets() {
    dropletsCollected = 0;
}

void Player::resetApples() {
    collectedApples = 0;
}


void Player::incrementSnowflakes() {
    collectedSnowflakes++;
}

int Player::getCollectedSnowflakes() const {
    return collectedApples;
}

void Player::resetSnowflakes() {
    collectedSnowflakes = 0;
}


void Player::incrementStars() {
    collectedStars++;
}

int Player::getCollectedStars() const {
    return collectedStars;
}

void Player::resetStars() {
    collectedStars = 0;
}

void Player::incrementDiamonds() {
    collectedDiamonds++;
}

int Player::getCollectedDiamonds () const{
    return collectedDiamonds;
}

void Player::resetDiamonds() {
    collectedDiamonds = 0;
}


void Player::applyGravity() {
    if (jumpTimer->isActive()) {
        return;
    }

    if (isOnGround) {
        bool stillOnPlatform = false;

        if (y() == groundY) {
            stillOnPlatform = true;
        } else {
            QList<QGraphicsItem*> colliding = collidingItems();
            for (QGraphicsItem* item : colliding) {
                if (item == this) continue;
                if (item->data(0) == "platform") {
                    stillOnPlatform = true;
                    break;
                }
            }
        }

        if (!stillOnPlatform) {
            isOnGround = false;
            velocityY = 0;
        } else {
            return;
        }
    }

    velocityY += gravity;
    if (velocityY > 15) velocityY = 15;
    setPos(x(), y() + velocityY);

    QList<QGraphicsItem*> colliding = collidingItems();
    for (QGraphicsItem* item : colliding) {
        if (item == this) continue;
        if (item->data(0) == "platform" && velocityY > 0 && y() + boundingRect().height() >= item->y() - 2) {
            // setY(item->y() - boundingRect().height());
            isOnGround = true;
            velocityY = 0;
            return;
        }
    }

    if (y() >= groundY) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }

    if (y() > 1000) {
        setY(groundY);
        isOnGround = true;
        velocityY = 0;
    }
}

void Player::resetPlayer() {
    isJumping = false;
    isCrouching = false;
    attacking = false;
    isRight = false;
    isLeft = false;
    isOnGround = true;

    velocityY = 0;
    setPos(100, 400);

    health = 100;
    collectedApples = 0;
    collectedSnowflakes = 0;
    dropletsCollected = 0;
    collectedStars = 0;
    collectedDiamonds = 0;
}

void Player::freezeBySpaceship() {
    qDebug() << "PLAYER FROZEN by spaceship";
    // Store original state
    wasFocusable = flags() & QGraphicsItem::ItemIsFocusable;

    // Disable movement
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    frozenBySpaceship = true;
}

void Player::unfreezeFromSpaceship() {
    qDebug() << "PLAYER UNFROZEN from spaceship";

    // Only try to set flags if we're in a scene
    if (scene()) {
        // Restore to original state
        if (wasFocusable) {
            setFlag(QGraphicsItem::ItemIsFocusable, true);
            setFocus();
        }
    }
    frozenBySpaceship = false;
}

bool Player::isFrozenBySpaceship() const {
    return frozenBySpaceship;
}
void Player::incrementCoins(int amount) {
    coins += amount;
}

int Player::getCoins() const {
    return coins;
}

void Player::setCoins(int amount) {
    coins = amount;
}

void Player::spendCoins(int amount) {
    coins -= amount;
    if (coins < 0) coins = 0;
}

void Player::resetCoins() {
    coins = 0;
}
// Add this function to your Player.cpp file:

// Now modify your enableSpeedBoost method to call this:

void Player::enableSpeedBoost(int seconds) {

    speedBoostEnabled = true;

    // Disconnect any previous connections
    disconnect(speedBoostTimer, &QTimer::timeout, nullptr, nullptr);

    // Connect timeout signal
    connect(speedBoostTimer, &QTimer::timeout, [this]() {
        speedBoostEnabled = false;

        // Update appearance based on current state
        if (isJumping || !isOnGround) {
            setPixmap(standingImage);
        } else if (isRight) {
            setPixmap(runningRightImage);
        } else if (isLeft) {
            setPixmap(runningLeftImage);
        } else {
            setPixmap(standingImage);
        }

        if (scene()) {
            Message* msg = new Message("Speed Boost expired!", 2000);
            msg->showMessage(scene(), 300, 100);
        }
    });

    // Start timer
    speedBoostTimer->start(seconds * 1000);

    // Update current image based on state
    if (isJumping || !isOnGround) {
        setPixmap(speedJumpImage);
    } else if (isRight) {
        setPixmap(speedRightImage);
    } else if (isLeft) {
        setPixmap(speedLeftImage);
    } else {
        setPixmap(standingImage);
    }

    if (scene()) {
        Message* msg = new Message("Speed Boost activated!", 2000);
        msg->showMessage(scene(), 300, 100);
    }
}

void Player::enableShield(int seconds) {
    shieldEnabled = true;
    shieldDisplayed = false;

    // Disconnect any previous connections to avoid multiple connections
    disconnect(shieldTimer, &QTimer::timeout, nullptr, nullptr);

    // Connect timeout signal
    connect(shieldTimer, &QTimer::timeout, [this]() {
        shieldEnabled = false;


        if (scene()) {
            Message* msg = new Message("Shield expired!", 2000);
            msg->showMessage(scene(), 300, 100);
        }
    });

    // Start timer
    shieldTimer->start(seconds * 1000);
\
    if (scene()) {
        Message* msg = new Message("Shield activated!", 2000);
        msg->showMessage(scene(), 300, 100);
    }
}

void Player::enableSuperAttack(int seconds) {
    superAttackEnabled = true;

    // Add a sword flame overlay
    addSwordFlame();

    // Disconnect any previous connections to avoid multiple connections
    disconnect(superAttackTimer, &QTimer::timeout, nullptr, nullptr);

    // Connect timeout signal
    connect(superAttackTimer, &QTimer::timeout, [this]() {
        superAttackEnabled = false;

        if (scene()) {
            Message* msg = new Message("Super Attack expired!", 2000);
            msg->showMessage(scene(), 300, 100);
        }
    });

    // Start timer
    superAttackTimer->start(seconds * 1000);

    // Show activation message
    if (scene()) {
        Message* msg = new Message("Super Attack activated!", 2000);
        msg->showMessage(scene(), 300, 100);
    }
}
bool Player::hasSpeedBoost() const {
    return speedBoostEnabled;
}

bool Player::hasShield() const {
    return shieldEnabled;
}

bool Player::hasSuperAttack() const {
    return superAttackEnabled;
}
void Player::toggleShield() {
    // Only work if shield boost is enabled
    if (!shieldEnabled) {
        return;
    }

    // Toggle shield display state
    shieldDisplayed = !shieldDisplayed;

    // Update appearance based on new state
    if (shieldDisplayed) {
        // Show appropriate shield image based on direction
        if (isLeft) {
            setPixmap(shieldLeftImage);
        } else {
            setPixmap(shieldImage);
        }

        // Display a small message that shield is visible
        if (scene()) {
            Message* msg = new Message("Shield raised!", 500);
            msg->showMessage(scene(), 300, 100);
        }
    } else {
        // Return to normal appearance based on current state
        if (isJumping || !isOnGround) {
            if (speedBoostEnabled) {
                setPixmap(speedJumpImage);
            } else {
                setPixmap(standingImage);
            }
        } else if (isRight) {
            if (speedBoostEnabled) {
                setPixmap(speedRightImage);
            } else {
                setPixmap(runningRightImage);
            }
        } else if (isLeft) {
            if (speedBoostEnabled) {
                setPixmap(speedLeftImage);
            } else {
                setPixmap(runningLeftImage);
            }
        } else {
            if (speedBoostEnabled) {
                setPixmap(speedRightImage);
            } else {
                setPixmap(standingImage);
            }
        }

        // Display a small message that shield is lowered
        if (scene()) {
            Message* msg = new Message("Shield lowered.", 500);
            msg->showMessage(scene(), 300, 100);
        }
    }
}
// Add a flame overlay on the player's sword
void Player::addSwordFlame() {
    // Remove any existing flame
    removeSwordFlame();

    // Load flame image (you can create this or use an existing one)
    QPixmap flamePixmap(":/Rewards/sword.png"); // Use existing sword image

    // If we don't have a flame image, we can create a colored overlay
    if (flamePixmap.isNull()) {
        // Create a colored rectangle
        flamePixmap = QPixmap(30, 30);
        flamePixmap.fill(Qt::transparent);

        QPainter painter(&flamePixmap);
        QRadialGradient gradient(15, 15, 15);
        gradient.setColorAt(0, QColor(255, 200, 0, 200)); // Yellow center
        gradient.setColorAt(0.7, QColor(255, 50, 0, 150)); // Orange-red
        gradient.setColorAt(1, QColor(255, 0, 0, 0));    // Transparent edge

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 30, 30);
    }

    // Create the flame overlay
    swordFlame = new QGraphicsPixmapItem(flamePixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation), this);

    // Position it near the player's hand/weapon
    swordFlame->setPos(boundingRect().width()/2, boundingRect().height()/2);
    swordFlame->setZValue(1); // Above player
}

// Remove the flame effect
void Player::removeSwordFlame() {
    if (swordFlame) {
        // Just remove it from this item, no need to remove from scene
        delete swordFlame;
        swordFlame = nullptr;
    }
}
bool Player::bossBattle = false;

