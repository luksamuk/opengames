/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * main.c
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

#include "stdafx.h"
#include "level.h"
#include "character.h"

// Variáveis globais
bool running;
Uint32 gameloop_count;
level lvl;
character chr;
int currentsection;

// Protótipos de funções globais
void init();
void update();
void handleKeyboard(KeyboardKey, bool);
void handleMouse(int, int, MouseButton, bool);
void draw();

// Função principal
int main(int argc, char** argv)
{
	// Inicia o SDL
	SDL_Init(SDL_INIT_VIDEO);
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
		// Lógica do jogo
		update();
		// Renderização do jogo
		draw();
		SDL_GL_SwapBuffers();

		// Força o FPS ao framerate através de delay.
		if(1000.0 / FRAMERATE > SDL_GetTicks() - gameloop_count)
			SDL_Delay(1000.0 / FRAMERATE - (SDL_GetTicks() - gameloop_count));
	}
	SDL_Quit();
	return 0;
}


// Funções globais principais
void init()
{
	running = true;
	// O canto superior da tela agora ficará
	// em (0, 0).
	glMatrixMode(GL_PROJECTION);
		glOrtho(0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	// Defina a cor de tela vazia
	glClearColor(0, 0, 0, 1);

	// Carregue a fase
	loadlevel("leveldef.lvl", &lvl);
	currentsection = lvl.init_section;
	// Carregue o personagem
	loadcharacter("brbr.chr", &chr);
	initcharacter(&chr, &lvl);
}

void update()
{
	// Atualize o personagem
	updatecharacter(&chr);

	SDL_Event event;

	// Redistribui os eventos de saída e presionamentos
	// de teclado e mouse, dentre outras coisas.
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			// Interrompa o jogo ao receber um sinal
			// de saída.
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
	// Resete a matrix de posição
	glLoadIdentity();

	// Desenhe a seção atual
	rendersection(&lvl, currentsection);
	// Desenhe o personagem
	rendercharf(&chr);
}

// Funções globais de input
void handleKeyboard(KeyboardKey key, bool isPressed)
{
	switch(key)
	{
	case SDLK_ESCAPE:
		// Se apertar ESC, saia.
		running = false;
		break;
	case SDLK_LEFT:
		// Mova o personagem para a esquerda
		if(isPressed)
			movchartodir(&chr, DIRECTION_LEFT);
		break;
	case SDLK_RIGHT:
		// Mova o personagem para a direita
		if(isPressed)
			movchartodir(&chr, DIRECTION_RIGHT);
		break;
	}
}

void handleMouse(int X, int Y, MouseButton button, bool isPressed)
{
	// Por enquanto nada. Apenas referência para
	// ações futuras
	switch(button)
	{
	// Botão esquerdo do mouse
	case SDL_BUTTON_LEFT:
		break;
	// Botão direito do mouse
	case SDL_BUTTON_RIGHT:
		break;
	// Botão do meio do mouse
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

