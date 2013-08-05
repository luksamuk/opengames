#include "palette.h"

GLfloat MASKTOFLOAT(color c)
{
	GLfloat res = c;
	res /= 16;
	return res;
}

color createcolor(byte r, byte g, byte b)
{
	color c = 0x0000;
	// 0000 00000 0000 0000
	MIXCOLOR(c,r);
	// 0000 0000 0000 RRRR
	NEXTCOLOR(c);
	// 0000 0000 RRRR 0000
	MIXCOLOR(c,g);
	// 0000 0000 RRRR GGGG
	NEXTCOLOR(c);
	// 0000 RRRR GGGG 0000
	MIXCOLOR(c,b);
	// 0000 RRRR GGGG BBBB
	return c;
}

void gpalette_load(palette* p)
{
	int i;
	p->data = malloc(sizeof(color) * MAX_COLORS_NOMODES);
	
	// REMEMBER: CornflowerBlue is 0x058E.
	// TODO: Define logic for building all colors.
	// Meanwhile all colors are black
	for(i = 0; i < MAX_COLORS_NOMODES; i++)
		p->data[i] = createcolor(COLOR_0, COLOR_0, COLOR_0);
}

void palette_load(palette* p)
{
	
}

void palette_unload(palette* p)
{
	free(p->data);
}

color getcolor(palette* p, int idx)
{
	if(idx < 0 || idx > 15)
		return COLOR_0;
	return p->data[idx];
}

void dumpcolor(color c)
{
	printf("0x%04X -> %d\n", c, c);
}