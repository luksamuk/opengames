#include "level.h"

void level_init(level* lvl)
{
	word i, j;

	// Default size: 100x20 chunks
	lvl->levelsize_x = 0x0064;
	lvl->levelsize_y = 0x0014;
	lvl->mapping = malloc(sizeof(byte*) * lvl->levelsize_x);
	for(i = 0x0000; i < lvl->levelsize_x; i += 0x0001)
	{
		lvl->mapping[i] = malloc(sizeof(byte) * lvl->levelsize_y);
		for(j = 0x0000; i < lvl->levelsize_y; i += 0x0001)
			lvl->mapping[i][j] = 0x00;
	}
}

void level_load(level* lvl)
{

}

void level_rendertile(byte tile)
{

}

void level_renderpiece(byte piece)
{

}

void level_renderchunk(byte chunk)
{

}

void level_renderlevel(level* lvl)
{
	/* TODO */
	/*
	byte i;
	for(i = 0x00; i <= 0xFF; i += 0x01)
	{
		level_renderchunk
	}
	*/
}