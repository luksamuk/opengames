#ifndef LEVEL_H
#define LEVEL_H

#include "palette.h"

typedef struct TILE_8X8
{
	byte colornum[8][8];
} tile8;

typedef struct TILE_16X16
{
	byte tilenum[2][2];
} tile16;

typedef struct TILE_128X128
{
	byte piecenum[8][8];
} tile128;

typedef struct LEVELDEF
{
	palette    p          ;
	tile8      tiles  [255];
	tile16     pieces [255];
	tile128    chunks [255];

} level;

void level_load(level*);
void level_render(level*);

#endif