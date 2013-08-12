#include "stdafx.h"
#include "global.h"

// Global variables.
// references on global.h
Uint32  gameloop_c;
bool    GAMERUN;
palette MAINPALETTE;
int currentmode;

// Global functions prototype
void init();
void load();
void update();
void handleKeyboard(KeyboardKey, bool);
void handleMouse(int, int, MouseButton, bool);
void draw();
void quit();
void unload();

// Main function
int main(int argc, char** argv)
{
	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption(GAMENAME, NULL);
	SDL_WM_SetIcon(SDL_LoadBMP(GAMEICON), NULL);
	SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 16, SDL_OPENGL);

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
	currentmode = 1;
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
		case SDL_MOUSEBUTTONDOWN:
			handleMouse(event.button.x, event.button.y, event.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			handleMouse(event.button.x, event.button.y, event.button.button, false);
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
		if(isPressed && currentmode > 0)
			currentmode--;
		else if(isPressed && currentmode <= 0)
			currentmode = 2;
		break;
	case SDLK_RIGHT:
		if(isPressed && currentmode < 2)
			currentmode++;
		else if(isPressed && currentmode >= 2)
			currentmode = 0;
		break;
	default:
		break;
	}
}

void handleMouse(int X, int Y, MouseButton button, bool isPressed)
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

void draw()
{
	// Get rid of artifacts, reset draw pos
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Render your game here.
	int i, j;
	float icoord, jcoord;
	for(i = 0; i < (int)(WIN_WIDTH / 10.0f); i++)
	{
		icoord = i * 10.0f;
		for(j = 0; j < (int)(WIN_HEIGHT / 10.0f); j++)
		{
			jcoord = j * 10.0f;
			color c = MAINPALETTE.data[(i * 10) + j];

			if(currentmode == 0)      c = SHADOWCOLOR(c);
			else if(currentmode == 2) c = HIGHLIGHTCOLOR(c); 
			
			glColor3b(GETRCOLOR(c), GETGCOLOR(c), GETBCOLOR(c));
			glBegin(GL_QUADS);
				glVertex2f(0.0f + icoord, 0.0f + jcoord);
				glVertex2f(10.0f + icoord, 0.0f + jcoord);
				glVertex2f(10.0f + icoord, 10.0f + jcoord);
				glVertex2f(0.0f + icoord, 10.0f + jcoord);
			glEnd();
		}
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