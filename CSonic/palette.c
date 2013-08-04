#include "palette.h"

color createcolor(byte r, byte g, byte b)
{
	color c;
	c = 0x00;
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

void palette_init(palette* p)
{
	color c;
	createcolor(COLOR_0, COLOR_0, COLOR_0);
}

void palette_load(palette* p)
{
	
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