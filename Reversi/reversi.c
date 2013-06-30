#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>

typedef unsigned char           byte;
#define TABULEIRO_LINE_DISTANCE 0.25f
#define POINT_SIZE              30.0f
#define CASA_VAZIA              (byte)0
#define CASA_PRETA              (byte)1
#define CASA_BRANCA             (byte)2

int vwprt_x, vwprt_y;

void OnIdle();
void OnDisplay();
void OnReshape(int, int);
void OnKeyPress(unsigned char, int, int);
void OnMouseClick(int, int, int, int);
void OnMouseMove(int, int);
void renderBitmapString(float, float, char*);
char msg[50];

// Objetos de jogo
byte tabuleiro[8][8] = {0};
bool turn;
void drawTabuleiro();

// Objetos de controle
bool   MouseLButton, oldMouseLButton;
float  mousepressX, mousepressY;

int main(int argc, char** argv)
{
	// Window Setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	vwprt_x = 640; vwprt_y = 480;
	glutInitWindowSize(vwprt_x, vwprt_y);
	glutCreateWindow("Reversi");
	// Callback Setup
	glutIdleFunc(OnIdle);
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutMouseFunc(OnMouseClick);
	glutPassiveMotionFunc(OnMouseMove);

	// Inicializa o tabuleiro
	tabuleiro[3][3] = tabuleiro[4][4] = CASA_BRANCA;
	tabuleiro[3][4] = tabuleiro[4][3] = CASA_PRETA;
	turn = false;
	sprintf(msg, "Vez das PECAS PRETAS");

	// Inicializa o input
	MouseLButton = oldMouseLButton = false;

	glutMainLoop();

	return 0;
}

void renderBitmapString(float x, float y, char *string)
{
    glRasterPos2f(x, y);
	char* c;
    for (c = string; *c != '\0'; c++)
    {
        if((*c) == '\n')
        {
            y -= 0.045;
            glRasterPos2f(x, y);
        }
        else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}
// Reshape function
void OnReshape(int width, int height)
{
	vwprt_x = width;
	vwprt_y = height;
}


// Idle function
void OnIdle()
{
	int i = (mousepressX / TABULEIRO_LINE_DISTANCE);
	int j = (mousepressY / TABULEIRO_LINE_DISTANCE);

	sprintf(msg, "none");
	// Compara se o mouse foi pressionado em uma das casas
	if(!oldMouseLButton && MouseLButton)
	{
		if(tabuleiro[i][j] == CASA_VAZIA)
		{
			tabuleiro[i][j] = (turn ? CASA_BRANCA : CASA_PRETA);
			turn = !turn;
		}
	}

	sprintf(msg, (turn ? "Vez das PECAS BRANCAS" : "Vez das PECAS PRETAS"));
	glutPostRedisplay();

}

void drawTabuleiro()
{
	int i, j;

	glColor3f(1.0f, 1.0f, 1.0f);
	// O tabuleiro é composto de 8 linhas e 8 colunas.
	// A distância de uma linha a outra eh de 2/8, 0.25f
	// ou TABULEIRO_LINE_DISTANCE.

	// Linhas:
	glBegin(GL_LINES);
		glVertex2f((-3.0f * TABULEIRO_LINE_DISTANCE), 1.0f);
		glVertex2f((-3.0f * TABULEIRO_LINE_DISTANCE), -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f((-2.0f * TABULEIRO_LINE_DISTANCE), 1.0f);
		glVertex2f((-2.0f * TABULEIRO_LINE_DISTANCE), -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f((-1.0f * TABULEIRO_LINE_DISTANCE), 1.0f);
		glVertex2f((-1.0f * TABULEIRO_LINE_DISTANCE), -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(0.0f, 1.0f);
		glVertex2f(0.0f, -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(TABULEIRO_LINE_DISTANCE, 1.0f);
		glVertex2f(TABULEIRO_LINE_DISTANCE, -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f((2.0f * TABULEIRO_LINE_DISTANCE), 1.0f);
		glVertex2f((2.0f * TABULEIRO_LINE_DISTANCE), -1.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f((3.0f * TABULEIRO_LINE_DISTANCE), 1.0f);
		glVertex2f((3.0f * TABULEIRO_LINE_DISTANCE), -1.0f);

	// Colunas:
	glBegin(GL_LINES);
		glVertex2f(1.0f, (-3.0f * TABULEIRO_LINE_DISTANCE));
		glVertex2f(-1.0f, (-3.0f * TABULEIRO_LINE_DISTANCE));
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, (-2.0f * TABULEIRO_LINE_DISTANCE));
		glVertex2f(-1.0f, (-2.0f * TABULEIRO_LINE_DISTANCE));
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, (-1.0f * TABULEIRO_LINE_DISTANCE));
		glVertex2f(-1.0f, (-1.0f * TABULEIRO_LINE_DISTANCE));
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(-1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, TABULEIRO_LINE_DISTANCE);
		glVertex2f(-1.0f, TABULEIRO_LINE_DISTANCE);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, (2.0f * TABULEIRO_LINE_DISTANCE));
		glVertex2f(-1.0f, (2.0f * TABULEIRO_LINE_DISTANCE));
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(1.0f, (3.0f * TABULEIRO_LINE_DISTANCE));
		glVertex2f(-1.0f, (3.0f * TABULEIRO_LINE_DISTANCE));
	glEnd();

	// Pecas:
	glPointSize(POINT_SIZE);
	glBegin(GL_POINTS);
		for(i = 0; i < 8; i++)
			for(j = 0; j < 8; j++)
				if(tabuleiro[i][j] != CASA_VAZIA)
				{
					switch(tabuleiro[i][j])
					{
						case CASA_PRETA:
							glColor3f(0.6f, 0.0f, 0.0f);
							break;
						case CASA_BRANCA:
							glColor3f(1.0f, 1.0f, 1.0f);
							break;
					}
					
					glVertex2f(-1.0f + (TABULEIRO_LINE_DISTANCE * i) + (TABULEIRO_LINE_DISTANCE/2),
							   1.0f - (TABULEIRO_LINE_DISTANCE * j) - (TABULEIRO_LINE_DISTANCE/2));

				}
	glEnd();
}

// Display function
void OnDisplay()
{
	// Clean canvas
	glClear(GL_COLOR_BUFFER_BIT);

	// Imprime o tabuleiro
	drawTabuleiro();
	glColor3f(1.0f, 1.0f, 1.0f);
	renderBitmapString(-1.0f, 0.9f, msg);

	// Draw
	glutSwapBuffers();
}

// Keyboard key press function
void OnKeyPress(unsigned char key, int x, int y)
{
	
}

// Mouse button state function
void OnMouseClick(int button, int state, int x, int y)
{
	oldMouseLButton = MouseLButton;
	if(button == GLUT_LEFT_BUTTON)
	{
		switch(state)
		{
			case GLUT_UP:
				MouseLButton = false;
				break;
			case GLUT_DOWN:
				MouseLButton = true;
				break;
		}
	}
}

void OnMouseMove(int x, int y)
{
	mousepressX = (((float)x / (float)vwprt_x) * 2.0f);
	mousepressY = (((float)y / (float)vwprt_y) * 2.0f);
}
