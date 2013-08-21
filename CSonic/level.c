#include "level.h"

float DWORDTOFLOAT(dword d)
{
	float f = d;
	return f;
}

dword get_pos_relto(dword d, dword rel)
{
	return d / rel;
}

byte clamptobyte(int b, int min, int max)
{
	b = (b < min ? min : b);
	b = (b > max ? max : b);
	return (byte)b;
}

void level_init(level* lvl)
{
	word i, j;

	// Default size: 100x20 chunks
	lvl->levelsize_x = 0x3200;
	lvl->levelsize_y = 0x2560;
	for(i = 0x00; i < 0xFF; i += 0x01)
		for(j = 0x00; i < 0xFF; i += 0x01)
			lvl->mapping[i][j] = 0x00;
	// Reset camera
	lvl->camera = new_vec2(0x00000080, 0x00000080);
	level_cameraclamp(lvl);
}

void level_load(level* lvl)
{

}

void level_rendertile(level* lvl, byte tile, vec2 pos)
{
	dword i, j;
	int pox, poy, px, py;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
		{
			color c = lvl->tiles[tile].colors[i][j];
			if(c != COLOR_TRANSPARENT)
			{
				pox = DRAWADJUST_x(pos.x) + DRAWADJUST_x(i);
				px  = pox + DRAWADJUST_x(1);
				poy = DRAWADJUST_y(pos.y) + DRAWADJUST_y(j);
				py  = poy + DRAWADJUST_y(1);
				glColorM(c);
				glBegin(GL_QUADS);
					glVertex2i(pox, poy);
					glVertex2i(px, poy);
					glVertex2i(px, py);
					glVertex2i(pox, py);
				glEnd();
			}
		}
}

void level_renderpiece(level* lvl, byte piece, vec2 pos)
{
	dword i, j;
	for(i = 0; i < 2; i++)
		for(j = 0; j < 2; j++)
		{
			vec2 tilepos;
			tilepos.x = pos.x + (8 * i);
			tilepos.y = pos.y + (8 * j);
			level_rendertile(lvl,
				             lvl->pieces[piece].tiles[i][j],
				             tilepos);
		}
}

void level_renderchunk(level* lvl, byte chunk, vec2 pos)
{
	// Chunk 0 is always blank.
	if(chunk == 0x00) return;
	dword i, j;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
		{
			vec2 piecepos;
			piecepos.x = pos.x + (16 * i);
			piecepos.y = pos.y + (16 * j);
			level_renderpiece(lvl,
				              lvl->chunks[chunk].pieces[i][j],
				              piecepos);
		}
}

void level_renderlevel(level* lvl)
{
	dword i, j, Xpos, Ypos;
	vec2 min, max;

	// Clamp level rendering to character position by hash.
	Xpos = get_pos_relto(lvl->camera.x, DRAWADJUST_x(128));
	Ypos = get_pos_relto(lvl->camera.y, DRAWADJUST_y(128));
	min.x = clamptobyte((int)Xpos - 3,
		0,
		get_pos_relto(lvl->levelsize_x, DRAWADJUST_x(128)) + 1);
	min.y = clamptobyte((int)Ypos - 2,
		0,
		get_pos_relto(lvl->levelsize_y, DRAWADJUST_y(128)) + 1);
	max.x = clamptobyte((int)Xpos + 3,
		0,
		get_pos_relto(lvl->levelsize_x, DRAWADJUST_x(128)) + 1);
	max.y = clamptobyte((int)Ypos + 2,
		0,
		get_pos_relto(lvl->levelsize_y, DRAWADJUST_y(128)) + 1);

	// Render active sections
	for(i = min.x; i < max.x; i++)
		for(j = min.y; j < max.y; j++)
		{
			level_renderchunk(lvl,
				              lvl->mapping[i][j],
				              new_vec2(i * 128, j * 128));
		}

}

vec2 level_getcamera(level* lvl)
{
	return new_vec2(DRAWADJUST_x(lvl->camera.x),
		DRAWADJUST_y(lvl->camera.y));
}

void level_cameraclamp(level* lvl)
{
	if(lvl->camera.x < 0xA0u)
		lvl->camera.x = 0xA0u;
	else if(lvl->camera.x > (lvl->levelsize_x - 0xA0u))
		lvl->camera.x = (lvl->levelsize_x - 0xA0u);

	if(lvl->camera.y < 0x78u)
		lvl->camera.y = 0x78u;
	else if(lvl->camera.y > (lvl->levelsize_y - 0x78u))
		lvl->camera.y = (lvl->levelsize_y - 0x78u);
}

