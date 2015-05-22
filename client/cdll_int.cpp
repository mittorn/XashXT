/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  cdll_int.c
//
// this implementation handles the linking of the engine to the DLL
//



#include "hud.h"
#include "utils.h"
#include "r_local.h"
#include <mathlib.h>
#include "usercmd.h"
#include "entity_state.h"
#include "cdll_exp.h"



int developer_level;
int g_iXashEngineBuildNumber;
cl_enginefunc_t gEngfuncs;


render_api_t gRenderfuncs;
CHud gHUD;

void Game_HookEvents( void );

/*
==========================
    Initialize

Called when the DLL is first loaded.
==========================
*/


extern "C"
{
int		DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
int		DLLEXPORT HUD_VidInit( void );
void	DLLEXPORT HUD_Init( void );
int		DLLEXPORT HUD_Redraw( float flTime, int intermission );
int		DLLEXPORT HUD_UpdateClientData( client_data_t *cdata, float flTime );
void	DLLEXPORT HUD_Reset ( void );
void	DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server );
void	DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove );
char	DLLEXPORT HUD_PlayerMoveTexture( char *name );
int		DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
int		DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs );
void	DLLEXPORT HUD_Frame( double time );
void	DLLEXPORT HUD_VoiceStatus(int entindex, qboolean bTalking);
void	DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf );

//nicknekit: xashmod exports (for client working without "void DLLEXPORT F"). Commented funcs declared from other places
void	DLLEXPORT HUD_Shutdown( void );
//void	DLLEXPORT IN_ActivateMouse( void );
//void	DLLEXPORT IN_DeactivateMouse( void );
//void	DLLEXPORT IN_MouseEvent( int mstate );
//void	DLLEXPORT IN_ClearStates( void );
//void	DLLEXPORT IN_Accumulate( void );
//void	DLLEXPORT CL_CreateMove( float frametime, usercmd_t *cmd, int active );
//int 	DLLEXPORT CL_IsThirdPerson( void );
//void	DLLEXPORT CL_CameraOffset( float *ofs );
//void	DLLEXPORT KB_Find( const char *name );
//void	DLLEXPORT CAM_Think( void );
//void	DLLEXPORT V_CalcRefdef( struct ref_params_s *pparams );
//int 	DLLEXPORT HUD_AddEntity( int type, struct cl_entity_s *ent, const char *modelname );
//void	DLLEXPORT HUD_CreateEntities( void );
void	DLLEXPORT HUD_DrawNormalTriangles( void );
void	DLLEXPORT HUD_DrawTransparentTriangles( void );
//void	DLLEXPORT HUD_StudioEvent( const struct mstudioevent_s *event, const struct cl_entity_s *entity );
void	DLLEXPORT HUD_PostRunCmd( struct local_state_s*, local_state_s *, struct usercmd_s*, int, double, unsigned int );
//void	DLLEXPORT HUD_TxferLocalOverrides( struct entity_state_s *state, const struct clientdata_s *client );
//void	DLLEXPORT HUD_ProcessPlayerState( struct entity_state_s *dst, const struct entity_state_s *src );
//void	DLLEXPORT HUD_TxferPredictionData( entity_state_t *ps, const entity_state_t *pps, clientdata_t *pcd, const clientdata_t *ppcd, weapon_data_t *wd, const weapon_data_t *pwd );
void	DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer );

int 	DLLEXPORT HUD_Key_Event( int eventcode, int keynum, const char *pszCurrentBinding );
/*void	DLLEXPORT HUD_TempEntUpdate(
        double frametime,   // Simulation time
        double client_time, // Absolute time on client
        double cl_gravity,  // True gravity on client
        TEMPENTITY **ppTempEntFree,   // List of freed temporary ents
        TEMPENTITY **ppTempEntActive, // List
        int  (*Callback_AddVisibleEntity)( cl_entity_t *pEntity ),
        void (*Callback_TempEntPlaySound)( TEMPENTITY *pTemp, float damp )); */
//cl_entity_t	DLLEXPORT HUD_GetUserEntity( int index );
//int 	DLLEXPORT HUD_GetStudioModelInterface( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio );
//int 	DLLEXPORT HUD_GetRenderInterface( int version, render_api_t *renderfuncs, render_interface_t *callback );

}




