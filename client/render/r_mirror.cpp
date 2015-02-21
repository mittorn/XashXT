/*
r_mirror.cpp - draw reflected surfaces
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

/*
================
R_BeginDrawMirror

Setup texture matrix for mirror texture
================
*/
void R_BeginDrawMirror( msurface_t *fa )
{

}

/*
================
R_EndDrawMirror

Restore identity texmatrix
================
*/
void R_EndDrawMirror( void )
{

}

/*
=============================================================

	MIRROR RENDERING

=============================================================
*/
/*
================
R_PlaneForMirror

Get transformed mirrorplane and entity matrix
================
*/
void R_PlaneForMirror( msurface_t *surf, mplane_t &out, matrix4x4 &m )
{

}

/*
================
R_AllocateMirrorTexture

Allocate the screen texture and make copy
================
*/
int R_AllocateMirrorTexture( bool copyToImage = true )
{

}

/*
================
R_DrawMirrors

Draw all viewpasess from mirror position
Mirror textures will be drawn in normal pass
================
*/
void R_DrawMirrors( cl_entity_t *ignoreent )
{

}

/*
================
R_RecursiveMirrorNode
================
*/
void R_RecursiveMirrorNode( mnode_t *node, unsigned int clipflags, int &numMirrors )
{

}

/*
=================
R_FindBmodelMirrors

Check all bmodel surfaces and make personal mirror chain
=================
*/
void R_FindBmodelMirrors( cl_entity_t *e, bool static_entity, int &numMirrors )
{

}

/*
=================
R_CheckMirrorEntitiesOnList

Check all bmodels for mirror surfaces
=================
*/
void R_CheckMirrorEntitiesOnList( int &numMirrors )
{

}

/*
================
R_FindMirrors

Build mirror chains for this frame
================
*/
bool R_FindMirrors( const ref_params_t *fd )
{

}
