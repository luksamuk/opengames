#include "stdafx.h"
#include "global.h"

// Global variables.
// references on global.h
Uint32  gameloop_c;
bool    GAMERUN;
palette MAINPALETTE;

int MOUSEPOS_X, MOUSEPOS_Y;

// Global functions prototype
void renderpalette(palette*);

void init();
void load();
void update();
void handleKeyboard(KeyboardKey, bool);
void handleMouseClick(int, int, MouseButton, bool);
void handleMouseMotion(int, int);
void renderBitmapString(float, float, char*);
void draw();
void quit();
void unload();

// Main function
int main(int argc, char** argv)
{
	//Initialize SDL
	glutInit(&argc, argv);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption(GAMENAME, NULL);
	SDL_WM_SetIcon(SDL_LoadBMP(GAMEICON), NULL);
	SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 8, SDL_OPENGL);

	// Initialize game and load resources
	init();
	load();

	// Main game loop
	while(GAMERUN)
	{
		// Snapshot of time before update & draw
		gameloop_c = SDL_GetTicks();

		// Game logic update
		update();
		// Game rendering, double-buffered
		draw();
		SDL_GL_SwapBuffers();

		// Force screen refresh rate
		if(1000.0 / REFRESHRATE > SDL_GetTicks() - gameloop_c)
			SDL_Delay(1000.0 / REFRESHRATE - (SDL_GetTicks() - gameloop_c));
	}

	unload();
	SDL_Quit();

	return 0;
}

// Function for initializing values
void init()
{
	GAMERUN = true;
	glMatrixMode(GL_PROJECTION);
		glOrtho(0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	
	glClearColor(MASKTOFLOAT(GETRCOLOR(COLOR_CORNFLOWERBLUE)),
		         MASKTOFLOAT(GETGCOLOR(COLOR_CORNFLOWERBLUE)),
		         MASKTOFLOAT(GETBCOLOR(COLOR_CORNFLOWERBLUE)),
		         1.0f);

	// Init your game logic here.
	MOUSEPOS_X = MOUSEPOS_Y = -1;
}

void load()
{
	// Global palette initialization
	gpalette_load(&MAINPALETTE);
	// Load all your resources here.
}

void update()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		// Redistribute input events
		switch(event.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			handleKeyboard(event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			handleKeyboard(event.key.keysym.sym, false);
			break;
		case SDL_MOUSEMOTION:
			handleMouseMotion(event.button.x, event.button.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handleMouseClick(event.button.x, event.button.y, event.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			handleMouseClick(event.button.x, event.button.y, event.button.button, false);
			break;
		// TODO: Handle more events here
		default:
			break;
		}
	}
}

void handleKeyboard(KeyboardKey key, bool isPressed)
{
	// TODO: Edit this accordingly
	switch(key)
	{
	case SDLK_ESCAPE:
		quit();
		break;
	case SDLK_UP:
		break;
	case SDLK_DOWN:
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	default:
		break;
	}
}

void handleMouseClick(int X, int Y, MouseButton button, bool isPressed)
{
	// TODO: Edit this accordingly
	switch(button)
	{
	case SDL_BUTTON_LEFT:
		break;
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

void handleMouseMotion(int X, int Y)
{
	MOUSEPOS_X = X; MOUSEPOS_Y = Y;
}

void renderBitmapString(float x, float y, char *string)
{
	char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        if((*c) == '\n')
        {
            y += 12;
            glRasterPos2f(x, y);
        }
        else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void draw()
{
	// Get rid of artifacts, reset draw pos
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Render your game here.

	renderpalette(&MAINPALETTE);
}

void renderpalette(palette* pal)
{
	int i;
	float colorpsize = (float)WIN_WIDTH / (float)pal->numcolors;
	char* output = malloc(sizeof(char) * 255);

	// Render lines of colors
	for(i = 0; i < pal->numcolors; i++)
	{
		float icoord = (float)i * colorpsize;
		color c = pal->data[i];
		glColorM(SHADOWCOLOR(c));
		glBegin(GL_QUADS);
				glVertex2f(icoord, 0.0f);
				glVertex2f(icoord + colorpsize, 0.0f);
				glVertex2f(icoord + colorpsize, colorpsize * 25);
				glVertex2f(icoord, colorpsize * 25);
		glEnd();

	}
	for(i = 0; i < pal->numcolors; i++)
	{
		float icoord = (float)i * colorpsize;
		color c = pal->data[i];
		glColorM(c);
		glBegin(GL_QUADS);
				glVertex2f(icoord, colorpsize * 25);
				glVertex2f(icoord + colorpsize, colorpsize * 25);
				glVertex2f(icoord + colorpsize, (colorpsize * 25) + colorpsize * 25);
				glVertex2f(icoord, (colorpsize * 25) + colorpsize * 25);
		glEnd();

	}
	for(i = 0; i < pal->numcolors; i++)
	{
		float icoord = (float)i * colorpsize;
		color c = pal->data[i];
		glColorM(HIGHLIGHTCOLOR(c));
		glBegin(GL_QUADS);
				glVertex2f(icoord, (colorpsize * 25) + colorpsize * 25);
				glVertex2f(icoord + colorpsize, (colorpsize * 25) + colorpsize * 25);
				glVertex2f(icoord + colorpsize, (colorpsize * 25) + (colorpsize * 25) * 2);
				glVertex2f(icoord, (colorpsize * 25) + (colorpsize * 25) * 2);
		glEnd();

	}

	// Render color info
	if(MOUSEPOS_Y <= colorpsize * 75 && MOUSEPOS_Y >= 0)
	{
		int i = MOUSEPOS_X / colorpsize;
		sprintf(output, "%03d -> 0x%04X", i, SHADOWCOLOR(MAINPALETTE.data[i]));

		glColorM(COLOR_WHITE);
		renderBitmapString(5.0f, (colorpsize * 75) + 10.0f, output);
	}
}

void quit()
{
	GAMERUN = false;
}

void unload()
{
	// Unload main palette
	palette_unload(&MAINPALETTE);
	// Unload all your resources here.
}