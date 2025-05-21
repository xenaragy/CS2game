#include "Message.h"
#include <QGraphicsScene>

Message::Message(const QString& text, int duration, QGraphicsItem* parent)
    : QGraphicsTextItem(parent), displayDuration(duration) {
    setPlainText(text);
    setDefaultTextColor(Qt::white);
    QFont font = this->font();
    font.setPointSize(30);
    font.setBold(true);
    setFont(font);
}
void Message::showMessage(QGraphicsScene* scene, int x, int y) {
    if (!scene) {
        qDebug() << "Warning: Attempting to show message on null scene";
        return;
    }

    scene->addItem(this);
    setPos(x, y);

    // Instead of using a single-shot timer with a lambda that captures 'this',
    // use a regular timer and connect its signal to a safe slot
    QTimer* timer = new QTimer(this); // Parent to this so it gets deleted with the Message
    timer->setSingleShot(true);
    timer->setInterval(displayDuration);

    // Connect using safer signal-slot connection
    connect(timer, &QTimer::timeout, this, &Message::onTimeout);

    // Store the scene pointer for later use in onTimeout
    this->scene = scene;

    // Start the timer
    timer->start();
}

void Message::onTimeout() {
    if (scene) {
        scene->removeItem(this);
        delete this;
    }
}

Message* Message::createLevelStartMessage(int levelNumber) {
    return new Message("Level " + QString::number(levelNumber) + " Begins!", 3000);
}

Message* Message::createLevelCompleteMessage(int levelNumber) {
    return new Message("Level " + QString::number(levelNumber) + " Complete!", 3000);
}

Message* Message::createGameOverMessage() {
    return new Message("GAME OVER", 3000);
}


// New methods for Level 1 specific messages
Message* Message::createLevelOneStartMessage() {
    return new Message("Collect all droplets", 3000);
}

Message* Message::createLevelOneCompleteMessage() {
    return new Message("Level completed", 3000);
}

Message* Message::createLevelTwoStartMessage() {
    return new Message("Collect all Apples!", 3000);
}

Message* Message::createLevelTwoCompleteMessage() {
    return new Message("Forest Level Completed!", 3000);
}

Message* Message::createLevelThreeStartMessage() {
    return new Message("Collects all Snowflakes", 3000);
}

Message* Message::createLevelThreeCompleteMessage() {
    return new Message("Snow Level Completed!", 3000);
}

Message* Message::createLevelFourStartMessage() {
    return new Message("Collects all Stars", 3000);
}

Message* Message::createLevelFourCompleteMessage() {
    return new Message("Space Level Completed!", 3000);
}
Message* Message::createLevelFiveStartMessage() {
    return new Message("Collects all Diamonds", 3000);
}

Message* Message::createLevelFiveCompleteMessage() {
    return new Message("Final Level Completed!", 3000);
}

Message* Message::killedPinguinBonus() {
    Message* msg = new Message("Health Bonus! Pinguin killed successfuly", 3000);

    // Change font size internally
    QFont font = msg->font();
    font.setPointSize(20);  // set your smaller font size here
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}

Message* Message::killedAlienBonus() {
    Message* msg = new Message("Health Bonus! Alien killed successfuly", 3000);

    // Change font size internally
    QFont font = msg->font();
    font.setPointSize(20);  // set your smaller font size here
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);

    return msg;
}
Message* Message::HintMessagelevel3() {
    Message* msg = new Message("Kill all penguins to proceed", 3000);
    QFont font = msg->font();
    font.setPointSize(30);  // set your smaller font size here
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::red);
    return msg;
}
Message* Message::HintMessagelevel4(){
    Message* msg = new Message("Kill all aliens to proceed", 3000);
    QFont font = msg->font();
    font.setPointSize(30);  // set your smaller font size here
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::red);
    return msg;
}
