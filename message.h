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
    static Message* createLevelTwoStartMessage();           // Make sure this is here
    static Message* createLevelTwoCompleteMessage();


private:
    int displayDuration;
};

#endif // MESSAGE_H
