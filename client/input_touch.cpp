/*
touchscreen.c - touchscreen support prototype
Copyright (C) 2015 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "port.h"

#include "input.h"

typedef enum
{
	touch_tap,		// Just tap a button
	touch_slider,	// Slider. Maybe isn't need because here is touch_stick
	touch_stick		// Like a joystick stick.
} touchButtonType;

typedef struct touchbutton_s
{
	// Field of button in pixels
	float left, right, top, bottom;
	bool lookAvailable;
	// Touch button type: tap, stick or slider
	touchButtonType buttonType;
	// Callback for a button
	void (*touchEvent)(void);
	// Drawing callback
	void (*drawButton)(void);
} touchbutton_t;

void IN_TouchJump(void)
{
	IN_JumpUp();
	IN_JumpDown();
}

void IN_TouchDuck(void)
{
	static bool ducked = true;
	if(!ducked)
	{
		IN_DuckDown();
		ducked = true;
	}
	else
	{
		IN_DuckUp();
		ducked = false;
	}
}


touchbutton_t buttons[] =
{
	{0.8, 1.0, 0.4, 0.6, true, touch_tap, IN_TouchJump, 0},
	{0.8, 1.0, 0.6, 0.8, true, touch_tap, IN_TouchDuck, 0},
}; // array of buttons

extern "C" int IN_ClientTouchEvent( bool down, int fingerID, float x, float y, float dx, float dy )
{
	// Find suitable button
	/*for( int i = 0; i < sizeof( buttons ); i++ )
	{
		if( ( x > buttons[i].left &&
			  x < buttons[i].right ) &&
			( y > buttons[i].bottom &&
			  y < buttons[i].top ) )
		{
			buttons[i].touchEvent();
			break;
		}
	}*/
	return 0;
}
