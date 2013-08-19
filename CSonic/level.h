#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"

typedef struct TILE_8X8
{
	color colors[8][8];
} tile8;

typedef struct TILE_16X16
{
	byte tiles[2][2];
	// TODO:
	// Collision
} tile16;

typedef struct TILE_128X128
{
	byte pieces[8][8];
} tile128;

typedef struct LEVELDEF
{
	tile8      tiles  [255];
	tile16     pieces [255];
	tile128    chunks [255];
	byte**     mapping;
	word       levelsize_x;
	word       levelsize_y;
	// TODO:
	// Object mapping
	// Object placement
	// Level events
} level;

void level_init(level*);
void level_load(level*);
void level_rendertile(byte);
void level_renderpiece(byte);
void level_renderchunk(byte);
void level_renderlevel(level*);

#endif