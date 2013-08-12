#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdafx.h"
#include "types.h"
#include "palette.h"
#include "level.h"

// 320x240, 30Hz
#define WIN_WIDTH    600
#define WIN_HEIGHT   480
#define REFRESHRATE 30.0

// TODO: Change your game name and icon here
#define GAMENAME    "CSonic Engine"
#define GAMEICON    "resources/icon.bmp"

// External definition for ending the game.
extern void quit();

// External definition for the main palette
extern palette MAINPALETTE;

#endif