/*
================================
HUD_GetHullBounds

Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int HUD_GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	int iret = 0;

	switch( hullnumber )
	{
	case 0:	// Normal player
		Vector( -16, -16, -36 ).CopyToArray( mins );
		Vector(  16,  16,  36 ).CopyToArray( maxs );
		iret = 1;
		break;
	case 1:	// Crouched player
		Vector( -16, -16, -18 ).CopyToArray( mins );
		Vector(  16,  16,  18 ).CopyToArray( maxs );
		iret = 1;
		break;
	case 2:	// Point based hull
		g_vecZero.CopyToArray( mins );
		g_vecZero.CopyToArray( maxs );
		iret = 1;
		break;
	}
	return iret;
}

/*
================================
HUD_ConnectionlessPacket

Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.
Incoming, it holds the max size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int HUD_ConnectionlessPacket( const struct netadr_s *, const char *, char *, int *response_buffer_size )
{
	// Parse stuff from args
	int max_buffer_size = *response_buffer_size;

	// Zero it out since we aren't going to respond.
	// If we wanted to response, we'd write data into response_buffer
	*response_buffer_size = 0;

	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead.
	return 0;
}

void HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
	PM_Init( ppmove );
}

char HUD_PlayerMoveTexture( char *name )
{
	return PM_FindTextureType( name );
}

void HUD_PlayerMove( struct playermove_s *ppmove, int server )
{
	PM_Move( ppmove, server );
}

int Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	gEngfuncs = *pEnginefuncs;

	if( iVersion != CLDLL_INTERFACE_VERSION )
		return 0;

	memcpy( &gEngfuncs, pEnginefuncs, sizeof( cl_enginefunc_t ));

	// get developer level
	developer_level = (int)CVAR_GET_FLOAT( "developer" );

	if( !CVAR_GET_POINTER( "host_clientloaded" ))
		return 0;	// Not a Xash3D engine

	g_iXashEngineBuildNumber = (int)CVAR_GET_FLOAT( "build" ); // 0 for old builds or GoldSrc

    Game_HookEvents();

	return 1;
}

/*
==========================
	HUD_VidInit

Called when the game initializes
and whenever the vid_mode is changed
so the HUD can reinitialize itself.
==========================
*/
int HUD_VidInit( void )
{
	gHUD.VidInit();

	return 1;
}

/*
==========================
	HUD_Init

Called whenever the client connects
to a server.  Reinitializes all
the hud variables.
==========================
*/
void HUD_Init( void )
{
	InitInput();
	gHUD.Init();

}

void HUD_Shutdown( void )
{
	ShutdownInput();
}

/*
==========================
	HUD_Redraw

called every screen frame to
redraw the HUD.
===========================
*/
int HUD_Redraw( float time, int intermission )
{
	return gHUD.Redraw( time, intermission );
}

/*
==========================
	HUD_UpdateClientData

called every time shared client
dll/engine data gets changed,
and gives the cdll a chance
to modify the data.

returns 1 if anything has been changed, 0 otherwise.
==========================
*/
void IN_Commands();
int HUD_UpdateClientData( client_data_t *pcldata, float flTime )
{
	IN_Commands();
	return gHUD.UpdateClientData( pcldata, flTime );
}

/*
==========================
	HUD_Reset

Called at start and end of demos to restore to "non"HUD state.
==========================
*/
void HUD_Reset( void )
{
	gHUD.VidInit();
}



/*
==========================
HUD_Frame

Called by engine every frame that client .dll is loaded
==========================
*/
void HUD_Frame( double time )
{

}

int HUD_Key_Event( int eventcode, int keynum, const char *pszCurrentBinding )
{
	return 1;
}

void HUD_PostRunCmd( struct local_state_s*, local_state_s *, struct usercmd_s*, int, double, unsigned int )
{
}

void HUD_VoiceStatus( int entindex, qboolean bTalking )
{
}

void HUD_DirectorMessage( int iSize, void *pbuf )
{
}

void HUD_DrawNormalTriangles( void )
{
}

void HUD_DrawTransparentTriangles( void )
{
}

void Demo_ReadBuffer( int size, unsigned char *buffer )
{
}

extern "C" cl_entity_t *HUD_GetUserEntity( int index )
{
	return NULL;
}

cldll_func_t cldll_func =
{
    Initialize,
    HUD_Init,
    HUD_VidInit,
    HUD_Redraw,
    HUD_UpdateClientData,
    HUD_Reset,
    HUD_PlayerMove,
    HUD_PlayerMoveInit,
    HUD_PlayerMoveTexture,
    IN_ActivateMouse,
    IN_DeactivateMouse,
    IN_MouseEvent,
    IN_ClearStates,
    IN_Accumulate,
    CL_CreateMove,
    CL_IsThirdPerson,
    CL_CameraOffset,
    KB_Find,
    CAM_Think,
    V_CalcRefdef,
    HUD_AddEntity,
    HUD_CreateEntities,
    HUD_DrawNormalTriangles,
    HUD_DrawTransparentTriangles,
    HUD_StudioEvent,
    HUD_PostRunCmd,
    HUD_Shutdown,
    HUD_TxferLocalOverrides,
    HUD_ProcessPlayerState,
    HUD_TxferPredictionData,
    Demo_ReadBuffer,
    HUD_ConnectionlessPacket,
    HUD_GetHullBounds,
    HUD_Frame,
    HUD_Key_Event,
    HUD_TempEntUpdate,
    HUD_GetUserEntity,
    HUD_VoiceStatus,
    HUD_DirectorMessage,
    HUD_GetStudioModelInterface,
    NULL,	// HUD_ChatInputPosition,
    NULL,	// HUD_GetPlayerTeam
    NULL,	// HUD_GetClientFactory
    HUD_GetRenderInterface,
    NULL,	// HUD_ClipMoveToEntity
};

#ifdef _WIN32 //nicknekit: thats not working in gcc for some reason, need some macro magic
extern "C" void DLLEXPORT F( void *pv )
{
    cldll_func_t *pcldll_func = (cldll_func_t *)pv;
    *pcldll_func = cldll_func;
}
#endif
