/*
 * cbreakout - Breakout OpenGL clone
 * by Lucas Samuel Vieira
 * Itabira, 2013
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SPEED                0.00025f
#define PAD_DIST_FROMBOTTOM  0.05f
#define PAD_SIZE_X           0.25f
#define PAD_SIZE_Y           0.025f
#define BALL_SIZE            0.025f
typedef unsigned char byte;


// Global elements
byte Lives = 3;

// Pad  elements
GLfloat Xpad  = 0.0f;
bool movLeft  = false;
bool movRight = false;

// Pad actions
void setpad(GLfloat x);

// Ball elements
GLfloat ball_Xpos = 0.0f,
        ball_Ypos = -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y,
        ball_Xspeed = SPEED/2,
        ball_Yspeed = SPEED/2;

// Ball actions
void setball(GLfloat x, GLfloat y);

/* GLUT callback Handlers */

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    setpad(Xpad);
    setball(ball_Xpos, ball_Ypos);

    glutSwapBuffers();
}


static void keyDown(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
        movLeft = true;
        break;
    case 'd':
        movRight = true;
        break;
    }

    glutPostRedisplay();
}

static void keyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
        movLeft = false;
        break;
    case 'd':
        movRight = false;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    // Pad movement
    if(movLeft && Xpad  > -1.0f + PAD_SIZE_X/2) Xpad -= SPEED;
    if(movRight && Xpad <  1.0f - PAD_SIZE_X/2) Xpad += SPEED;

    // Ball collisions
    // X collisions
    if(ball_Xpos > 1.0f - BALL_SIZE/2) // Right boundary
    {
        ball_Xpos = 1.0f - BALL_SIZE/2;
        ball_Xspeed = -SPEED/2;
    }
    else if(ball_Xpos < -1.0f + BALL_SIZE/2) // Left boundary
    {
        ball_Xpos = -1.0f + BALL_SIZE/2;
        ball_Xspeed = SPEED/2;
    }

    // Y Collisions
    if(ball_Ypos > 1.0f - BALL_SIZE) // Up boundary
    {
        ball_Ypos  = 1.0f - BALL_SIZE;
        ball_Yspeed = -SPEED/2;
    }
    // Pad collision
    else if(ball_Ypos < -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y // if ball is within
         && ball_Ypos > -1.0f + PAD_DIST_FROMBOTTOM  // Y pad range and
         && ball_Xpos - BALL_SIZE/2 >= Xpad - PAD_SIZE_X/2  // X pad
         && ball_Xpos + BALL_SIZE/2 <= Xpad + PAD_SIZE_X/2) // range
    {
        ball_Ypos = -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y;
        ball_Yspeed = (SPEED/2);
    }

    // Ball respawn
    if(ball_Ypos < -1.0 - BALL_SIZE)
    {
        if(Lives == 0) exit(EXIT_SUCCESS);

        ball_Xpos = 0.0f;
        ball_Ypos = -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y;
        ball_Xspeed = SPEED/2;
        ball_Yspeed = SPEED/2;
        Lives--;
    }

    // Ball movement
    ball_Xpos += ball_Xspeed;
    ball_Ypos += ball_Yspeed;
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("cbreakout");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void setpad(GLfloat x)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - PAD_SIZE_X/2, -1.0f + PAD_DIST_FROMBOTTOM);
        glVertex2f(x - PAD_SIZE_X/2, -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y);
        glVertex2f(x + PAD_SIZE_X/2, -1.0f + PAD_DIST_FROMBOTTOM + PAD_SIZE_Y);
        glVertex2f(x + PAD_SIZE_X/2, -1.0f + PAD_DIST_FROMBOTTOM);
    glEnd();
}

void setball(GLfloat x, GLfloat y)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - BALL_SIZE/2, y - BALL_SIZE);
        glVertex2f(x - BALL_SIZE/2, y);
        glVertex2f(x + BALL_SIZE/2, y);
        glVertex2f(x + BALL_SIZE/2, y - BALL_SIZE);
    glEnd();
}

