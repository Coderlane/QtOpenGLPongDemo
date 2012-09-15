#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    // Timer for the main loop
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->setInterval(20);
    timer->start();

    fontDatabase.addApplicationFont(":/DroidSans.ttf");
}
GLWidget::~GLWidget()
{
    // Free up memory
    glDeleteLists(ballID, 1);
    glDeleteLists(lPadel, 1);
    glDeleteLists(rPadel, 1);
    glDeleteLists(wallID, 1);
}

// Setup variables (After OpenGL has init
void GLWidget::setupGame()
{
    newGame();
    ballID = makeBall(ballR);
    lPadel = makePadel();
    rPadel = makePadel();
    wallID = makeWall();
}

// Game Loop
void GLWidget::loop()
{



    // Padel movement
    // Computer
    compMove();
    // Human
    if(lPadelDY != 0)
    {
        if(lPadelDY > 0)
        {
            if(lPadelY + padelY < tWall)
                lPadelY += lPadelDY;
            if(lPadelY + padelY > tWall)
                lPadelY = tWall - padelY;
        }
        else
        {
            if(lPadelY - padelY > bWall)
                lPadelY += lPadelDY;
            if(lPadelY - padelY < bWall)
                lPadelY = bWall + padelY;
        }
    }

    // Scoring Walls
    // LEFT
    if(ballX-ballR <= lWall)
    {
        ballX = 0;
        ballY = 0;
        ballDX = sBallDX;
        if(ballDY > 0)
            ballDY = sBallDY;
        else
            ballDY = -sBallDY;
        lives--;
        if(lives <= 0)
        {
            gameOver();
        }
    }
    //RIGHT
    else if(ballX+ballR >= rWall)
    {
        ballX = 0;
        ballY = 0;
        ballDX = -sBallDX;
        if(ballDY > 0)
            ballDY = sBallDY;
        else
            ballDY = -sBallDY;
        level++;
    }

    // Non-Scoring Walls
    if(ballY+ballR >= tWall)
    {
        ballY = tWall-ballR;
        ballDY = -ballDY - (ballDDY + (level*ballLevelDDY));
    }
    else if(ballY - ballR <= bWall)
    {
        ballY = bWall+ballR;
        ballDY = -ballDY + (ballDDY + (level*ballLevelDDY));
    }

    // Padel Tests
    // RIGHT PADEL
    if(ballX + ballR >= rPadelX - padelX)
    {
        if((ballY - ballR <= rPadelY + padelY) && (ballY + ballR >= rPadelY - padelY))
        {
            if(ballDX != cBallDX)
            {
                ballDX = cBallDX;
                if(ballDY > 0)
                    ballDY = cBallDY;
                else
                    ballDY = -cBallDY;
            }
            ballDX = -ballDX;
            ballX = rPadelX - (padelX + ballR);
        }
    }
    // LEFT PADEL
    if(ballX - ballR <= lPadelX + padelX)
    {
        if((ballY - ballR <= lPadelY + padelY) && (ballY + ballR >= lPadelY - padelY))
        {
            if(ballDX != -cBallDX)
            {
                ballDX = -cBallDX;
                if(ballDY > 0)
                    ballDY = cBallDY;
                else
                    ballDY = -cBallDY;
            }
            ballDX = -ballDX;
            ballX = lPadelX + (padelX + ballR);
        }
    }

    // Move Ball
    ballX += ballDX;
    ballY += ballDY;

   updateGL();
}

void GLWidget::initializeGL()
{
    // Init Stuff
    glShadeModel(GL_SMOOTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    setupGame();
}
void GLWidget::paintGL()
{
    qglClearColor(QColor(Qt::black));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Show Text
    glColor3f(0.0f, 1.0f, 0.0f);
    renderText( 10,  9 , 0, "Level: " + QString::number(level), QFont("DroidSans", 30, 10, false));
    renderText(-13, 9 , 0, "Lives: " + QString::number(lives), QFont("DroidSans", 30, 10, false));
    if(timer->isActive() == false)
        renderText(-.8,-9,0, "PAUSED", QFont("DroidSans", 20, 10, false));

    glPushMatrix();
    drawWall(wallID);
    drawBall(ballID, ballX, ballY);
    drawPadel(lPadel, lPadelX, lPadelY);
    drawPadel(rPadel, rPadelX, rPadelY);
    glPopMatrix();

}
void GLWidget::resizeGL(int width, int height)
{
    // Safety Check! Wear your hard hat
    if(width == 0)
        width = 1;
    if(height == 0)
        height = 1;
    double ratio = (width>height?((double)width/(double)height):((double)height/(double)width));
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0f,ratio,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40);

}

