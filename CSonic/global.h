#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdafx.h"
#include "types.h"
#include "input.h"
#include "palette.h"
#include "level.h"

// 320x240
//#define WIN_WIDTH  0x140
//#define WIN_HEIGHT 0x0F0

// 640x480 - double
//#define WIN_WIDTH    0x280
//#define WIN_HEIGHT   0x1E0

// 960x720 - triple
#define WIN_WIDTH    0x3C0
#define WIN_HEIGHT   0x2D0

// 30Hz
//#define REFRESHRATE 30.0

// 60Hz
#define REFRESHRATE 60.0



// TODO: Change your game name and icon here
#define GAMENAME    "CSonic Engine"
#define GAMEICON    "resources/icon.bmp"

// External surface of the game
extern SDL_Surface* surface;

// External definition for ending the game.
extern void quit();

// External definition for the main palette
extern palette MAINPALETTE;

// External definition for the main Input States
extern inputstate INPUT_STATE;
extern inputstate INPUTSTATE_OLD;

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