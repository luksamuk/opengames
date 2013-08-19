#include "global.h"
#include "level.h"

// Global variables.
// references on global.h
Uint32  gameloop_c;
bool    GAMERUN;
SDL_Surface* surface;
palette MAINPALETTE;
inputstate INPUT_STATE;
inputstate INPUTSTATE_OLD;

bool displaypalette = false;

// Global functions prototype
void renderpalette(palette*);

void init();
void load();
void unload();
void update();
void draw();
void quit();

void handleEvents();
void handleKeyboard(KeyboardKey, bool);
void handleMouseClick(int, int, MouseButton, bool);
void handleMouseMotion(int, int);
void renderBitmapString(float, float, char*);

// Main function
int main(int argc, char** argv)
{
	//Initialize SDL
	glutInit(&argc, argv);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption(GAMENAME, NULL);
	SDL_WM_SetIcon(SDL_LoadBMP(GAMEICON), NULL);
	surface = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 8, SDL_OPENGL);

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
	// Set game to run
	GAMERUN = true;
	// Set orthographic view
	glMatrixMode(GL_PROJECTION);
		glOrtho(0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	// Set clear color
	glClearColorM(COLOR_CORNFLOWERBLUE);
	// Init input structures
	input_initstate(&INPUT_STATE);
	input_initstate(&INPUTSTATE_OLD);

	// TODO: Init your game logic here.
}

void load()
{
	// Global palette initialization
	gpalette_load(&MAINPALETTE);

	// TODO: Load all your resources here.
}

void unload()
{
	// Unload main palette
	palette_unload(&MAINPALETTE);
	// Unload all your resources here.
}

void update()
{
	// Redistribute input events
	input_copystate(&INPUTSTATE_OLD, INPUT_STATE);
	// Handle SDL events
	handleEvents();

	// TODO: Add your logic here.

	/* TESTS! */

	// Palette test display
	if(input_haspressed(&INPUT_STATE, &INPUTSTATE_OLD, BUTTON_A))
		displaypalette = !displaypalette;
}

void draw()
{
	// Get rid of artifacts, reset draw pos
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// TODO: Render your game here.

	/* TESTS! */
	// Render main palette, if active.
	if(displaypalette)
		renderpalette(&MAINPALETTE);

	// Render mousepos
	char str[255];
	sprintf(str, "0x%08X\n0x%08X\n",
		INPUT_STATE.mousepos.x,,
		INPUT_STATE.mousepos.y);
	glColorM(COLOR_WHITE);
	renderBitmapString(5.0f, WIN_HEIGHT - 28, str);

}

void quit()
{
	GAMERUN = false;
}

void handleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
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
		INPUT_STATE.hat.y = (isPressed ? -1 : 0);
		break;
	case SDLK_DOWN:
		INPUT_STATE.hat.y = (isPressed ? 1 : 0);
		break;
	case SDLK_LEFT:
		INPUT_STATE.hat.x = (isPressed ? -1 : 0);
		break;
	case SDLK_RIGHT:
		INPUT_STATE.hat.x = (isPressed ? 1 : 0);
		break;
	case SDLK_z:
		INPUT_STATE.a = (isPressed ? 1 : 0);
		break;
	case SDLK_x:
		INPUT_STATE.b = (isPressed ? 1 : 0);
		break;
	case SDLK_c:
		INPUT_STATE.c = (isPressed ? 1 : 0);
		break;
	default:
		break;
	}
}
void handleMouseClick(int X, int Y, MouseButton button, bool isPressed)
{
	INPUT_STATE.mousepos.x = X;
	INPUT_STATE.mousepos.y = Y;
	switch(button)
	{
	case SDL_BUTTON_LEFT:
		INPUT_STATE.mouse_left = isPressed;
		break;
	case SDL_BUTTON_RIGHT:
		INPUT_STATE.mouse_right = isPressed;
		break;
	case SDL_BUTTON_MIDDLE:
		INPUT_STATE.mouse_middle = isPressed;
		break;
	}
}
void handleMouseMotion(int X, int Y)
{
	// Gets and sets mouse motion	
	INPUT_STATE.mousepos.x = X;
	INPUT_STATE.mousepos.y = Y;
}

void renderBitmapString(float x, float y, char *string)
{
	char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        if((*c) == '\n')
        {
            y += 18;
            glRasterPos2f(x, y);
        }
        else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
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
		color c = getcolor(pal, i);
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
		color c = getcolor(pal, i);
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
		color c = getcolor(pal, i);
		glColorM(HIGHLIGHTCOLOR(c));
		glBegin(GL_QUADS);
				glVertex2f(icoord, (colorpsize * 25) + colorpsize * 25);
				glVertex2f(icoord + colorpsize, (colorpsize * 25) + colorpsize * 25);
				glVertex2f(icoord + colorpsize, (colorpsize * 25) + (colorpsize * 25) * 2);
				glVertex2f(icoord, (colorpsize * 25) + (colorpsize * 25) * 2);
		glEnd();

	}

	// Render color info
	if(INPUT_STATE.mousepos.y <= colorpsize * 75)
	{
		int i = INPUT_STATE.mousepos.x / colorpsize;
		sprintf(output, "%03u -> 0x%04X", i, MAINPALETTE.data[i]);

		glColorM(COLOR_BLACK);
		renderBitmapString(5.0f, (colorpsize * 75) + 10.0f, output);
	}
}