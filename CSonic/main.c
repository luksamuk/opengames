#include "stdafx.h"
#include "types.h"
#include "palette.h"
#include "level.h"

int main()
{
	color c;
	// Ao criar a cor, pode-se utilizar os macros
	c = createcolor(COLOR_0, COLOR_1, COLOR_1);
	// Ou criar a cor direto com hex.
	//c = 0x022;

	printf("Color:\n");
	dumpcolor(c);
	dumpcolor(SHADOWCOLOR(c));
	dumpcolor(HIGHLIGHTCOLOR(c));
	printf("\nR: "); dumpcolor(GETRCOLOR(c));
	printf("G: "); dumpcolor(GETGCOLOR(c));
	printf("B: "); dumpcolor(GETBCOLOR(c));

	int r, g, b;
	r = MASKTOBYTE(GETRCOLOR(c));
	g = MASKTOBYTE(GETGCOLOR(c));
	b = MASKTOBYTE(GETBCOLOR(c));
	printf("Color is (%d, %d, %d)\n", r, g, b);
	return 0;
}