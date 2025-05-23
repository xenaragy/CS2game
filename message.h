#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsTextItem>
#include <QTimer>
#include <QGraphicsScene>
#include <QString>


class Message : public QGraphicsTextItem {
    Q_OBJECT
public:
    Message(const QString& text, int duration = 3000, QGraphicsItem* parent = nullptr);
    void showMessage(QGraphicsScene* scene, int x, int y);

    static Message* createLevelStartMessage(int levelNumber); //static allows us to create messages without calling an instance of the object
    static Message* createLevelCompleteMessage(int levelNumber);
    static Message* createGameOverMessage();
    static Message* createLevelOneStartMessage();
    static Message* createLevelOneCompleteMessage();
    static Message* createLevelTwoStartMessage();
    static Message* createLevelTwoCompleteMessage();
    static Message* createLevelThreeStartMessage();
    static Message* createLevelThreeCompleteMessage();
    static Message* createLevelFourStartMessage();
    static Message* createLevelFourCompleteMessage();
    static Message* createLevelFiveStartMessage();
    static Message* createLevelFiveCompleteMessage();
    static Message* killedPinguinBonus();
    static Message* killedAlienBonus();
    static Message* HintMessagelevel3();
    static Message* HintMessagelevel4();
    static Message* HintMessagelevel5();
    static Message* killedCaveCreatureBonus();
    static Message* killedTrollBonus();
    static Message* KilledCaveManBonus();
    void onTimeout();
private:
    int displayDuration;
    QGraphicsScene* scene = nullptr;
};

#endif // MESSAGE_H
