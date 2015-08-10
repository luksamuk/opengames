// Game of Life example
// 


#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C"
{
	#include <GL/gl.h>
	#include <GL/glut.h>
	#include <GL/glu.h>
	#include <unistd.h>
}

typedef unsigned char byte;

#define WIDTH      800u
#define HEIGHT     800u
#define SQUARE_SIDE 10u

float SQUARE_SIDE_N;
bool pause_required = true;
int fps = 5;
int TRUEWIDTH, TRUEHEIGHT;

constexpr byte n_squares() { return WIDTH / SQUARE_SIDE; }

bool** table;

void OnIdle(int);
void OnDisplay();
void OnReshape(int, int);
void OnKeyPress(unsigned char, int, int);
void OnMouseClick(int, int, int, int);
void OnMouseMove(int, int);

long long int lifespan = 0l;
char lifespan_string[30];

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


bool** createTable();
void   deleteTable(bool**);


// Tools for creating specific figures

// Static figures
void createBlock(int x, int y, int angle);
void createBeehive(int, int, int);
void createLoaf(int, int, int);
void createBoat(int, int, int);

// Oscillators
void createBlinker(int, int, int);
void createToad(int, int, int);
void createBeacon(int, int, int);
void createPulsar(int, int, int);
void createPentadecathlon(int, int, int);

// Spaceships
void createGlider(int, int, int);
void createLWSS(int, int, int);

// Misc
void createGosperGliderGun(int, int, int);
void createRPentomino(int, int, int);

char instructions[] = "Press ] to start iterations, [ to stop\n"
                      "Press > to advance frame by frame\n"
                      "Pushing ] more than once will make the game go faster";

struct neighbor
{
	int i, j;

	void set(int i, int j) {
		this->i = i;
		this->j = j;
	}

	bool valid() {
		return (this->i >= 0 && this->j >= 0) && (this->i < n_squares() && this->j < n_squares());
	}
};

int main(int argc, char** argv)
{
	TRUEWIDTH = WIDTH;
	TRUEHEIGHT = HEIGHT;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Conway's Game of Life");
	glutFullScreen();
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(OnMouseMove);

	table = createTable();

	SQUARE_SIDE_N = (2.0f / n_squares());

	// Glider
	//createGlider(0, 0, 0);

	// Lightweight spaceship 1
	//createLWSS(0, 0, 0);

	//createGosperGliderGun(0, 0, 0);

	//createPentadecathlon(0, 30, 0);
	//createBlinker(0, 50, 0);
	//createToad(0, 55, 0);
	//createBeacon(0, 62, 0);
	//createPulsar(20, 30, 0);
	createRPentomino(n_squares() / 2, n_squares() / 2, 0);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	glutMainLoop();

	deleteTable(table);

	return 0;
}

void OnIdle(int val)
{
	neighbor up, down, left, right,
		     ul, ur,   dl,   dr;
	bool** newTable = createTable();

	for(int i = 0; i < n_squares(); i++)
		for(int j = 0; j < n_squares(); j++)
		{
			// Compare number of neighbors.
			byte n_neighbors = 0;

			// Create neighbors nearby
			up.set   (i,     j - 1);
			down.set (i,     j + 1);
			left.set (i - 1,     j);
			right.set(i + 1,     j);
			ul.set   (i - 1, j - 1);
			ur.set   (i + 1, j - 1);
			dl.set   (i - 1, j + 1);
			dr.set   (i + 1, j + 1);

			// Compare for number of neighbors
			if(up.valid())
				if(table[up.i][up.j]) n_neighbors++;
			if(down.valid())
				if(table[down.i][down.j]) n_neighbors++;
			if(left.valid())
				if(table[left.i][left.j]) n_neighbors++;
			if(right.valid())
				if(table[right.i][right.j]) n_neighbors++;
			if(ul.valid())
				if(table[ul.i][ul.j]) n_neighbors++;
			if(ur.valid())
				if(table[ur.i][ur.j]) n_neighbors++;
			if(dl.valid())
				if(table[dl.i][dl.j]) n_neighbors++;
			if(dr.valid())
				if(table[dr.i][dr.j]) n_neighbors++;


			if(table[i][j])
			{
				//printf("Found a living cell with %d neighbors\n", n_neighbors);
				// More than 3 neighbors: death by superpopulation
				if(n_neighbors > 3 || n_neighbors < 2) newTable[i][j] = false;
				// Less than 2 neighbors: death by loneliness
				//else if(n_neighbors < 2) newTable[i][j] = false;
				else newTable[i][j] = true;
			}
			else
			{
				// Dead and with exactly 3 neighbors: live once again
				if(n_neighbors == 3) {
					newTable[i][j] = true;
				}
			}
		}
	deleteTable(table);
	table = newTable;
	lifespan++;
	glutPostRedisplay();
	if(!pause_required)
		glutTimerFunc(1000 / fps, OnIdle, 0);
}

void OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	// Draw grid
	for(byte i = 0; i < n_squares(); i++)
	{
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex2f(-1.0f + (i * SQUARE_SIDE_N), 1.0f);
		glVertex2f(-1.0f + (i * SQUARE_SIDE_N), -1.0f);
		glEnd();
		glPopMatrix();
	}
	for(byte j = 0; j < n_squares(); j++)
	{
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex2f(-1.0f, 1.0f - (j * SQUARE_SIDE_N));
		glVertex2f(1.0f, 1.0f - (j * SQUARE_SIDE_N));
		glEnd();
		glPopMatrix();
	}

	// Draw cells
	for(byte i = 0; i < n_squares(); i++)
		for(byte j = 0; j < n_squares(); j++)
		{
			if(table[i][j])
			{
				// TODO: NORMALIZE SIZES
				float xpos = -1.0f,
				      ypos =  1.0f - (SQUARE_SIDE_N);
				xpos += (SQUARE_SIDE_N * i);
				ypos -= (SQUARE_SIDE_N * j);
				glPushMatrix();
					glTranslatef(xpos, ypos, 0.0f);
					glBegin(GL_QUADS);
					glVertex2f(0.0f, 0.0f);
					glVertex2f(SQUARE_SIDE_N, 0.0f);
					glVertex2f(SQUARE_SIDE_N, SQUARE_SIDE_N);
					glVertex2f(0.0f, SQUARE_SIDE_N);
					glEnd();
				glPopMatrix();
			}
		}

	if(pause_required) {
		glColor3f(0.7f, 0.0f, 0.0f);
		renderBitmapString(-1.0f, 0.9f, instructions);
	}
	sprintf(lifespan_string, "Age: %d\nFPS: %d", lifespan, fps);
	glColor3f(0.0f, 0.0f, 1.0f);
	renderBitmapString(-1.0f, -0.9f, lifespan_string);

	glutSwapBuffers();
}

void OnReshape(int w, int h)
{
	TRUEWIDTH = w;
	TRUEHEIGHT = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void OnKeyPress(unsigned char key, int x, int y)
{
	if(key == ']') {
		if(pause_required) {
			pause_required = false;
			OnIdle(0);
		}
		else
		{
			fps += 5;
		}
	}
	else if(key == '[')
	{
		pause_required = true;
		fps = 5;
	}
	else if(key == '>' && pause_required) OnIdle(0);
	else if(key == 'c')
	{
		deleteTable(table);
		table = createTable();
		glutPostRedisplay();
	}
}

bool mousedown = false;
void OnMouseClick(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		float xpos_perc, ypos_perc;
		xpos_perc = float(x) / float(TRUEWIDTH);
		ypos_perc = float(y) / float(TRUEHEIGHT);

		if(state == GLUT_DOWN)
		{
			if((xpos_perc > 0.0f && xpos_perc < 1.0f) && (ypos_perc > 0.0f && ypos_perc < 1.0f))
				table[int(xpos_perc * n_squares())][int(ypos_perc * n_squares())] = true;
			mousedown = true;
		}
		else
			mousedown = false;

		glutPostRedisplay();
	}
}

