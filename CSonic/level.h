#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"

typedef struct TILE_8X8
{
	color colors[8][8] = {0};
} tile8;

typedef struct TILE_16X16
{
	byte tiles[2][2]   = {0};
	// TODO:
	// Collision
} tile16;

typedef struct TILE_128X128
{
	byte pieces[8][8]  = {0};
} tile128;

typedef struct LEVELDEF
{
	tile8      tiles  [255];
	tile16     pieces [255];
	tile128    chunks [255];
	word       levelsize_X;
	word       levelsize_Y;
	// TODO:
	// Object mapping
	// Object placement
	// Level events
} level;

void level_load(level*);
void level_rendertile(tile8*);
void level_renderpiece(tile16*);
void level_renderchunk(tile128*);
void level_renderlevel(level*);

#endif