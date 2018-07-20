#include <GL/glut.h>

typedef struct
{
    int x, y;
} position;

/* Globals */
position p1, p2;
int awaiting_press;

position window_size;

void reshape(int, int);

/* GL projection config and globals initialization */
void
init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(window_size.x, 0, window_size.y, 0);
    glMatrixMode(GL_MODELVIEW);

    p1.x = p1.y = awaiting_press = 0;
    p2.x = window_size.x;
    p2.y = window_size.y;
}

/* Callbacks */
void
reshape(int width, int height)
{
    window_size.x = width;
    window_size.y = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(width, 0, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}


void
mouse(int button, int state, int x, int y)
{
    if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
	x = window_size.x - x;
	if(!awaiting_press) {
	    p1.x = x; p1.y = y;
	    awaiting_press = 1;
	} else {
	    p2.x = x; p2.y = y;
	    awaiting_press = 0;
	}
	glutPostRedisplay();
    }
}


void
draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(!awaiting_press) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(p1.x, p1.y);
	glVertex2i(p2.x, p2.y);
	glEnd();
    }
    glutSwapBuffers();
}


/* ===== */
int
main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    window_size.x = 1280;
    window_size.y = 720;
    glutInitWindowSize(window_size.x, window_size.y);
    
    glutCreateWindow("Lines");
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    
    init();
    glutMainLoop();
}
