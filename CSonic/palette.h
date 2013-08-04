#ifndef PALETTE_H
#define PALETTE_H

#include "types.h"

// Macros
// Cores
#define COLOR_0 0x00
#define COLOR_1 0x02
#define COLOR_2 0x04
#define COLOR_3 0x06
#define COLOR_4 0x08
#define COLOR_5 0x0A
#define COLOR_6 0x0C
#define COLOR_7 0x0F
// Operações de criação de cores
#define NEXTCOLOR(x)  (x = (x << 4))
#define MIXCOLOR(x,y) (x = (x | y))
#define SHADOWCOLOR(x) (x >> 1)
#define HIGHLIGHTCOLOR(x) ((x >> 1) + 0x77)
// Operações de extração de cores
#define GETRCOLOR(x) ((x ^ ~(~x | 0x0F << 8)) >> 8)
#define GETGCOLOR(x) ((x ^ ~(~x | 0x0F << 4)) >> 4)
#define GETBCOLOR(x) (x ^ ~(~x | 0x0F))
#define MASKTOBYTE(x) (x * 16)

// 0x0RGB
typedef word color;

typedef struct PALETTE_DEF
{
	color data[16];
} palette;


color createcolor(byte, byte, byte);
void palette_init(palette*);
void palette_load(palette*);
color getcolor(palette*, int);
void dumpcolor(color);

#endif // PALETTE_H