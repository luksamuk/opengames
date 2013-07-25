/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * character.h
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "stdafx.h"
#include "pallete.h"

typedef struct animframe
{
	int hotspotX, hotspotY;
	int* colorfrompallete;
} animframe;

typedef enum
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT
} chardirection;

typedef struct character
{
	pallete p;
	int n_frames;
	int width, height;
	chardirection dir;
	animframe* frames;

	float x, y;
} character;

bool loadcharacter(char*, character*);
void initcharacter(character*);
void unloadcharacter(character*);
void rendercharf(character*);

#endif
