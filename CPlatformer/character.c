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
		chr->frames[i].colorfrompallete = malloc(sizeof(int) * arraysize);
		for(j = 0; j < arraysize; j++)
		{
			fscanf(file, "%d", &intbuf);
			chr->frames[i].colorfrompallete[j] = intbuf - 1;
		}
	}
	fclose(file);
	return true;
}

void initcharacter(character* chr, level* lvl)
{
	// Inicializa direção
	chr->dir = DIRECTION_RIGHT;
	// Inicializa a posição inicial
	chr->x = ((lvl->init_pos_x + 1) * TILESIZE_PX) - (TILESIZE_PX / 2);
	chr->y = ((lvl->init_pos_y + 1) * TILESIZE_PX) - (TILESIZE_PX / 2);

	chr->x -= chr->frames[0].hotspotX;
	chr->y -= chr->frames[0].hotspotY;
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
	int currentcolor, i;
	GLfloat r, g, b;
	currentcolor = -1;
	// Renderizar personagem na tela.
	// TODO: Levar direção em consideração.
	glBegin(GL_POINTS);
		for(i = 0; i < (chr->width * chr->height); i++)
		{
			int getcolor = chr->frames[chr->currentframe].colorfrompallete[i];
			// Se não houver cor, não renderize.
			if(getcolor > -1)
			{
				// Se for uma cor diferente da utilizada anteriorimente,
				// troque o estado de máquina.
				if(currentcolor != getcolor)
				{
					currentcolor = getcolor;
					r = chr->p.colors[currentcolor].r;
					g = chr->p.colors[currentcolor].g;
					b = chr->p.colors[currentcolor].b;
					glColor3f(r, g, b);
				}
				// Calcule a posição correta do pixel
				int factor = i / chr->width;
				float pointXpos = i - (factor * chr->width)
                    - (chr->frames[chr->currentframe].hotspotX / 2);
				float pointYpos = factor - (chr->frames[chr->currentframe].hotspotY / 2);
				pointXpos += chr->x;
				pointYpos += chr->y;
				// Desenhe.
				glVertex2f(pointXpos, pointYpos);
			}
		}
	glEnd();
}
