// #include "game.h"
// #include <QGraphicsTextItem>
// #include <QFont>
// #include <QApplication>
// #include "waterdroplet.h"

//     Game::Game(QWidget* parent) : QGraphicsView(parent), gameOver(false), score(0) {
//     // Create and configure the scene
//     scene = new QGraphicsScene(this);
//     scene->setSceneRect(0, 0, 800, 600); // Set scene size
//     setScene(scene);
//     setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//     setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//     setFixedSize(800, 600);

//     // Initialize player
//     player = new Player();

//     // Initialize level (starting with level 1)
//     currentLevel = new Level(1, scene, player);

//     // Set up game timer for game loop
//     gameTimer = new QTimer(this);
//     connect(gameTimer, &QTimer::timeout, this, &Game::update);

//     // Create UI elements
//     healthLabel = new QLabel("Health: 100", this);
//     healthLabel->setGeometry(10, 10, 100, 30);
//     healthLabel->setStyleSheet("color: white;");

//     levelLabel = new QLabel("Level: 1", this);
//     levelLabel->setGeometry(10, 40, 100, 30);
//     levelLabel->setStyleSheet("color: white;");

//     // Set focus policy so the view can receive keyboard events
//     setFocusPolicy(Qt::StrongFocus);
// }

// Game::~Game() {
//     delete player;
//     delete currentLevel;
//     delete gameTimer;
//     // scene will be deleted automatically by QGraphicsView
// }

// void Game::startGame() {
//     // Set up the first level
//     currentLevel->setupLevel();

//     // Start the game timer
//     gameTimer->start(16); // ~60 FPS

//     // Update UI elements
//     updateUI();
// }

// void Game::pauseGame() {
//     gameTimer->stop();

//     QGraphicsTextItem* pauseText = scene->addText("PAUSED");
//     pauseText->setPos(350, 250);
//     pauseText->setDefaultTextColor(Qt::black);
//     QFont font = pauseText->font();
//     font.setPointSize(150);
//     pauseText->setFont(font);
//     pauseText->setData(0, "pauseText"); // To identify this item later
// }

// void Game::resumeGame() {
//     // Remove the pause text
//     for (auto item : scene->items()) {
//         if (item->data(0).toString() == "pauseText") {
//             scene->removeItem(item);
//             delete item;
//             break;
//         }
//     }

//     // Resume the game timer
//     gameTimer->start(16);
// }

// void Game::restartGame() {
//     // Reset score
//     score = 0;

//     // Reset game over state
//     gameOver = false;

//     // Reset level to 1
//     delete currentLevel;
//     currentLevel = new Level(1, scene, player);

//     // Clear the scene
//     scene->clear();

//     // Start the game
//     startGame();
// }

// void Game::gameOverScreen() {
//     gameTimer->stop();
//     gameOver = true;

//     // Create "Game Over" text
//     QGraphicsTextItem* gameOverText = scene->addText("GAME OVER");
//     gameOverText->setPos(300, 200);
//     gameOverText->setDefaultTextColor(Qt::red);
//     QFont font = gameOverText->font();
//     font.setPointSize(36);
//     gameOverText->setFont(font);

//     // Create score text
//     QGraphicsTextItem* scoreText = scene->addText("Score: " + QString::number(score));
//     scoreText->setPos(340, 250);
//     scoreText->setDefaultTextColor(Qt::white);
//     font.setPointSize(24);
//     scoreText->setFont(font);

//     // Create restart instruction
//     QGraphicsTextItem* restartText = scene->addText("Press R to restart");
//     restartText->setPos(320, 300);
//     restartText->setDefaultTextColor(Qt::white);
//     font.setPointSize(18);
//     restartText->setFont(font);
// }

// void Game::updateUI() {
//     healthLabel->setText("Health: " + QString::number(player->getHealth()));
//     levelLabel->setText("Level: " + QString::number(currentLevel->getLevelNumber()));
// }

// void Game::checkCollisions() {
//     // Get player's position and bounding rect
//     QRectF playerRect = player->sceneBoundingRect();

//     // Check collisions with obstacles
//     for (auto item : scene->items()) {
//         // Skip the player itself
//         if (item == player) continue;

//         QRectF itemRect = item->sceneBoundingRect();

//         // If collision detected
//         if (playerRect.intersects(itemRect)) {
//             // Check what the player collided with
//             // You can use item->data() to store type information

//             // Example: if item is an enemy
//             // if (item->data(0).toString() == "enemy") {
//             //     player->takeDamage(10);
//             // }

//             // For now, let's just handle basic collision logic
//             // Prevent player from moving through obstacles
//             // Move player back slightly
//             if (playerRect.intersects(itemRect)) {
//                 // Simple collision resolution
//                 // Move player back from the obstacle
//                 double overlapX = qMin(playerRect.right(), itemRect.right()) - qMax(playerRect.left(), itemRect.left());
//                 double overlapY = qMin(playerRect.bottom(), itemRect.bottom()) - qMax(playerRect.top(), itemRect.top());

//                 if (overlapX < overlapY) {
//                     // Resolve X-axis collision
//                     if (player->x() < item->x()) {
//                         player->setPos(player->x() - overlapX, player->y());
//                     } else {
//                         player->setPos(player->x() + overlapX, player->y());
//                     }
//                 } else {
//                     // Resolve Y-axis collision
//                     if (player->y() < item->y()) {
//                         player->setPos(player->x(), player->y() - overlapY);
//                     } else {
//                         player->setPos(player->x(), player->y() + overlapY);
//                     }
//                 }
//             }
//         }
//     }

//     // Update UI to reflect any health changes
//     updateUI();

//     // Check if player is dead
//     if (player->getHealth() <= 0 && !gameOver) {
//         gameOverScreen();
//     }
// }

// void Game::advanceLevel() {
//     // Increase score for completing level
//     score += 1000;

//     // Move to next level
//     currentLevel->nextLevel();

//     // Update UI
//     updateUI();
// }

// void Game::keyPressEvent(QKeyEvent* event) {
//     if (gameOver) {
//         // If the game is over, only respond to the restart key
//         if (event->key() == Qt::Key_R) {
//             restartGame();
//         }
//         return;
//     }

//     switch (event->key()) {
//     case Qt::Key_P:
//         // Toggle pause
//         if (gameTimer->isActive()) {
//             pauseGame();
//         } else {
//             resumeGame();
//         }
//         break;
//     case Qt::Key_N:
//         // For testing: advance to next level
//         advanceLevel();
//         break;
//     case Qt::Key_R:
//         // Restart the game
//         restartGame();
//         break;
//     default:
//         // Pass other key events to the player
//         QGraphicsView::keyPressEvent(event);
//         break;
//     }
// }

// void Game::keyReleaseEvent(QKeyEvent* event) {
//     // Pass key release events to the player
//     QGraphicsView::keyReleaseEvent(event);
// }

// void Game::update() {
//     // This is the main game loop function that's called by the timer

//     // Check for collisions
//     checkCollisions();
//     for (QGraphicsItem* item : scene->items()) {
//         WaterDroplet* drop = dynamic_cast<WaterDroplet*>(item);
//         if (drop) {
//             drop->checkCollision(player);
//         }
//     }

//     // Check level completion (you'll need to define your own criteria)
//     // Example: if (allEnemiesDefeated && playerAtExit) {
//     //    advanceLevel();
//     // }

//     // Other game logic can go here:
//     // - Enemy AI updates
//     // - Physics simulation
//     // - Time-based events

//     // Update UI
//     updateUI();
// }
