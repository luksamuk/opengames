#ifndef PALETTE_H
#define PALETTE_H

#include "types.h"

// Colors are all stored in 16-bit format 0x0RGB.

/* Macros */
// System definitions
#define MAX_COLORS_NOMODES   512


// Definitions for building your own color.
// Each definition is 8-bit sized
#define COLOR_DEF_0          0x00
#define COLOR_DEF_1              0x02
#define COLOR_DEF_2              0x04
#define COLOR_DEF_3              0x06
#define COLOR_DEF_4              0x08
#define COLOR_DEF_5              0x0A
#define COLOR_DEF_6              0x0C
#define COLOR_DEF_7              0x0E


// Standard colors
#define COLOR_BLACK          SHADOWCOLOR(0x0000)
#define COLOR_WHITE          HIGHLIGHTCOLOR(0x0EEE)
#define COLOR_CORNFLOWERBLUE 0x068E


// Color creation operations
#define NEXTCOLOR(x)  (x = (x << 4))
#define MIXCOLOR(x,y) (x = (x | y))
#define SHADOWCOLOR(x) (x >> 1)
#define HIGHLIGHTCOLOR(x) ((x >> 1) + 0x0077)


// Color extraction operations
#define GETRCOLOR(x) ((x ^ ~(~x | 0x000F << 8)) >> 8)
#define GETGCOLOR(x) ((x ^ ~(~x | 0x000F << 4)) >> 4)
#define GETBCOLOR(x) (x ^ ~(~x | 0x000F))
#define MASKTOBYTE(x) (x * 16)

typedef word color;

typedef struct PALETTE_DEF
{
	int numcolors;
	color* data;
} palette;

// Converts color mask to GLfloat
float MASKTOFLOAT(color);
// Retrieves hue from a color
float GETCOLORHUE(color);

// Creates a color from three bytes
color createcolor(byte, byte, byte);

// Loads the global palette
void  gpalette_load(palette*);

// Loads a palette from a file
void  palette_load(palette*, const char*);

// Exports palette to file
void  palette_export(palette*, const char*);

// Unloads any palette
void  palette_unload(palette*);

// Returns a color from a palette
color getcolor(palette*, int);

// Prints color info on console
void  dumpcolor(color);

#endif // PALETTE_H