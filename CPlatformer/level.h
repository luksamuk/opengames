/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * level.h
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include "stdafx.h"
#include "pallete.h"

#define TILESIZE_PX (WIN_WIDTH / 20)

typedef struct tile
{
	bool collidable;
	int  colorfrompallete;
} tile;

typedef struct section
{
	int tiles[20 * 15];
} section;

typedef struct level
{
	int      n_sections, init_section;
	int      n_tiles;
	tile*    tiles;
	section* sections;
	pallete  p;
} level;

bool loadlevel(char*, level*);
void unloadlevel(level*);
void rendersection(level*, int currentsection);

#endif
