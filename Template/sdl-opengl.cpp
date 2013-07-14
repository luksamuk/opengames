// Bibliotecas
#include <cstdlib>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Definições de display
#define WIDTH        640
#define HEIGHT       480
#define REFRESH_RATE 60.0

// Definições de input
//typedef SDLKey KeyboardKey
//typedef int MouseButton
#define KeyboardKey SDLKey
#define MouseButton int

bool running;
void init();
void handleEvents();
void handleKeyboard(KeyboardKey);
void handleMouse(int, int, MouseButton);
void draw();

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("SDL/OpenGL Test", NULL);
    SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
    Uint32 start;
    running = true;

    init();

    while(running)
    {
        start = SDL_GetTicks();
        handleEvents();
        draw();

        // Update de 60hz
            // Display SDL/OpenGL
        SDL_GL_SwapBuffers();
        if(1000.0/REFRESH_RATE > SDL_GetTicks() - start)
            SDL_Delay(1000.0/REFRESH_RATE - (SDL_GetTicks() - start));
    }
    SDL_Quit();
    return 0;
}

void init()
{
    // Muda o canto superior da tela para cima, esquerda
    glMatrixMode(GL_PROJECTION);
        glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 1);
}

void handleEvents()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            handleKeyboard(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouse(event.button.x, event.button.y, event.button.button);
            break;
        }
    }
}

void handleKeyboard(KeyboardKey key)
{
    switch(key)
    {
    case SDLK_ESCAPE:
        running = false;
        break;
    }
}

void handleMouse(int X, int Y, MouseButton button)
{
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
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
}
