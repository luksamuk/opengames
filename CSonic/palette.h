#ifndef PALETTE_H
#define PALETTE_H

#include "types.h"

// Colors are all stored in 16-bit format 0x0RGB.

/* Macros */
// System definitions
#define MAX_COLORS_NOMODES   512u


// Definitions for building your own color.
// Each definition is 8-bit sized
#define COLOR_DEF_0              0x00u
#define COLOR_DEF_1              0x02u
#define COLOR_DEF_2              0x04u
#define COLOR_DEF_3              0x06u
#define COLOR_DEF_4              0x08u
#define COLOR_DEF_5              0x0Au
#define COLOR_DEF_6              0x0Cu
#define COLOR_DEF_7              0x0Eu


// Standard colors
#define COLOR_TRANSPARENT    0xF000u
#define COLOR_BLACK          0x0000u
#define COLOR_RED            0x0E00u
#define COLOR_GREEN          0x00E0u
#define COLOR_BLUE           0x000Eu
#define COLOR_WHITE          0x0EEEu
#define COLOR_CORNFLOWERBLUE 0x068Eu


// Color creation operations
#define NEXTCOLOR(x)  (x = (x << 4))
#define MIXCOLOR(x,y) (x = (x | y))
#define SHADOWCOLOR(x) (x >> 1)
#define HIGHLIGHTCOLOR(x) ((x >> 1) + 0x0777u)


// Color extraction operations
#define GETRCOLOR(x) ((x ^ ~(~x | 0x000Fu << 8)) >> 8)
#define GETGCOLOR(x) ((x ^ ~(~x | 0x000Fu << 4)) >> 4)
#define GETBCOLOR(x) (x ^ ~(~x | 0x000Fu))
#define MASKTOBYTE(x) (x * 8)

typedef word color;

typedef struct PALETTE_DEF
{
	int numcolors;
	color* data;
} palette;


float MASKTOFLOAT(color);
float GETCOLORHUE(color);
color createcolor(byte, byte, byte);
void  gpalette_load(palette*);
void  palette_load(palette*, const char*);
void  palette_export(palette*, const char*);
void  palette_unload(palette*);
color getcolor(palette*, int);
void  dumpcolor(color);


/* USEFUL GLOBAL FUNCTIONS */

// Sets OpenGL to use a color from the palette as
// clear color
void glClearColorM(color);
void glClearAccumM(color);

// Sets OpenGL to render a color from the palette
void glColorM(color);

#endif // PALETTE_H