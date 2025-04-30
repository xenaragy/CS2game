#include "home.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>

Home::Home(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);  // Set fixed size for the window
    setupUI();
}

void Home::setupUI()
{
    // Set the background image for the home page
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/backgrounds/homepage.jpeg").scaled(size()));
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();  // Put the background behind everything else

    // Create Level 1 portal button
    QPushButton* portalBtn = new QPushButton(this);
    portalBtn->setIcon(QIcon(":/backgrounds/portal.png"));
    portalBtn->setIconSize(QSize(200, 200));  // Adjust the size of the portal image
    portalBtn->setFixedSize(200, 200);        // Adjust the button size
    portalBtn->setFlat(true);                  // Remove the default button border

    QLabel* portalText = new QLabel("Level 1", this);
    portalText->setAlignment(Qt::AlignCenter);  // Center the text inside the label
    portalText->setStyleSheet("font-size: 18px; font-weight: bold; color: green; background-color: transparent;");
    portalText->setGeometry(90, 150, 120, 140);  // Set the same geometry as the image for proper overlay

    // Manually position the portal button at a specific location (e.g., near the top-left corner)
    portalBtn->move(50, 100);  // Move the button to position (50, 100)

    // Connect the first portal button to start level 1
    connect(portalBtn, &QPushButton::clicked, this, &Home::startLevel1);
}

void Home::startLevel1()
{
    MainWindow* gameWindow = new MainWindow(); // Create MainWindow instance
    gameWindow->show();                       // Show the MainWindow (game)
    this->close();                            // Close the Home screen
}
