#include <GL/glut.h>

void OnIdle();
void OnDisplay();
void OnKeyPress(unsigned char, int, int);
void OnMouseClick(int, int, int, int);
void OnMouseMove(int, int);

int main(int argc, char** argv)
{
	// Window Setup
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Exemplo 1");
	// Callback Setup
	glutIdleFunc(OnIdle);
	glutDisplayFunc(OnDisplay);
	glutKeyboardFunc(OnKeyPress);
	glutMouseFunc(OnMouseClick);
	glutPassiveMotionFunc(OnMouseMove);

	glutMainLoop();

	return 0;
}

// Idle function
void OnIdle() {}

// Display function
void OnDisplay()
{
	glutSwapBuffers();
}

// Keyboard key press function
void OnKeyPress(unsigned char key, int x, int y) {}

// Mouse button state function
void OnMouseClick(int button, int state, int x, int y) {}

// Mouse movement when no buttons are pressed
void OnMouseMove(int x, int y) {}
