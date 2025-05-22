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
        return;
    }
    scene->addItem(this);
    setPos(x, y);
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true); //how long a message should stay on the screen
    timer->setInterval(displayDuration);//sets interval to the set duration
    connect(timer, &QTimer::timeout, this, &Message::onTimeout); //when the duration passes, time out is called and the msg dissapears
    this->scene = scene;
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
    QFont font = msg->font();
    font.setPointSize(20);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}

Message* Message::killedAlienBonus() {
    Message* msg = new Message("Health Bonus! Alien killed successfuly", 3000);
    QFont font = msg->font();
    font.setPointSize(20);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}
Message* Message::HintMessagelevel3() {
    Message* msg = new Message("Kill all penguins to proceed", 3000);
    QFont font = msg->font();
    font.setPointSize(30);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::red);
    return msg;
}
Message* Message::HintMessagelevel4(){
    Message* msg = new Message("Kill all aliens to proceed", 3000);
    QFont font = msg->font();
    font.setPointSize(30);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::red);
    return msg;
}
Message* Message::HintMessagelevel5(){
    Message* msg = new Message("Kill all monsters to proceed", 3000);
    QFont font = msg->font();
    font.setPointSize(30);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::red);
    return msg;
}
Message* Message::killedCaveCreatureBonus() {
    Message* msg = new Message("Health Bonus! Cave creature killed successfuly", 3000);
    QFont font = msg->font();
    font.setPointSize(20);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}
Message* Message::killedTrollBonus(){
    Message* msg = new Message("Health Bonus! Troll killed successfuly", 3000);
    QFont font = msg->font();
    font.setPointSize(20);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}
Message* Message::KilledCaveManBonus(){
    Message* msg = new Message("Health Bonus! Cave man killed successfuly", 3000);
    QFont font = msg->font();
    font.setPointSize(20);
    msg->setFont(font);
    msg->setDefaultTextColor(Qt::green);
    return msg;
}
