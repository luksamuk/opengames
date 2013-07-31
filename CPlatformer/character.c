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

	// A direção inicial a se mover o personagem deve ser NENHUMA.
	chr->movToDir      =  NULL;
	chr->moveCountdown =     0;
	chr->currmov       =     0;
}

bool isnexttilesolid(character* chr, level* lvl)
{
	int nextTileX, nextTileY, finalTile, currTile;

	// Calcule, a partir da posição do personagem,
	// o próximo tile na matriz do jogo.
	nextTileX = (chr->x / TILESIZE_PX);
	nextTileY = (chr->y + 11.0f) / TILESIZE_PX;
	if(chr->dir == DIRECTION_LEFT)
		nextTileX -= 2;
	else nextTileX++;

	// Para o tile ao nível do pé
	finalTile = (nextTileY * 20) + nextTileX;
	currTile = lvl->sections[lvl->currentsection].tiles[finalTile];
	if(lvl->tiles[currTile].collidable)
		return true;
	nextTileY--;
	// Para o tile ao nível da cabeça
	finalTile = (nextTileY * 20) + nextTileX;
	currTile = lvl->sections[lvl->currentsection].tiles[finalTile];
	if(lvl->tiles[currTile].collidable)
		return true;
	return false;
}

void updatecharacter(character* chr, level* lvl)
{
	// Se houver uma direção registrada para o personagem
	// se mover, faça;
	if(chr->movToDir)
	{
		// Se o lugar para onde o personagem estiver olhando
		// for diferente da direção pretendida, então apenas
		// vire o personagem e conclua.
		if(chr->dir != (*chr->movToDir) && chr->moveCountdown == 0)
		{
			chr->dir = (*chr->movToDir);
			free(chr->movToDir);
			chr->movToDir = NULL;
			chr->moveCountdown = CHAR_WALK_DELAY;
		}
		// Se não, se o personagem está no lugar e o delay estiver
		// em 0, faça o personagem fazer o primeiro movimento.
		else if(chr->currmov == 0 && chr->moveCountdown == 0)
		{
			// Se houver algum tile sólido até 2 tiles de altura
			// do personagem logo à frente do mesmo, não ande.
			// O personagem deve estar também dentro de uma área
			// em que não pode ocorrer transição de seções.
			if(((chr->x / TILESIZE_PX) > 1 && (chr->x / TILESIZE_PX < 19))
				&& isnexttilesolid(chr, lvl))
			{
				free(chr->movToDir);
				chr->movToDir = NULL;
			}
			// Se não, continue a andar para a próxima direção.
			else
			{
				chr->x += (TILESIZE_PX / 2)
					* ((*chr->movToDir) == DIRECTION_LEFT ? -1 : 1);
				chr->currmov = 1;
				chr->moveCountdown = CHAR_WALK_DELAY;
			}
		}
		// Se o personagem já tiver feito o primeiro movimento,
		// faça o segundo movimento, e conclua.
		else if(chr->currmov == 1 && chr->moveCountdown == 0)
		{
			chr->x += (TILESIZE_PX / 2)
				* ((*chr->movToDir) == DIRECTION_LEFT ? -1 : 1);
			free(chr->movToDir);
			chr->movToDir = NULL;
			chr->currmov = 0;
			chr->moveCountdown = CHAR_WALK_DELAY;
			
			if(chr->x / TILESIZE_PX < 1)
			{
				lvl->currentsection--;
				// Reposicionar personagem à direita
				chr->x = 19 * TILESIZE_PX;
			}
			else if(chr->x / TILESIZE_PX > 19)
			{
				lvl->currentsection++;
				// Reposicionar personagem à esquerda
				chr->x = TILESIZE_PX;
			}
		}
	}

	// Contador regressivo
	if(chr->moveCountdown > 0)
	chr->moveCountdown--;
	// Define a animação atual
	chr->currentframe = chr->currmov;
}

bool movchartodir(character* chr, chardirection dir)
{
	// Caso não haja direções registradas para o personagem
	// se mover, registre a nova direção no ponteiro.
	if(chr->movToDir)
		return false;
	chr->movToDir = malloc(sizeof(chardirection));
	(*chr->movToDir) = dir;
	return true;
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
				float pointXpos = (i - (factor * chr->width)
                    - (chr->frames[chr->currentframe].hotspotX / 2))
                    * (chr->dir == DIRECTION_LEFT ? -1 : 1);
				float pointYpos = factor - (chr->frames[chr->currentframe].hotspotY / 2);
				pointXpos += chr->x;
				pointYpos += chr->y;
				// Desenhe.
				glVertex2f(pointXpos, pointYpos);
			}
		}
	glEnd();
}
