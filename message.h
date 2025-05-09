#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsTextItem>
#include <QTimer>
#include <QGraphicsScene>
#include <QString>

            class Message : public QGraphicsTextItem {
public:
    Message(const QString& text, int duration = 3000, QGraphicsItem* parent = nullptr);
    void showMessage(QGraphicsScene* scene, int x, int y);

    static Message* createLevelStartMessage(int levelNumber);
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




private:
    int displayDuration;
};

#endif // MESSAGE_H