// Ball stuff
GLuint GLWidget::makeBall(double radius)
{
    //Start List
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    //Start Object
    glColor3f(0,1,0);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < ballS; i++)
    {
        glVertex2f(radius*cos(i*2*pi/ballS),radius*sin(i*2*pi/ballS));
    }
    // End Object
    glEnd();
    //End List
    glEndList();
    return list;
}
void GLWidget::drawBall(GLuint b, double dx, double dy)
{
    glPushMatrix();
    glColor3f(0, 1, 0); // Green
    glTranslated(dx, dy, 0);
    glCallList(b);
    glPopMatrix();
}

// Padel stuff
GLuint GLWidget::makePadel()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glColor3f(0, 1, 0); // Green
    // Rectangle
    glBegin(GL_LINE_LOOP);
    glVertex3f(-padelX, -padelY, 0.0f);
    glVertex3f(-padelX, padelY, 0.0f);
    glVertex3f(padelX, padelY, 0.0f);
    glVertex3f(padelX, -padelY, 0.0f);
    glEnd();
    glEndList();
    return list;
}
void GLWidget::drawPadel(GLuint p, double dx, double dy)
{
    glPushMatrix();
    glColor3f(0, 1, 0); // Green
    glTranslated(dx, dy, 0);
    glCallList(p);
    glPopMatrix();
}

// Border wall stuff
GLuint GLWidget::makeWall()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glColor3f(0, 1, 0); // Green
    // Rectangle!!!
    glBegin(GL_LINE_LOOP);
    glVertex3f(lWall, bWall, 0.0f);
    glVertex3f(lWall, tWall, 0.0f);
    glVertex3f(rWall, tWall, 0.0f);
    glVertex3f(rWall, bWall, 0.0f);
    glEnd();
    glEndList();
    return list;
}
void GLWidget::drawWall(GLuint w)
{
    glPushMatrix();
    glColor3f(0, 1, 0); // Green
    glTranslated(0, 0, 0); // Origin
    glCallList(w);
    glPopMatrix();
}

// Pause/Unpause the game
void GLWidget::pause()
{
    timer->stop();
    updateGL();
}
void GLWidget::unpause()
{
    timer->start();
    updateGL();
}

// Padel Button Code
void GLWidget::lPadelUp()
{
    lPadelDY = lPadelDYMAX;
}
void GLWidget::lPadelDown()
{
    lPadelDY = -lPadelDYMAX;
}
// Redundant
void GLWidget::lPadelUpRel()
{
    lPadelDY = 0;
}
void GLWidget::lPadelDownRel()
{
    lPadelDY = 0;
}

// Game Over
void GLWidget::gameOver()
{
    pause();
    setCursor(Qt::ArrowCursor);
    if(QMessageBox::question(this, "GAME OVER", "Level: " + QString::number(level) + "\n"+"Would you like to play another game?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        newGame();
        setCursor(Qt::BlankCursor);
        unpause();
    }
    else
        emit closeGame();
}

// New Game Stuff
void GLWidget::newGame()
{
    ballX = 0;
    ballY = 0;
    ballDX = .05;
    ballDY = .05;
    lPadelDY = 0;
    lPadelY = 0;
    rPadelY = 0;
    rPadelDY = 0;
    level = 1;
    lives = 5;
}

// Computer AI
void GLWidget::calcMove()
{
    if(ballY+padelY > rPadelY)
    {
        if(rPadelDY < 0)
            rPadelDY += 2*(rPadelDDYMAX + (level*rPadelLevelDDYMAX));
        else
            rPadelDY += rPadelDDYMAX + (level*rPadelLevelDDYMAX);

        if(rPadelDY > rPadelDYMAX + (level*rPadelLevelDYMAX))
            rPadelDY = rPadelDYMAX + (level*rPadelLevelDYMAX);
    }
    else if(ballY-padelY < rPadelY)
    {
        if(rPadelDY > 0)
             rPadelDY -= 2*(rPadelDDYMAX + (level*rPadelLevelDDYMAX));
        else
            rPadelDY -= rPadelDDYMAX + (level*rPadelLevelDDYMAX);

        rPadelDY -= rPadelDDYMAX;
        if(rPadelDY < -rPadelDYMAX - (level*rPadelLevelDYMAX))
            rPadelDY = -rPadelDYMAX - (level*rPadelLevelDYMAX);
    }
}
void GLWidget::compMove()
{
    calcMove();
    rPadelY += rPadelDY;
    if(rPadelY + padelY > tWall)
    {
        rPadelY = tWall - padelY;
        rPadelDY = 0;
    }
    if(rPadelY - padelY < bWall)
    {
        rPadelY = bWall + padelY;
        rPadelDY = 0;
    }
}
