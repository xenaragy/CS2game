#include "Message.h"
#include <QGraphicsScene>

              Message::Message(const QString& text, int duration, QGraphicsItem* parent)
    : QGraphicsTextItem(parent), displayDuration(duration) {
    setPlainText(text);
    setDefaultTextColor(Qt::red);
    QFont font = this->font();
    font.setPointSize(30);
    font.setBold(true);
    setFont(font);
}

void Message::showMessage(QGraphicsScene* scene, int x, int y) {
    scene->addItem(this);
    setPos(x, y);

    QTimer::singleShot(displayDuration, [scene, this]() {
        scene->removeItem(this);
        delete this;
    });
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
    return new Message("Collects all Earths", 3000);
}

Message* Message::createLevelFourCompleteMessage() {
    return new Message("Space Level Completed!", 3000);
}
