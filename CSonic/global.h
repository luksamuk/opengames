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

// Size.
// Varies between 1x (default), 2x, 3x, 4x,
//720p and 1080p, full/windowed.
extern word WIN_WIDTH, WIN_HEIGHT;
extern bool fullscreen;


// Refresh Rate.
// Varies between 15, 20, 30 (default), 60.
extern double REFRESHRATE;


// Operations for ingame setup

/* Draw adjusting. Use on your draw calculations
 * and assume a screen always 320x240. */

// Draw adjusting for X coordinate
#define DRAWADJUST_x(n) (n * (WIN_WIDTH / 0x140))

// Draw adjusting for Y coordinate
#define DRAWADJUST_y(n) (n * (WIN_HEIGHT / 0x0F0))



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

#endif