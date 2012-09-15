#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QTimer>
#include <cmath>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>

// Constants
const double ballR = .5;
const double ballS = 40;
const double cBallDX = .1;
const double cBallDY = .1;
const double sBallDX = .05;
const double sBallDY = .05;
const double ballDDY = .005;
const double ballDDX = .005;
const double ballLevelDDX = .00005;
const double ballLevelDDY = .00005;

const double lPadelX = -14;
const double rPadelX = 14;
const double padelY = 1;
const double padelX = .2;
const double padelV = .4;
const double lPadelDYMAX = .4;
const double rPadelDYMAX = .2;
const double rPadelDDYMAX = .005;
const double rPadelLevelDDYMAX = .0005;
const double rPadelLevelDYMAX = .02;

const double lWall = -15;
const double rWall = 15;
const double tWall = 10;
const double bWall = -10;

const double pi = 3.14159265;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    // ***** Functions *****
    // Ball
    GLuint makeBall(double r = ballR);
    void drawBall(GLuint b, double dx, double dy);

    // Padel
    GLuint makePadel();
    void drawPadel(GLuint p, double dx, double dy);

    // Wall
    GLuint makeWall();
    void drawWall(GLuint);

    //MISC
    void setupGame();
    void newGame();
    void calcMove();
    void compMove();
    void gameOver();


    // ***** Variables *****
    // Padel
    GLuint lPadel;
    GLuint rPadel;
    double lPadelY;
    double lPadelDY;
    double rPadelY;
    double rPadelDY;


    // Ball
    GLuint ballID;
    double ballX;
    double ballY;
    double ballDX;
    double ballDY;

    // Wall
    GLuint wallID;

    // Score
    int level;
    int lives;

    // FPS Stuff
    double fps;
    int pTime;
    int frameCount;

    // MISC
    QTimer *timer;
    QFontDatabase fontDatabase;

signals:
    void closeGame();
    
public slots:
    void loop();

    void pause();
    void unpause();

    void lPadelUp();
    void lPadelDown();
    void lPadelUpRel();
    void lPadelDownRel();
};

#endif // GLWIDGET_H