void level_ortho_camera(level* lvl)
{
	vec2 cam = lvl->camera;
	cam.x = DRAWADJUST_x(cam.x);
	cam.y = DRAWADJUST_y(cam.y);

	GLdouble left, right, top, bottom;

	left = cam.x - (WIN_WIDTH / 2);
	right = cam.x + (WIN_WIDTH / 2);
	top = cam.y + (WIN_HEIGHT / 2);
	bottom = cam.y - (WIN_HEIGHT / 2);

	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(left, right, top, bottom, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void level_createtest(level* lvl)
{
	word i, j;
	tile8 t;
	tile16 p;
	tile128 c;

	// Grass 1
	t.colors[0][0] = t.colors[1][0] = COLOR_GREEN;
	t.colors[0][1] = t.colors[1][1] = COLOR_GREEN;
	t.colors[0][2] = t.colors[1][2] = COLOR_GREEN;
	t.colors[0][3] = t.colors[1][3] = COLOR_GREEN;
	t.colors[0][4] = t.colors[1][4] = COLOR_GREEN;
	t.colors[0][5] = t.colors[1][5] = COLOR_GREEN;
	t.colors[0][6] = t.colors[1][6] = COLOR_GREEN;
	t.colors[0][7] = t.colors[1][7] = COLOR_GREEN;

	t.colors[2][0] = t.colors[3][0] = COLOR_TRANSPARENT;
	t.colors[2][1] = t.colors[3][1] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][2] = t.colors[3][2] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][3] = t.colors[3][3] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][4] = t.colors[3][4] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][5] = t.colors[3][5] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][6] = t.colors[3][6] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][7] = t.colors[3][7] = SHADOWCOLOR(COLOR_GREEN);

	t.colors[4][0] = t.colors[5][0] = COLOR_TRANSPARENT;
	t.colors[4][1] = t.colors[5][1] = COLOR_TRANSPARENT;
	t.colors[4][2] = t.colors[5][2] = COLOR_GREEN;
	t.colors[4][3] = t.colors[5][3] = COLOR_GREEN;
	t.colors[4][4] = t.colors[5][4] = COLOR_GREEN;
	t.colors[4][5] = t.colors[5][5] = COLOR_GREEN;
	t.colors[4][6] = t.colors[5][6] = COLOR_GREEN;
	t.colors[4][7] = t.colors[5][7] = COLOR_GREEN;

	t.colors[6][0] = t.colors[7][0] = COLOR_TRANSPARENT;
	t.colors[6][1] = t.colors[7][1] = COLOR_TRANSPARENT;
	t.colors[6][2] = t.colors[7][2] = COLOR_TRANSPARENT;
	t.colors[6][3] = t.colors[7][3] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][4] = t.colors[7][4] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][5] = t.colors[7][5] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][6] = t.colors[7][6] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][7] = t.colors[7][7] = SHADOWCOLOR(COLOR_GREEN);

	lvl->tiles[2] = t;

	// Grass 2
	t.colors[0][0] = t.colors[1][0] = COLOR_TRANSPARENT;
	t.colors[0][1] = t.colors[1][1] = COLOR_TRANSPARENT;
	t.colors[0][2] = t.colors[1][2] = COLOR_TRANSPARENT;
	t.colors[0][3] = t.colors[1][3] = COLOR_GREEN;
	t.colors[0][4] = t.colors[1][4] = COLOR_GREEN;
	t.colors[0][5] = t.colors[1][5] = COLOR_GREEN;
	t.colors[0][6] = t.colors[1][6] = COLOR_GREEN;
	t.colors[0][7] = t.colors[1][7] = COLOR_GREEN;

	t.colors[2][0] = t.colors[3][0] = COLOR_TRANSPARENT;
	t.colors[2][1] = t.colors[3][1] = COLOR_TRANSPARENT;
	t.colors[2][2] = t.colors[3][2] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][3] = t.colors[3][3] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][4] = t.colors[3][4] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][5] = t.colors[3][5] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][6] = t.colors[3][6] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[2][7] = t.colors[3][7] = SHADOWCOLOR(COLOR_GREEN);

	t.colors[4][0] = t.colors[5][0] = COLOR_TRANSPARENT;
	t.colors[4][1] = t.colors[5][1] = COLOR_GREEN;
	t.colors[4][2] = t.colors[5][2] = COLOR_GREEN;
	t.colors[4][3] = t.colors[5][3] = COLOR_GREEN;
	t.colors[4][4] = t.colors[5][4] = COLOR_GREEN;
	t.colors[4][5] = t.colors[5][5] = COLOR_GREEN;
	t.colors[4][6] = t.colors[5][6] = COLOR_GREEN;
	t.colors[4][7] = t.colors[5][7] = COLOR_GREEN;

	t.colors[6][0] = t.colors[7][0] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][1] = t.colors[7][1] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][2] = t.colors[7][2] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][3] = t.colors[7][3] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][4] = t.colors[7][4] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][5] = t.colors[7][5] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][6] = t.colors[7][6] = SHADOWCOLOR(COLOR_GREEN);
	t.colors[6][7] = t.colors[7][7] = SHADOWCOLOR(COLOR_GREEN);

	lvl->tiles[1] = t;

	// Dirt 1
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			t.colors[i][j] = 0x0AA2;
	lvl->tiles[3] = t;

	// Dirt 2
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			t.colors[i][j] = SHADOWCOLOR(0x0AA2);
	lvl->tiles[4] = t;

	// Dirt and grass 1
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			t.colors[i][j] = SHADOWCOLOR(0x0AA2);
	// Grass
	for(i = 0; i < 4; i++)
		t.colors[0][i] = t.colors[1][i] = 0x0082;
	for(i = 0; i < 3; i++)
		t.colors[2][i] = t.colors[3][i] = SHADOWCOLOR(0x00C4);
	for(i = 0; i < 2; i++)
		t.colors[4][i] = t.colors[5][i] = 0x0082;
	for(i = 0; i < 1; i++)
		t.colors[6][i] = t.colors[7][i] = SHADOWCOLOR(0x00C4);
	// Shading
	t.colors[0][4] = t.colors[1][4] =
	t.colors[0][5] = t.colors[1][5] =
	t.colors[2][3] = t.colors[3][3] =
	t.colors[2][4] = t.colors[3][4] =
	t.colors[4][2] = t.colors[5][2] =
	t.colors[4][3] = t.colors[5][3] =
	t.colors[6][1] = t.colors[7][1] =
	t.colors[6][2] = t.colors[7][2] = SHADOWCOLOR(0x0042);
	lvl->tiles[5] = t;


	// Dirt and grass 2
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			t.colors[i][j] = 0x0AA2;
	// Grass
	for(i = 0; i < 1; i++)
		t.colors[0][i] = t.colors[1][i] = 0x0082;
	for(i = 0; i < 2; i++)
		t.colors[2][i] = t.colors[3][i] = SHADOWCOLOR(0x00C4);
	for(i = 0; i < 3; i++)
		t.colors[4][i] = t.colors[5][i] = 0x0082;
	for(i = 0; i < 4; i++)
		t.colors[6][i] = t.colors[7][i] = SHADOWCOLOR(0x00C4);
	// Shading
	t.colors[0][1] = t.colors[1][1] =
	t.colors[0][2] = t.colors[1][2] =
	t.colors[2][2] = t.colors[3][2] =
	t.colors[2][3] = t.colors[3][3] =
	t.colors[4][3] = t.colors[5][3] =
	t.colors[4][4] = t.colors[5][4] =
	t.colors[6][4] = t.colors[7][4] =
	t.colors[6][5] = t.colors[7][5] = SHADOWCOLOR(0x0042);
	lvl->tiles[6] = t;


	// Grass
	p.tiles[0][0] = 1; p.tiles[1][0] = 2;
	p.tiles[0][1] = 5; p.tiles[1][1] = 6;
	lvl->pieces[1] = p;
	// Dirt
	p.tiles[0][0] = 3; p.tiles[1][0] = 4;
	p.tiles[0][1] = 4; p.tiles[1][1] = 3;
	lvl->pieces[2] = p;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			if(!j) c.pieces[i][j] = 1;
			else c.pieces[i][j] = 2;

	lvl->chunks[1] = c;
	lvl->mapping[0][1] = lvl->mapping[1][1] = lvl->mapping[2][1] = 1;
}