void OnMouseMove(int x, int y)
{
	if(mousedown)
	{
		float xpos_perc, ypos_perc;
		xpos_perc = float(x) / float(TRUEWIDTH);
		ypos_perc = float(y) / float(TRUEHEIGHT);

		if((xpos_perc > 0.0f && xpos_perc < 1.0f) && (ypos_perc > 0.0f && ypos_perc < 1.0f))
			table[int(xpos_perc * n_squares())][int(ypos_perc * n_squares())] = true;
		glutPostRedisplay();
	}
}


bool** createTable()
{
	bool** newTable;
	newTable = new bool*[n_squares()];
	for(byte b = 0; b < n_squares(); b++)
	{
		newTable[b] = new bool[n_squares()];
		for(byte i = 0; i < n_squares(); i++)
			newTable[b][i] = false;
	}
	return newTable;
}

void   deleteTable(bool** oldtable)
{
	for(byte b = 0; b < n_squares(); b++)
		delete [] oldtable[b];
	delete [] oldtable;
}






//==========================
// I'm disconsidering angle for now until I find a
// proper way to rotate the actual figure around its axis.

void createBlock(int x, int y, int angle)
{
	table    [x][y]     = true;
	table[x + 1][y]     = true;
	table    [x][y + 1] = true;
	table[x + 1][y + 1] = true;
}

void createBeehive(int x, int y, int angle)
{
	table[x + 1][y]     = true;
	table[x + 2][y]     = true;
	table    [x][y + 1] = true;
	table[x + 3][y + 1] = true;
	table[x + 1][y + 2] = true;
	table[x + 2][y + 2] = true;
}

void createLoaf(int x, int y, int angle)
{
	table[x + 1][y]     = true;
	table[x + 2][y]     = true;
	table    [x][y + 1] = true;
	table[x + 3][y + 1] = true;
	table[x + 1][y + 2] = true;
	table[x + 3][y + 2] = true;
	table[x + 2][y + 3] = true;
}

void createBoat(int x, int y, int angle)
{
	table    [x][y]     = true;
	table[x + 1][y]     = true;
	table    [x][y + 1] = true;
	table[x + 2][y + 1] = true;
	table[x + 1][y + 2] = true;
}

void createBlinker(int x, int y, int angle)
{
	table[x + 1][y + 2] = true;
	table[x + 2][y + 2] = true;
	table[x + 3][y + 2] = true;
}

void createToad(int x, int y, int angle)
{
	table[x + 2][y + 2] = true;
	table[x + 3][y + 2] = true;
	table[x + 4][y + 2] = true;

	table[x + 1][y + 3] = true;
	table[x + 2][y + 3] = true;
	table[x + 3][y + 3] = true;
}

void createBeacon(int x, int y, int angle)
{
	createBlock(x + 1, y + 1, angle);
	createBlock(x + 3, y + 3, angle);
}

void createPulsar(int x, int y, int angle)
{
	table[x + 4][y + 2] = true;
	table[x + 5][y + 2] = true;
	table[x + 6][y + 2] = true;

	table[x + 10][y + 2] = true;
	table[x + 11][y + 2] = true;
	table[x + 12][y + 2] = true;



	table[x + 2][y + 4] = true;
	table[x + 7][y + 4] = true;
	table[x + 9][y + 4] = true;
	table[x + 14][y + 4] = true;

	table[x + 2][y + 5] = true;
	table[x + 7][y + 5] = true;
	table[x + 9][y + 5] = true;
	table[x + 14][y + 5] = true;

	table[x + 2][y + 6] = true;
	table[x + 7][y + 6] = true;
	table[x + 9][y + 6] = true;
	table[x + 14][y + 6] = true;




	table[x + 4][y + 7] = true;
	table[x + 5][y + 7] = true;
	table[x + 6][y + 7] = true;

	table[x + 10][y + 7] = true;
	table[x + 11][y + 7] = true;
	table[x + 12][y + 7] = true;




	table[x + 4][y + 9] = true;
	table[x + 5][y + 9] = true;
	table[x + 6][y + 9] = true;

	table[x + 10][y + 9] = true;
	table[x + 11][y + 9] = true;
	table[x + 12][y + 9] = true;



	table[x + 2][y + 10] = true;
	table[x + 7][y + 10] = true;
	table[x + 9][y + 10] = true;
	table[x + 14][y + 10] = true;

	table[x + 2][y + 11] = true;
	table[x + 7][y + 11] = true;
	table[x + 9][y + 11] = true;
	table[x + 14][y + 11] = true;

	table[x + 2][y + 12] = true;
	table[x + 7][y + 12] = true;
	table[x + 9][y + 12] = true;
	table[x + 14][y + 12] = true;




	table[x + 4][y + 14] = true;
	table[x + 5][y + 14] = true;
	table[x + 6][y + 14] = true;

	table[x + 10][y + 14] = true;
	table[x + 11][y + 14] = true;
	table[x + 12][y + 14] = true;
}

