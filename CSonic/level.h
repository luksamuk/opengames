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
	byte       mapping[255][255];
	word       levelsize_x;
	word       levelsize_y;
	vec2       camera;
	// TODO:
	// Object mapping
	// Object placement
	// Level events
} level;

float DWORDTOFLOAT(dword);
dword get_pos_relto(dword, dword);
byte clamptobyte(int, int, int);
void level_init(level*);
void level_load(level*);
void level_rendertile(level*, byte, vec2);
void level_renderpiece(level*, byte, vec2);
void level_renderchunk(level*, byte, vec2);
void level_renderlevel(level*);

// Camera settings
vec2 level_getcamera(level*);
void level_cameraclamp(level*);
void level_ortho_camera(level*);

// Creates a test level out of code.
void level_createtest(level*);

#endif