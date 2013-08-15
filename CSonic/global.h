#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdafx.h"
#include "types.h"
#include "palette.h"
#include "level.h"

// 320x240, 30Hz
#define WIN_WIDTH    640
#define WIN_HEIGHT   480
#define REFRESHRATE 30.0

// TODO: Change your game name and icon here
#define GAMENAME    "CSonic Engine"
#define GAMEICON    "resources/icon.bmp"

// External definition for ending the game.
extern void quit();

// External definition for the main palette
extern palette MAINPALETTE;

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