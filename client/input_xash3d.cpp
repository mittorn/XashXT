/*
 * Xash3D interface input code
 */
#include "hud.h"
#include "utils.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "kbutton.h"
#include "input.h"
#include "r_view.h"
#include "keydefs.h"
#include <mathlib.h>

extern "C"  void DLLEXPORT IN_ClientMoveEvent( float forwardmove, float sidemove );
extern "C" void DLLEXPORT IN_ClientLookEvent( float relyaw, float relpitch );

cvar_t		*sensitivity;

// Client symbols
extern kbutton_t	in_strafe;
extern kbutton_t	in_mlook;
extern kbutton_t	in_speed;
extern kbutton_t	in_jlook;

extern cvar_t	*m_pitch;
extern cvar_t	*m_yaw;
extern cvar_t	*m_forward;
extern cvar_t	*m_side;
extern cvar_t	*lookstrafe;
extern cvar_t	*lookspring;
extern cvar_t	*cl_pitchdown;
extern cvar_t	*cl_pitchup;
extern cvar_t	*cl_yawspeed;
extern cvar_t	*cl_sidespeed;
extern cvar_t	*cl_forwardspeed;
extern cvar_t	*cl_pitchspeed;
extern cvar_t	*cl_movespeedkey;

float ac_forwardmove;
float ac_sidemove;
float rel_yaw;
float rel_pitch;

#define F 1<<0	// Forward
#define B 1<<1	// Back
#define L 1<<2	// Left
#define R 1<<3	// Right
#define T 1<<4	// Forward stop
#define S 1<<5	// Side stop

void IN_ClientMoveEvent( float forwardmove, float sidemove )
{
	ac_forwardmove += forwardmove;
	ac_sidemove += sidemove;
}

void IN_ClientLookEvent( float relyaw, float relpitch )
{
	rel_yaw += relyaw;
	rel_pitch += relpitch;
}
// Rotate camera and add move values to usercmd
void IN_Move( float frametime, usercmd_t *cmd )
{
	static uint moveflags = T | S;
	Vector viewangles;
	gEngfuncs.GetViewAngles( viewangles );

	if( in_mlook.state & 1 )
	{
		V_StopPitchDrift();
	}
	if( !gHUD.m_iIntermission )
	{
		if( gHUD.GetSensitivity() != 0 )
		{
			rel_yaw *= gHUD.GetSensitivity();
			rel_pitch *= gHUD.GetSensitivity();
		}
		else
		{
			rel_yaw *= sensitivity->value;
			rel_pitch *= sensitivity->value;
		}

		viewangles[YAW] += rel_yaw;

		viewangles[PITCH] += rel_pitch;
		viewangles[PITCH] = bound( -cl_pitchup->value, viewangles[PITCH], cl_pitchdown->value );
	}
	float rgfl[3];
	viewangles.CopyToArray( rgfl );
	gEngfuncs.SetViewAngles( rgfl );
		
	if( ac_forwardmove ) cmd->forwardmove  = ac_forwardmove * cl_forwardspeed->value;
	if( ac_sidemove ) cmd->sidemove  = ac_sidemove * cl_sidespeed->value;
	
	if( ac_forwardmove )
		moveflags &= ~T;
	else if( !( moveflags & T ) )
	{
		IN_ForwardUp();
		IN_BackUp();
		moveflags |= T;
	}
	if( ac_sidemove )
		moveflags &= ~S;
	else if( !( moveflags & S ) )
	{
		IN_MoverightUp();
		IN_MoveleftUp();
		moveflags |= S;
	}

	if ( ac_forwardmove > 0.7 && !( moveflags & F ))
	{
		moveflags |= F;
		IN_ForwardDown();
	}
	else if ( ac_forwardmove < 0.7 && ( moveflags & F ))
	{
		moveflags &= ~F;
		IN_ForwardUp();
	}
	if ( ac_forwardmove < -0.7 && !( moveflags & B ))
	{
		moveflags |= B;
		IN_BackDown();
	}
	else if ( ac_forwardmove > -0.7 && ( moveflags & B ))
	{
		moveflags &= ~B;
		IN_BackUp();
	}
	if ( ac_sidemove > 0.9 && !( moveflags & R ))
	{
		moveflags |= R;
		IN_MoverightDown();
	}
	else if ( ac_sidemove < 0.9 && ( moveflags & R ))
	{
		moveflags &= ~R;
		IN_MoverightUp();
	}
	if ( ac_sidemove < -0.9 && !( moveflags & L ))
	{
		moveflags |= L;
		IN_MoveleftDown();
	}
	else if ( ac_sidemove > -0.9 && ( moveflags & L ))
	{
		moveflags &= ~L;
		IN_MoveleftUp();
	}

	ac_sidemove = ac_forwardmove = rel_pitch = rel_yaw = 0;	
}

void IN_MouseEvent( int mstate )
{
	static int mouse_oldbuttonstate;
	// perform button actions
	for( int i = 0; i < 5; i++ )
	{
		if(( mstate & BIT( i )) && !( mouse_oldbuttonstate & BIT( i )))
		{
			gEngfuncs.Key_Event( K_MOUSE1 + i, 1 );
		}

		if( !( mstate & BIT( i )) && ( mouse_oldbuttonstate & BIT( i )))
		{
			gEngfuncs.Key_Event( K_MOUSE1 + i, 0 );
		}
	}	
	
	mouse_oldbuttonstate = mstate;
}

// Stubs

void IN_ClearStates ( void )
{
}

void IN_ActivateMouse ( void )
{
}

void IN_DeactivateMouse ( void )
{
}

void IN_Accumulate ( void )
{
}

void IN_Commands ( void )
{
}

void IN_Shutdown ( void )
{
}

// Register cvars and reset data
void IN_Init( void )
{
	sensitivity	= CVAR_REGISTER ( "sensitivity","3", FCVAR_ARCHIVE );
	ac_forwardmove = ac_sidemove = rel_yaw = rel_pitch = 0;
}
