#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include "vec2.h"

typedef enum INPUTBUTTON
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_C,
	BUTTON_START,
	BUTTON_SELECT,
	BUTTON_MOUSE_LEFT,
	BUTTON_MOUSE_MIDDLE,
	BUTTON_MOUSE_RIGHT
} inputbutton;

typedef struct INPUTSTATE_DEF
{
	vec2_b hat;
	vec2 mousepos;

	byte a;
	byte b;
	byte c;
	byte start;
	byte select;
	byte mouse_left;
	byte mouse_middle;
	byte mouse_right;
} inputstate;

// Initializes an input state.
void input_initstate(inputstate*);

// Copies an input state to another.
void input_copystate(inputstate*, inputstate);

bool input_ispressing(inputstate*, inputbutton);
bool input_haspressed(inputstate*, inputstate*, inputbutton);

#endif