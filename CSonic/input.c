#include "input.h"

void input_initstate(inputstate* state)
{
	state->hat.x = state->hat.y    = 0;
	state->a = state->b = state->c = 0x0;
	state->start = state->select   = 0x0;
}

void input_copystate(inputstate* dest, inputstate src)
{
	dest->hat.x  = src.hat.x;
	dest->hat.y  = src.hat.y;
	dest->a      = src.a;
	dest->b      = src.b;
	dest->c      = src.c;
	dest->start  = src.start;
	dest->select = src.select;
}

bool input_ispressing(inputstate* state, inputbutton button)
{
	switch(button)
	{
	case BUTTON_UP:
		return (state->hat.y == 1);
	case BUTTON_DOWN:
		return (state->hat.y == -1);
	case BUTTON_LEFT:
		return (state->hat.x == -1);
	case BUTTON_RIGHT:
		return (state->hat.x == 1);
	case BUTTON_A:
		return (state->a);
	case BUTTON_B:
		return (state->b);
	case BUTTON_C:
		return (state->c);
	case BUTTON_START:
		return (state->start);
	case BUTTON_SELECT:
		return (state->select);
	// Mouse buttons
	case BUTTON_MOUSE_LEFT:
		return (state->mouse_left);
	case BUTTON_MOUSE_MIDDLE:
		return (state->mouse_middle);
	case BUTTON_MOUSE_RIGHT:
		return (state->mouse_right);
	}
	return false;
}

bool input_haspressed(inputstate* state, inputstate* oldstate, inputbutton button)
{
	switch(button)
	{
	case BUTTON_UP:
		return (state->hat.y == 1 && oldstate->hat.y == 0);
	case BUTTON_DOWN:
		return (state->hat.y == -1 && oldstate->hat.y == 0);
	case BUTTON_LEFT:
		return (state->hat.x == -1 && oldstate->hat.x == 0);
	case BUTTON_RIGHT:
		return (state->hat.x == 1 && oldstate->hat.x == 0);
	case BUTTON_A:
		return (state->a && !oldstate->a);
	case BUTTON_B:
		return (state->b && !oldstate->b);
	case BUTTON_C:
		return (state->c && !oldstate->c);
	case BUTTON_START:
		return (state->start && !oldstate->start);
	case BUTTON_SELECT:
		return (state->select && !oldstate->select);
	// Mouse buttons
	case BUTTON_MOUSE_LEFT:
		return (state->mouse_left && !oldstate->mouse_left);
	case BUTTON_MOUSE_MIDDLE:
		return (state->mouse_middle && !oldstate->mouse_middle);
	case BUTTON_MOUSE_RIGHT:
		return (state->mouse_right && !oldstate->mouse_right);
	}
	return false;
}