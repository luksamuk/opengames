/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

// Bibliotecas
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Defini��es
#define WIN_WIDTH  640
#define WIN_HEIGHT 480
#define FRAMERATE 30.0

// Defini��es de tipos
typedef SDLKey KeyboardKey;
typedef Uint8  MouseButton;

// Vari�veis globais
bool running;
Uint32 gameloop_count;

// Prot�tipos de fun��es globais
void init();
void update();
void handleKeyboard(KeyboardKey, bool);
void handleMouse(int, int, MouseButton, bool);
void draw();

// Fun��o principal
int main(int argc, char** argv)
{
	// Inicia o SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	// Troca o nome na janela
	SDL_WM_SetCaption("CPlatformer", NULL);
	// Define largura e altura da tela com um buffer de 32bit
	// que usa OpenGL
	SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 32, SDL_OPENGL);

	// Inicialize o jogo
	init();

	while(running)
	{
		gameloop_count = SDL_GetTicks();
		// L�gica do jogo
		update();
		// Renderiza��o do jogo
		draw();
		SDL_GL_SwapBuffers();

		// For�a o FPS ao framerate atrav�s de delay.
		if(1000.0 / FRAMERATE > SDL_GetTicks() - gameloop_count)
			SDL_Delay(1000.0 / FRAMERATE - (SDL_GetTicks() - gameloop_count));
	}
	SDL_Quit();
	return 0;
}


// Fun��es globais principais
void init()
{
	running = true;
	// O canto superior da tela agora ficar�
	// em (0, 0).
	glMatrixMode(GL_PROJECTION);
		glOrtho(0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	// Defina a cor de tela vazia
	glClearColor(0, 0, 0, 1);
}

void update()
{
	SDL_Event event;

	// Redistribui os eventos de sa�da e presionamentos
	// de teclado e mouse, dentre outras coisas.
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			// Interrompa o jogo ao receber um sinal
			// de sa�da.
			running = false;
			break;
		// Repasse os sinais de teclado.
		case SDL_KEYDOWN:
			handleKeyboard(event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			handleKeyboard(event.key.keysym.sym, false);
			break;
		// Repasse os sinais de mouse.
		case SDL_MOUSEBUTTONDOWN:
			handleMouse(event.button.x, event.button.y, event.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			handleMouse(event.button.x, event.button.y, event.button.button, false);
			break;
		}
	}
}

void draw()
{
	// Limpe a tela para se livrar de artefatos
	glClear(GL_COLOR_BUFFER_BIT);
	// Resete a matrix de posi��o
	glLoadIdentity();
}

// Fun��es globais de input
void handleKeyboard(KeyboardKey key, bool isPressed)
{
	switch(key)
	{
	case SDLK_ESCAPE:
		// Se apertar ESC, saia.
		running = false;
		break;
	}
}

void handleMouse(int X, int Y, MouseButton button, bool isPressed)
{
	// Por enquanto nada. Apenas refer�ncia para
	// a��es futuras
	switch(button)
	{
	// Bot�o esquerdo do mouse
	case SDL_BUTTON_LEFT:
		break;
	// Bot�o direito do mouse
	case SDL_BUTTON_RIGHT:
		break;
	// Bot�o do meio do mouse
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

