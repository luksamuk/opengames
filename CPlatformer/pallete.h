/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * pallete.h
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

#ifndef PALLETE_H
#define PALLETE_H

#include "stdafx.h"

typedef struct color
{
	GLfloat r, g, b;
} color;

typedef struct pallete
{
	int    n_colors;
	color* colors; 
} pallete;

#endif
