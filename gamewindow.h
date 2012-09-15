#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLayout>
#include "glwidget.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
protected:
    void keyPressEvent( QKeyEvent* pEvent );
    void keyReleaseEvent( QKeyEvent* pEvent );
    
private:
    Ui::GameWindow *ui;

public slots:

signals:
    void closeGameWindow();
};

#endif // GAMEWINDOW_H
