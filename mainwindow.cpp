#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainGameWindow = NULL;
    connect(ui->launchButton, SIGNAL(clicked()), this, SLOT(launchGameWindow()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    setFixedSize(width(),height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launchGameWindow()
{
    if(mainGameWindow == NULL) //No window? Make a new one
    {
        mainGameWindow = new GameWindow();  // Could allow for custom size windows, but for now lets just do full screen.
        connect(mainGameWindow, SIGNAL(closeGameWindow()), this, SLOT(closeGameWindow()));
        this->hide();
    }
}
void MainWindow::closeGameWindow()
{
    this->close();
}
