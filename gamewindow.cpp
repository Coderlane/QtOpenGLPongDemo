#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QWidget(parent), ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    showFullScreen(); // Full Screen, full attention. Don't even need to worry about resizing.
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::BlankCursor); // Hide the cursor, full attention
    connect(ui->glWidget, SIGNAL(closeGame()), this, SLOT(close()));
    connect(ui->glWidget, SIGNAL(closeGame()), this, SIGNAL(closeGameWindow()));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::keyPressEvent(QKeyEvent *pEvent)
{
    if(pEvent->key() == Qt::Key_Escape)
    {
        setCursor(Qt::ArrowCursor); // Unhide the cursor
        // Pause the game when ESC is pressed
        ui->glWidget->pause();
        if(QMessageBox::question(this, "Exit", "Would you like to exit", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            // Exit
            emit closeGameWindow();
            this->close(); //Probably not necessary, but I'm pretty sure everything will be gone.
        }
        // Back to the action!
        ui->glWidget->unpause();
        setCursor(Qt::BlankCursor); // Back to hiding
    }
    else if(pEvent->key() == Qt::Key_Q || pEvent->key() == Qt::Key_Up)
    {
        ui->glWidget->lPadelUp();
    }
    else if(pEvent->key() == Qt::Key_A || pEvent->key() == Qt::Key_Down)
    {
        ui->glWidget->lPadelDown();
    }
    else
        QWidget::keyPressEvent(pEvent); //If it isn't some key we are listening for, let QWidget handle it.
}

void GameWindow::keyReleaseEvent( QKeyEvent* pEvent )
{
    if(pEvent->key() == Qt::Key_Q || pEvent->key() == Qt::Key_Up)
    {
        ui->glWidget->lPadelUpRel();
    }
    else if(pEvent->key() == Qt::Key_A || pEvent->key() == Qt::Key_Down)
    {
        ui->glWidget->lPadelDownRel();
    }
    else
        QWidget::keyReleaseEvent(pEvent);


}
