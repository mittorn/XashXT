/*
r_monitor.cpp - draw screen surfaces
Copyright (C) 2011 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "hud.h"
#include "utils.h"
#include "r_local.h"
#include "mathlib.h"
#include "r_view.h"
#include "pm_movevars.h"

/*
================
R_BeginDrawScreen

Setup texture matrix for mirror texture
================
*/
void R_BeginDrawScreen( msurface_t *fa )
{

}

/*
================
R_EndDrawScreen

Restore identity texmatrix
================
*/
void R_EndDrawScreen( void )
{

}

/*
=============================================================

	SCREEN RENDERING

=============================================================
*/
/*
================
R_SetupScreenView

Setup screen view
================
*/
void R_SetupScreenView( cl_entity_t *camera )
{

}

/*
================
R_AllocateScreenTexture

Allocate the screen texture and make copy
================
*/
int R_AllocateScreenTexture( bool copyToImage = true )
{

}

/*
================
R_DrawScreens

Draw all viewpasess from monitor camera position
Screen textures will be drawn in normal pass
================
*/
void R_DrawScreens( cl_entity_t *ignoreent )
{
    return 0;
}

/*
=================
R_FindBmodelScreens

Check all bmodel surfaces and make personal screen chain
=================
*/
void R_FindBmodelScreens( cl_entity_t *e, bool static_entity, int &numScreens )
{

}

/*
=================
R_CheckScreenEntitiesOnList

Check all bmodels for screen surfaces
=================
*/
int R_CheckScreenEntitiesOnList( void )
{

    return 0;
}

/*
================
R_FindScreens

Build screen chains for this frame
================
*/
bool R_FindScreens( const ref_params_t *fd )
{

	return false;
}
