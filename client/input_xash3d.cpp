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
extern kbutton_t	in_forward;
extern kbutton_t	in_back;
extern kbutton_t	in_moveleft;
extern kbutton_t	in_moveright;

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
cvar_t	*cl_laddermode;


float ac_forwardmove;
float ac_sidemove;
int ac_movecount;
float rel_yaw;
float rel_pitch;

#define F 1<<0	// Forward
#define B 1<<1	// Back
#define L 1<<2	// Left
#define R 1<<3	// Right
#define T 1<<4	// Forward stop
#define S 1<<5	// Side stop

#define BUTTON_DOWN		1
#define IMPULSE_DOWN	2
#define IMPULSE_UP		4


void IN_ToggleButtons( float forwardmove, float sidemove )
{
	static uint moveflags = T | S;

	if( forwardmove )
		moveflags &= ~T;
	else
	{
		//if( in_forward.state || in_back.state ) gEngfuncs.Con_Printf("Buttons pressed f%d b%d\n", in_forward.state, in_back.state);
		if( !( moveflags & T ) )
		{
			//IN_ForwardUp();
			//IN_BackUp();
			//gEngfuncs.Con_Printf("Reset forwardmove state f%d b%d\n", in_forward.state, in_back.state);
			in_forward.state &= ~BUTTON_DOWN;
			in_back.state &= ~BUTTON_DOWN;
			moveflags |= T;
		}
	}
	if( sidemove )
		moveflags &= ~S;
	else
	{
		//gEngfuncs.Con_Printf("l%d r%d\n", in_moveleft.state, in_moveright.state);
		//if( in_moveleft.state || in_moveright.state ) gEngfuncs.Con_Printf("Buttons pressed l%d r%d\n", in_moveleft.state, in_moveright.state);
		if( !( moveflags & S ) )
		{
			//IN_MoverightUp();
			//IN_MoveleftUp();
			//gEngfuncs.Con_Printf("Reset sidemove state f%d b%d\n", in_moveleft.state, in_moveright.state);
			in_moveleft.state &= ~BUTTON_DOWN;
			in_moveright.state &= ~BUTTON_DOWN;
			moveflags |= S;
		}
	}

	if ( forwardmove > 0.7 && !( moveflags & F ))
	{
		moveflags |= F;
		in_forward.state |= BUTTON_DOWN;
	}
	if ( forwardmove < 0.7 && ( moveflags & F ))
	{
		moveflags &= ~F;
		in_forward.state &= ~BUTTON_DOWN;
	}
	if ( forwardmove < -0.7 && !( moveflags & B ))
	{
		moveflags |= B;
		in_back.state |= BUTTON_DOWN;
	}
	if ( forwardmove > -0.7 && ( moveflags & B ))
	{
		moveflags &= ~B;
		in_back.state &= ~BUTTON_DOWN;
	}
	if ( sidemove > 0.9 && !( moveflags & R ))
	{
		moveflags |= R;
		in_moveright.state |= BUTTON_DOWN;
	}
	if ( sidemove < 0.9 && ( moveflags & R ))
	{
		moveflags &= ~R;
		in_moveright.state &= ~BUTTON_DOWN;
	}
	if ( sidemove < -0.9 && !( moveflags & L ))
	{
		moveflags |= L;
		in_moveleft.state |= BUTTON_DOWN;
	}
	if ( sidemove > -0.9 && ( moveflags & L ))
	{
		moveflags &= ~L;
		in_moveleft.state &= ~BUTTON_DOWN;
	}

}

void IN_ClientMoveEvent( float forwardmove, float sidemove )
{
	//gEngfuncs.Con_Printf("IN_MoveEvent\n");

	ac_forwardmove += forwardmove;
	ac_sidemove += sidemove;
	ac_movecount++;
}

void IN_ClientLookEvent( float relyaw, float relpitch )
{
	rel_yaw += relyaw;
	rel_pitch += relpitch;
}
// Rotate camera and add move values to usercmd
void IN_Move( float frametime, usercmd_t *cmd )
{
	Vector viewangles;
	gEngfuncs.GetViewAngles( viewangles );
	bool fLadder = false;
	if( cl_laddermode->value !=2 ) fLadder = gEngfuncs.GetLocalPlayer()->curstate.movetype == MOVETYPE_FLY;
	//if(ac_forwardmove || ac_sidemove)
	//gEngfuncs.Con_Printf("Move: %f %f %f %f\n", ac_forwardmove, ac_sidemove, rel_pitch, rel_yaw);

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
		if( fLadder )
		{
			if( ( cl_laddermode->value == 1 ) )
				viewangles[YAW] -= ac_sidemove * 5;
			ac_sidemove = 0;
		}

		viewangles[PITCH] += rel_pitch;
		viewangles[PITCH] = bound( -cl_pitchup->value, viewangles[PITCH], cl_pitchdown->value );
	}
	float rgfl[3];
	viewangles.CopyToArray( rgfl );
	gEngfuncs.SetViewAngles( rgfl );
	
	if( ac_movecount )
	{
		IN_ToggleButtons( ac_forwardmove / ac_movecount, ac_sidemove / ac_movecount );
		if( ac_forwardmove ) cmd->forwardmove  = ac_forwardmove * cl_forwardspeed->value / ac_movecount;
		if( ac_sidemove ) cmd->sidemove  = ac_sidemove * cl_sidespeed->value / ac_movecount;
	}
	
	ac_sidemove = ac_forwardmove = rel_pitch = rel_yaw = 0;
	ac_movecount = 0;
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
	//gEngfuncs.Con_Printf("IN_ClearStates\n");
}

void IN_ActivateMouse ( void )
{
	//gEngfuncs.Con_Printf("IN_ActivateMouse\n");
}

void IN_DeactivateMouse ( void )
{
	//gEngfuncs.Con_Printf("IN_DeactivateMouse\n");
}

void IN_Accumulate ( void )
{
	//gEngfuncs.Con_Printf("IN_Accumulate\n");
}

void IN_Commands ( void )
{
	//gEngfuncs.Con_Printf("IN_Commands\n");
}

void IN_Shutdown ( void )
{
}

// Register cvars and reset data
void IN_Init( void )
{
	sensitivity	= CVAR_REGISTER ( "sensitivity", "3", FCVAR_ARCHIVE );
	cl_laddermode = CVAR_REGISTER ( "cl_laddermode", "2", FCVAR_ARCHIVE );
	ac_forwardmove = ac_sidemove = rel_yaw = rel_pitch = 0;
}
