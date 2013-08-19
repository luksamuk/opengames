#include "level.h"

float DWORDTOFLOAT(dword d)
{
	float f = d;
	return f;
}

void level_init(level* lvl)
{
	word i, j;

	// Default size: 100x20 chunks
	lvl->levelsize_x = 0x0064;
	lvl->levelsize_y = 0x0014;
	for(i = 0x00; i < 0xFF; i += 0x01)
		for(j = 0x00; i < 0xFF; i += 0x01)
			lvl->mapping[i][j] = 0x00;
}

void level_load(level* lvl)
{

}

void level_rendertile(level* lvl, byte tile, vec2 pos)
{
	dword i, j;
	float pox, poy, px, py;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
		{

			pox = DRAWADJUST_x(DWORDTOFLOAT(pos.x + i));
			px  = pox + DRAWADJUST_x(8);
			poy = DRAWADJUST_y(DWORDTOFLOAT(pos.y + j));
			py  = poy + DRAWADJUST_y(8);

			glColorM(lvl->tiles[tile].colors[i][j]);
			glBegin(GL_QUADS);
				glVertex2f(pox, poy);
				glVertex2f(px, poy);
				glVertex2f(px, py);
				glVertex2f(pox, py);
			glEnd();
		}
}

void level_renderpiece(level* lvl, byte piece, vec2 pos)
{
	dword i, j;
	for(i = 0; i < 2; i++)
		for(j = 0; j < 2; j++)
		{
			vec2 tilepos;
			tilepos.x = pos.x + ((16 / 2) * i);
			tilepos.y = pos.y + ((16 / 2) * j);
			level_rendertile(lvl,
				             lvl->pieces[piece].tiles[i][j],
				             tilepos);
		}
}

void level_renderchunk(level* lvl, byte chunk, vec2 pos)
{
	dword i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
		{
			vec2 piecepos;
			piecepos.x = pos.x + ((128 / 8) * i);
			piecepos.y = pos.y + ((128 / 8) * j);
			level_renderpiece(lvl,
				              lvl->chunks[chunk].pieces[i][j],
				              piecepos);
		}
}

void level_renderlevel(level* lvl)
{
	dword i, j;
	for(i = 0x00; i < 0xFF; i++)
		for(j = 0x00; j < 0xFF; j++)
			level_renderchunk(lvl,
				              lvl->mapping[i][j],
				              new_vec2(i * 128, j * 128));

}