/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * character.c
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

 #include "character.h"

bool loadcharacter(char* filename, character* chr)
{
	FILE* file;
	file = fopen(filename, "r");
	if(!file) return false;

	int   intbuf, i, j;
	float floatbuf;

	// Número de cores na paleta
	fscanf(file, "%d", &intbuf);
	chr->p.n_colors = intbuf;
	chr->p.colors = malloc(sizeof(color) * chr->p.n_colors);
	// Importa a paleta
	for(i = 0; i < chr->p.n_colors; i++)
	{
		fscanf(file, "%f", &floatbuf);
		chr->p.colors[i].r = floatbuf;
		fscanf(file, "%f", &floatbuf);
		chr->p.colors[i].g = floatbuf;
		fscanf(file, "%f", &floatbuf);
		chr->p.colors[i].b = floatbuf;
	}
	// Largura e altura do personagem
	fscanf(file, "%d", &intbuf);
	chr->width = intbuf;
	fscanf(file, "%d", &intbuf);
	chr->height = intbuf;
	// Número de frames na animação
	fscanf(file, "%d", &intbuf);
	chr->n_frames = intbuf;
	chr->frames = malloc(sizeof(animframe) * chr->n_frames);
	// Hotspots de cada animação e
	// dados de cores da paleta
	int arraysize = chr->width * chr->height;
	for(i = 0; i < chr->n_frames; i++)
	{
		fscanf(file, "%d", &intbuf);
		chr->frames[i].hotspotX = intbuf;
		fscanf(file, "%d", &intbuf);
		chr->frames[i].hotspotY = intbuf;
		chr->frames[i].colorfrompallete = malloc(sizeof(int) * arraysize)
		for(j = 0; j < arraysize; j++)
		{
			fscanf(file, "%d", &intbuf);
			chr->frames[i].colorfrompallete[j] = intbuf - 1;
		}
	}
	fclose(file);
	return true;
}

void initcharacter(character* chr)
{
	// Inicializa direção
	chr->dir = DIRECTION_RIGHT;
	// Inicializa a posição inicial
	// TODO
	chr->xpos = chr->ypos = WIN_WIDTH / 2.0f;
}

void unloadcharacter(character* chr)
{
	int i;
	// Descarregar a paleta, os dados de cada
	// frame e os frames em si.
	free(chr->p.colors);
	for(i = 0; i < chr->n_frames; i++)
		free(chr->frames->colorfrompallete);
	free(chr->frames);
}

void rendercharf(character* chr)
{
	// Renderizar personagem na tela.
	// TODO
}
