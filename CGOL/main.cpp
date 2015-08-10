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

constexpr byte n_squares() { return WIDTH / SQUARE_SIDE; }

bool** table;

void OnIdle(int);
void OnDisplay();
void OnReshape(int, int);
void OnKeyPress(unsigned char, int, int);
void OnMouseClick(int, int, int, int);
void OnMouseMove(int, int);

bool** createTable();
void   deleteTable(bool**);

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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Game of Life");
	glutFullScreen();
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutMouseFunc(OnMouseClick);
	glutPassiveMotionFunc(OnMouseMove);

	table = createTable();

	SQUARE_SIDE_N = (2.0f / n_squares());

	// Initialize?
	//table[0][0] = true;
	//table[1][0] = true;
	//table[0][1] = true;

	// Glider 1
	table[10][10] = true;
	table[11][10] = true;
	table[12][10] = true;
	table[10][11] = true;
	table[11][12] = true;

	// Glider 2
	table[0][1] = true;
	table[1][2] = true;
	table[2][0] = true;
	table[2][1] = true;
	table[2][2] = true;

	// Lightweight spaceship 1
	table[0][15] = true;
	table[3][15] = true;
	table[4][16] = true;
	table[0][17] = true;
	table[4][17] = true;
	table[1][18] = true;
	table[2][18] = true;
	table[3][18] = true;
	table[4][18] = true;

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	//OnIdle(0);
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
	glutPostRedisplay();
	if(!pause_required)
		glutTimerFunc(1000 / 10, OnIdle, 0);
}

void OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw board
	glColor3f(0.0f, 0.0f, 0.0f);
	for(byte i = 0; i < n_squares(); i++)
		for(byte j = 0; j < n_squares(); j++)
		{
			if(table[i][j])
			{
				// TODO: NORMALIZE SIZES
				float xpos = -1.0f + (SQUARE_SIDE_N),
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

	glutSwapBuffers();
}

void OnReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void OnKeyPress(unsigned char key, int x, int y)
{
	if(key == ']') {
		pause_required = false;
		OnIdle(0);
	}
	else if(key == '[')
		pause_required = true;
	else if(key == '.' && pause_required) OnIdle(0);
}

bool mousedown = false;
void OnMouseClick(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		glutPostRedisplay();
		if(state == GLUT_DOWN)
		{
			if(x <= WIDTH && y <= HEIGHT)
				table[x / SQUARE_SIDE][y / SQUARE_SIDE] = true;
			mousedown = true;
		}
		else
			mousedown = false;
	}
}

void OnMouseMove(int x, int y)
{
	if(mousedown)
		table[x / SQUARE_SIDE][y / SQUARE_SIDE] = true;
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