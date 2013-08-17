#include "palette.h"

float MASKTOFLOAT(color c)
{
	float res = c;
	res /= 16;
	return res;
}
float GETCOLORHUE(color c)
{
	float r, g, b, MAX, MIN;
	// Convert RGB(bitmask) to RGB(float)
	r = MASKTOFLOAT(GETRCOLOR(c));
	g = MASKTOFLOAT(GETGCOLOR(c));
	b = MASKTOFLOAT(GETBCOLOR(c));

	// Selects MAX value and MIN value
	MAX = MIN = r;
	if(MAX < g) MAX = g;
	if(MAX < b) MAX = b;
	if(MIN > g) MIN = g;
	if(MIN > b) MIN = b;

	if(!(MAX - MIN)) { MAX = 1.0f; MIN = 0.0f; }

	// Finally, calculate hue and return
	if(MAX == r)
	{
		float res = 60.0f * ((g - b) / (MAX - MIN));
		if(g >= b) return res;
		else return res += 360.0f;
	}
	else if(MAX == g)
		return 60.0f * ((b - r) / (MAX - MIN)) + 120.0f;
	else return 60.0f * ((r - g) / (MAX - MIN)) + 240.0f;

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
	p->data = malloc(sizeof(color) * MAX_COLORS_NOMODES);

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
	FILE* file;
	file = fopen(filename, "rb");
	if(!file) return;

	fread(&p->numcolors, sizeof(int), 1, file);
	fread(p->data, sizeof(color), p->numcolors, file);

	fclose(file);
}

void  palette_export(palette* p, const char* filename)
{
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
	if(idx < 0 || idx > p->numcolors)
		return COLOR_DEF_0;
	return p->data[idx];
}

void dumpcolor(color c)
{
	printf("0x%04X -> %d\n", c, c);
}

void glClearColorM(color c)
{
	glClearColor(MASKTOFLOAT(GETRCOLOR(c)),
		         MASKTOFLOAT(GETGCOLOR(c)),
		         MASKTOFLOAT(GETBCOLOR(c)),
		         1.0f);
}

void glColorM(color c)
{
	glColor3b(MASKTOBYTE(GETRCOLOR(c)),
			  MASKTOBYTE(GETGCOLOR(c)),
			  MASKTOBYTE(GETBCOLOR(c)));
}