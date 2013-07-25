#include "level.h"

bool loadlevel(char* filename, level* lvl)
{
	FILE *file;
	file = fopen(filename, "r");
	if(!file) return false;
	
	int intbuf, i, j;
	float floatbuf;
	
	// Número de cores na paleta
	fscanf(file, "%d", &intbuf);
	lvl->p.n_colors = intbuf;
	lvl->p.colors = malloc(sizeof(color) * lvl->p.n_colors);
	// Carregar as cores
	for(i = 0; i < lvl->p.n_colors; i++)
	{
		fscanf(file, "%f", &floatbuf);
		lvl->p.colors[i].r = floatbuf;
		fscanf(file, "%f", &floatbuf);
		lvl->p.colors[i].g = floatbuf;
		fscanf(file, "%f", &floatbuf);
		lvl->p.colors[i].b = floatbuf;
	}

	// Número de tiles
	fscanf(file, "%d", &intbuf);
	lvl->n_tiles = intbuf;
	lvl->tiles = malloc(sizeof(tile) * lvl->n_tiles);
	// Carregar os tiles
	for(i = 0; i < lvl->n_tiles; i++)
	{
		fscanf(file, "%d", &intbuf);
		lvl->tiles[i].collidable = intbuf;
		fscanf(file, "%d", &intbuf);
		lvl->tiles[i].colorfrompallete = intbuf;
	}

	// Número de seções
	fscanf(file, "%d", &intbuf);
	lvl->n_sections = intbuf;
	lvl->sections = malloc(sizeof(section) * lvl->n_sections);
	// Carregar as seções
	for(i = 0; i < lvl->n_sections; i++)
		for(j = 0; j < (20 * 15); j++)
		{
			fscanf(file, "%d", &intbuf);
			lvl->sections[i].tiles[j] = intbuf;
		}
	// Seção inicial
	fscanf(file, "%d", &intbuf);
	lvl->init_section = intbuf;
	
	fclose(file);
	return true;
}

void unloadlevel(level* lvl)
{
	// Deletar cores, tiles e sections.
	free(lvl->p.colors);
	free(lvl->tiles);
	free(lvl->sections);
}

void rendersection(level* lvl, int currentsection)
{
	// A seção deve ser válida.
	if(currentsection < 0 || currentsection >= lvl->n_sections)
		return;
	int i;

	for(i = 0; i < (20 * 15); i++)
	{
		// Seleciona a cor a renderizar
		int targetTile = lvl->sections[currentsection].tiles[i];
		int targetColor = lvl->tiles[targetTile].colorfrompallete;
		glColor3f(lvl->p.colors[targetColor].r, lvl->p.colors[targetColor].g,
				 lvl->p.colors[targetColor].b);

		// Localiza o tile na projeção
		int   factor = i / 20;
		float xpos = (i - (20.0f * factor)) * TILESIZE_PX;
		float ypos = factor * TILESIZE_PX;

		glBegin(GL_QUADS);
			glVertex2f(xpos, ypos);
			glVertex2f(xpos + TILESIZE_PX, ypos);
			glVertex2f(xpos + TILESIZE_PX, ypos + TILESIZE_PX);
			glVertex2f(xpos, ypos + TILESIZE_PX);
		glEnd();		
	}
}
