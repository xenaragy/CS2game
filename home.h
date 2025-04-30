#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);

private slots:
    void startLevel1();

private:
    void setupUI();
    QLabel* backgroundLabel;
};

#endif // HOME_H
