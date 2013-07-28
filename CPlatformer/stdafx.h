/* CPlatformer - Platformer feito em C,
 * SDL e OpenGL.
 *
 * stdafx.h
 *
 * Criado por Lucas Vieira
 * Unifei - Campus Itabira, 2013.
 */

#ifndef STDAFX_H
#define STDAFX_H

// Bibliotecas
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Definições
#define WIN_WIDTH  640
#define WIN_HEIGHT 480
#define FRAMERATE 30.0

// Definições de tipos
typedef SDLKey        KeyboardKey;
typedef Uint8         MouseButton;
typedef unsigned char byte;

#endif