void createPentadecathlon(int x, int y, int angle)
{

	                      table[x + 5][y + 1] = true;
	                      table[x + 5][y + 2] = true;

	                      table[x + 5][y + 4] = true;
	table[x + 4][y + 5] = true;                   table[x + 6][y + 5] = true;

    table[x + 4][y + 7] = true; table[x + 5][y + 7] = true; table[x + 6][y + 7] = true;


    table[x + 4][y + 10] = true; table[x + 5][y + 10] = true; table[x + 6][y + 10] = true;

    table[x + 4][y + 12] = true;                   table[x + 6][y + 12] = true;
                          table[x + 5][y + 13] = true;

    	                  table[x + 5][y + 15] = true;
	                      table[x + 5][y + 16] = true;
}


void createGlider(int x, int y, int angle)
{
	table    [x][y + 1] = true;
	table[x + 1][y + 2] = true;
	table[x + 2][y]     = true;
	table[x + 2][y + 1] = true;
	table[x + 2][y + 2] = true;
}

void createLWSS(int x, int y, int angle)
{
	table    [x][y + 15] = true;
	table[x + 3][y + 15] = true;
	table[x + 4][y + 16] = true;
	table    [x][y + 17] = true;
	table[x + 4][y + 17] = true;
	table[x + 1][y + 18] = true;
	table[x + 2][y + 18] = true;
	table[x + 3][y + 18] = true;
	table[x + 4][y + 18] = true;
}

void createGosperGliderGun(int x, int y, int angle)
{
	createBlock(x + 1, y + 5, angle);

	table[x + 11][y + 5] = true;
	table[x + 11][y + 6] = true;
	table[x + 11][y + 7] = true;

	table[x + 12][y + 4] = true;
	table[x + 12][y + 8] = true;

	table[x + 13][y + 3] = true;
	table[x + 13][y + 9] = true;

	table[x + 14][y + 3] = true;
	table[x + 14][y + 9] = true;

	table[x + 15][y + 6] = true;

	table[x + 16][y + 4] = true;
	table[x + 16][y + 8] = true;

	table[x + 17][y + 5] = true;
	table[x + 17][y + 6] = true;
	table[x + 17][y + 7] = true;

	table[x + 18][y + 6] = true;




	table[x + 21][y + 3] = true;
	table[x + 21][y + 4] = true;
	table[x + 21][y + 5] = true;

	table[x + 22][y + 3] = true;
	table[x + 22][y + 4] = true;
	table[x + 22][y + 5] = true;

	table[x + 23][y + 2] = true;
	table[x + 23][y + 6] = true;


	table[x + 25][y + 1] = true;
	table[x + 25][y + 2] = true;
	table[x + 25][y + 6] = true;
	table[x + 25][y + 7] = true;

	createBlock(x + 35, y + 3, angle);
}

void createRPentomino(int x, int y, int angle)
{
	table[x + 2][y + 1] = true;
	table[x + 3][y + 1] = true;
	table[x + 1][y + 2] = true;
	table[x + 2][y + 2] = true;
	table[x + 2][y + 3] = true;
}