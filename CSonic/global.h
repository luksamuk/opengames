#ifndef GLOBAL_H
#define GLOBAL_H

// Some obligatory headers for most files.
#include "stdafx.h"
#include "types.h"
#include "input.h"
#include "palette.h"




/* GENERAL PROPERTIES */

// TODO: Change your game name and icon here
#define GAMENAME    "CSonic Engine"
#define GAMEICON    "resources/icon.bmp"




/* SCREEN DEFINITIONS */

// Size

/* 320x240 - single size */
//#define WIN_WIDTH  0x140
//#define WIN_HEIGHT 0x0F0

/* 640x480 - double size */
#define WIN_WIDTH    0x280
#define WIN_HEIGHT   0x1E0

/* 960x720 - triple size */
//#define WIN_WIDTH    0x3C0
//#define WIN_HEIGHT   0x2D0




// Refresh Rate

/* 30Hz */
//#define REFRESHRATE 30.0
/* 60Hz */
#define REFRESHRATE 60.0



// Operations for ingame setup

/* Draw adjusting. Use on your draw calculations
 * and assume a screen always 320x240. */

// Draw adjusting for X coordinate
#define DRAWADJUST_X(x) (x * (WIN_WIDTH / 0x140))

// Draw adjusting for Y coordinate
#define DRAWADJUST_Y(y) (y * (WIN_HEIGHT / 0x0F0))



/* Step adjust. Use on your FPS-based calculations
 * and assuma a screen always refreshing @ 30Hz. */
#define STEPADJUST(s) (s * (30.0 / REFRESHRATE))






/* EXTERNAL GLOBAL VARIABLES
 * Most of them are defined on main.c */

// External surface of the game
extern SDL_Surface* surface;

// External definition for ending the game.
extern void quit();

// External definition for the main palette
extern palette MAINPALETTE;

// External definition for the main Input States
extern inputstate INPUT_STATE;
extern inputstate INPUTSTATE_OLD;






/* USEFUL GLOBAL FUNCTIONS */

// Sets OpenGL to use a color from the palette as
// clear color
void glClearColorM(color c)
{
	glClearColor(MASKTOFLOAT(GETRCOLOR(c)),
		         MASKTOFLOAT(GETGCOLOR(c)),
		         MASKTOFLOAT(GETBCOLOR(c)),
		         1.0f);
}

// Sets OpenGL to render a color from the palette
void glColorM(color c)
{
	glColor3b(MASKTOBYTE(GETRCOLOR(c)),
			  MASKTOBYTE(GETGCOLOR(c)),
			  MASKTOBYTE(GETBCOLOR(c)));
}

#endif