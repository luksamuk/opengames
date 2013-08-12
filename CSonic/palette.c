#include "palette.h"

float MASKTOFLOAT(color c)
{
	float res = c;
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
	byte i, j, k;
	p->numcolors = MAX_COLORS_NOMODES;
	p->data = malloc(sizeof(color) * p->numcolors);
	
	// REMEMBER: CornflowerBlue is 0x058E.
	// TODO: Define logic for building all colors.
	// Meanwhile all colors are black
	int currentcolor = 0;
	for(i = 0x00; i <= 0x0E; i += 0x02)
		for(j = 0x00; j <= 0x0E; j += 0x02)
			for(k = 0x00; k <= 0x0E; k += 0x02)
			{
				p->data[currentcolor] = createcolor(i, j, k);
				currentcolor++;
			}
}

void palette_load(palette* p, const char* filename)
{
	int i;
	FILE* file;
	file = fopen(filename, "rb");
	if(!file) return;

	fread(&p->numcolors, sizeof(int), 1, file);
	fread(p->data, sizeof(color), p->numcolors, file);

	fclose(file);
}

void  palette_export(palette* p, const char* filename)
{
	int i;
	FILE* file;
	file = fopen(filename, "wb");
	if(!file) return;
	fwrite(p->data, sizeof(color), p->numcolors, file);

	fclose(file);
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