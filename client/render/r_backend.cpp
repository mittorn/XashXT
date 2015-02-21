//=======================================================================
//			Copyright (C) XashXT Group 2006
//=======================================================================

#include "hud.h"
#include "utils.h"
#include "r_local.h"
#include "r_sprite.h"
#include "mathlib.h"

glState_t	glState;
glConfig_t glConfig;

/*
=============
R_GetSpriteTexture

=============
*/
int R_GetSpriteTexture( const model_t *m_pSpriteModel, int frame )
{
	if( !m_pSpriteModel || m_pSpriteModel->type != mod_sprite || !m_pSpriteModel->cache.data )
		return 0;

	return R_GetSpriteFrame( m_pSpriteModel, frame )->gl_texturenum;
}

/*
=============
TriSpriteTexture

bind current texture
=============
*/
int TriSpriteTexture( model_t *pSpriteModel, int frame )
{
	int	gl_texturenum;
	msprite_t	*psprite;

	if(( gl_texturenum = R_GetSpriteTexture( pSpriteModel, frame )) == 0 )
		return 0;

	psprite = (msprite_t *)pSpriteModel->cache.data;
	if( psprite->texFormat == SPR_ALPHTEST )
	{
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.0f );
	}

	GL_Bind( GL_TEXTURE0, gl_texturenum );

	return 1;
}

/*
==============
GL_DisableAllTexGens
==============
*/
void GL_DisableAllTexGens( void )
{
	GL_TexGen( GL_S, 0 );
	GL_TexGen( GL_T, 0 );
	GL_TexGen( GL_R, 0 );
	GL_TexGen( GL_Q, 0 );
}

/*
=================
GL_Cull
=================
*/
void GL_Cull( GLenum cull )
{
	if( !cull )
	{
        glDisable( GL_CULL_FACE );
		glState.faceCull = 0;
		return;
	}

    glEnable( GL_CULL_FACE );
    glCullFace( cull );
	glState.faceCull = cull;
}

/*
=================
GL_FrontFace
=================
*/
void GL_FrontFace( GLenum front )
{
    glFrontFace( front ? GL_CW : GL_CCW );
	glState.frontFace = front;
}

void GL_SetRenderMode( int mode )
{
	switch( mode )
	{
	case kRenderNormal:
	default:
        glDisable( GL_BLEND );
        glDisable( GL_ALPHA_TEST );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		break;
	case kRenderTransColor:
        glEnable( GL_BLEND );
        glDisable( GL_ALPHA_TEST );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderTransAlpha:
        glDisable( GL_BLEND );
        glEnable( GL_ALPHA_TEST );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderTransTexture:
        glEnable( GL_BLEND );
        glDisable( GL_ALPHA_TEST );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderGlow:
        glEnable( GL_BLEND );
        glDisable( GL_ALPHA_TEST );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	case kRenderTransAdd:
        glEnable( GL_BLEND );
        glDisable( GL_ALPHA_TEST );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		break;
	}
}

/*
=================
GL_LoadTexMatrix
=================
*/
void GL_LoadTexMatrix( const matrix4x4 source )
{
	GLfloat	dest[16];

	source.CopyToArray( dest );
	GL_LoadTextureMatrix( dest );
}

/*
=================
GL_LoadMatrix
=================
*/
void GL_LoadMatrix( const matrix4x4 source )
{
	GLfloat	dest[16];

	source.CopyToArray( dest );
    glLoadMatrixf( dest );
}

/*
================
R_BeginDrawProjection

Setup texture matrix for light texture
================
*/
void R_BeginDrawProjection( const plight_t *pl, bool decalPass )
{
    #ifndef __ANDROID__
	GLfloat	genVector[4][4];

	RI.currentlight = pl;
    glEnable( GL_BLEND );

	if( glState.drawTrans )
	{
		// particle lighting
        glDepthFunc( GL_LEQUAL );
        glBlendFunc( GL_SRC_COLOR, GL_SRC_ALPHA );
        glColor4ub( pl->color.r*0.3, pl->color.g*0.3, pl->color.b*0.3, 255 );
	}
	else
	{
        glBlendFunc( GL_ONE, GL_ONE );

		if( R_OVERBRIGHT_SILENT() && !decalPass )
            glColor4ub( pl->color.r / 2.0f, pl->color.g / 2.0f, pl->color.b / 2.0f, 255 );
		else
            glColor4ub( pl->color.r, pl->color.g, pl->color.b, 255 );

        if( decalPass ) glDepthFunc( GL_LEQUAL );
        else glDepthFunc( GL_EQUAL );
	}

	GL_Bind( GL_TEXTURE0, pl->projectionTexture );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	for( int i = 0; i < 4; i++ )
	{
		genVector[0][i] = i == 0 ? 1 : 0;
		genVector[1][i] = i == 1 ? 1 : 0;
		genVector[2][i] = i == 2 ? 1 : 0;
		genVector[3][i] = i == 3 ? 1 : 0;
	}

	GL_TexGen( GL_S, GL_OBJECT_LINEAR );
	GL_TexGen( GL_T, GL_OBJECT_LINEAR );
	GL_TexGen( GL_R, GL_OBJECT_LINEAR );
	GL_TexGen( GL_Q, GL_OBJECT_LINEAR );

    glTexGenfv( GL_S, GL_OBJECT_PLANE, genVector[0] );
    glTexGenfv( GL_T, GL_OBJECT_PLANE, genVector[1] );
    glTexGenfv( GL_R, GL_OBJECT_PLANE, genVector[2] );
    glTexGenfv( GL_Q, GL_OBJECT_PLANE, genVector[3] );

	if( tr.modelviewIdentity )
		GL_LoadTexMatrix( pl->textureMatrix );
	else GL_LoadTexMatrix( pl->textureMatrix2 );

	glState.drawProjection = true;

	// setup attenuation texture
	if( pl->attenuationTexture != 0 )
	{
		GL_Bind( GL_TEXTURE1, pl->attenuationTexture );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		if( pl->pointlight )
		{
			float r = 1.0f / (pl->radius * 2);
			Vector origin;

			if( !tr.modelviewIdentity )
			{
				// rotate attenuation texture into local space
				if( RI.currententity->angles != g_vecZero )
					origin = RI.objectMatrix.VectorITransform( pl->origin );
				else origin = pl->origin - RI.currententity->origin;
			}
			else origin = pl->origin;

			GLfloat planeS[] = { r, 0, 0, -origin[0] * r + 0.5 };
			GLfloat planeT[] = { 0, r, 0, -origin[1] * r + 0.5 };
			GLfloat planeR[] = { 0, 0, r, -origin[2] * r + 0.5 };

			GL_TexGen( GL_S, GL_EYE_LINEAR );
			GL_TexGen( GL_T, GL_EYE_LINEAR );
			GL_TexGen( GL_R, GL_EYE_LINEAR );

            glTexGenfv( GL_S, GL_EYE_PLANE, planeS );
            glTexGenfv( GL_T, GL_EYE_PLANE, planeT );
            glTexGenfv( GL_R, GL_EYE_PLANE, planeR );
                    }
                    else
                    {
			GLfloat	genPlaneS[4];
			Vector	origin, normal;

			if( !tr.modelviewIdentity )
			{
				if( RI.currententity->angles != g_vecZero )
				{
					// rotate attenuation texture into local space
					normal = RI.objectMatrix.VectorIRotate( pl->frustum[5].normal );
					origin = RI.objectMatrix.VectorITransform( pl->origin );
				}
				else
				{
					normal = pl->frustum[5].normal;
					origin = pl->origin - RI.currententity->origin;
				}
			}
			else
			{
				normal = pl->frustum[5].normal;
				origin = pl->origin;
			}
			genPlaneS[0] = normal[0] / pl->radius;
			genPlaneS[1] = normal[1] / pl->radius;
			genPlaneS[2] = normal[2] / pl->radius;
			genPlaneS[3] = -(DotProduct( normal, origin ) / pl->radius);

			GL_TexGen( GL_S, GL_OBJECT_LINEAR );
            glTexGenfv( GL_S, GL_OBJECT_PLANE, genPlaneS );
		}

		GL_LoadIdentityTexMatrix();
	}
	else
	{
		// can't draw shadows without attenuation texture
		return;
	}

	if( decalPass )
	{
		if( cg.decal0_shader && ( r_shadows->value <= 0.0f || pl->pointlight || FBitSet( pl->flags, CF_NOSHADOWS )))
		{
            glEnable( GL_FRAGMENT_PROGRAM_ARB );
            glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, (pl->pointlight) ? cg.decal3_shader : cg.decal0_shader );
		}
		else if( r_shadows->value == 1.0f && cg.decal1_shader )
		{
            glEnable( GL_FRAGMENT_PROGRAM_ARB );
            glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, cg.decal1_shader );
		} 
		else if( r_shadows->value > 1.0f && cg.decal2_shader )
		{
            glEnable( GL_FRAGMENT_PROGRAM_ARB );
            glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, cg.decal2_shader );
		}
	}
	else if( r_shadows->value > 1.0f && cg.shadow_shader && !pl->pointlight && !FBitSet( pl->flags, CF_NOSHADOWS ))
	{
        glEnable( GL_FRAGMENT_PROGRAM_ARB );
        glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, cg.shadow_shader );
	}

	// TODO: allow shadows for pointlights
	if( r_shadows->value <= 0.0f || pl->pointlight || FBitSet( pl->flags, CF_NOSHADOWS ))
		return;		

	GL_Bind( GL_TEXTURE2, pl->shadowTexture );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	GL_TexGen( GL_S, GL_EYE_LINEAR );
	GL_TexGen( GL_T, GL_EYE_LINEAR );
	GL_TexGen( GL_R, GL_EYE_LINEAR );
	GL_TexGen( GL_Q, GL_EYE_LINEAR );

    glTexGenfv( GL_S, GL_EYE_PLANE, genVector[0] );
    glTexGenfv( GL_T, GL_EYE_PLANE, genVector[1] );
    glTexGenfv( GL_R, GL_EYE_PLANE, genVector[2] );
    glTexGenfv( GL_Q, GL_EYE_PLANE, genVector[3] );

	if( tr.modelviewIdentity )
		GL_LoadTexMatrix( pl->shadowMatrix );
	else GL_LoadTexMatrix( pl->shadowMatrix2 );
#endif
}

/*
================
R_BeginDrawProjection

Setup texture matrix for fog texture
================
*/
qboolean R_SetupFogProjection( void )
{
    #ifndef __ANDROID__
	Vector	origin, vieworg;
	float	r;

	if( !RI.fogEnabled && !RI.fogCustom )
		return false;

	if( !tr.fogTexture2D || !tr.fogTexture1D )
		return false;

	if( RI.params & RP_SHADOWVIEW )
		return false;
	else if( RI.params & RP_MIRRORVIEW )
		vieworg = r_lastRefdef.vieworg;
	else vieworg = RI.vieworg;

	if( !tr.modelviewIdentity )
	{
		// rotate attenuation texture into local space
		if( RI.currententity->angles != g_vecZero )
			origin = RI.objectMatrix.VectorITransform( vieworg );
		else origin = vieworg - RI.currententity->origin;
	}
	else origin = vieworg;

	if( RI.fogCustom )
		r = 1.0f / (( RI.fogStart + RI.fogEnd ) * 2.0f );
	else
		r = 1.0f / (( 6.0f / RI.fogDensity ) * 2.0f );

	GLfloat planeS[] = { r, 0, 0, -origin[0] * r + 0.5 };
	GLfloat planeT[] = { 0, r, 0, -origin[1] * r + 0.5 };
	GLfloat planeR[] = { 0, 0, r, -origin[2] * r + 0.5 };

    glEnable( GL_BLEND );
    glColor3fv( RI.fogColor );
    glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA );

	GL_Bind( GL_TEXTURE0, tr.fogTexture2D );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	GL_TexGen( GL_S, GL_EYE_LINEAR );
	GL_TexGen( GL_T, GL_EYE_LINEAR );
    glTexGenfv( GL_S, GL_EYE_PLANE, planeS );
    glTexGenfv( GL_T, GL_EYE_PLANE, planeT );

	GL_Bind( GL_TEXTURE1, tr.fogTexture1D );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	GL_TexGen( GL_S, GL_EYE_LINEAR );
    glTexGenfv( GL_S, GL_EYE_PLANE, planeR );
#endif
	return true;
}

/*
================
R_EndDrawProjection

Restore identity texmatrix
================
*/
void R_EndDrawProjection( void )
{
	if( GL_Support( R_FRAGMENT_PROGRAM_EXT ))
        glDisable( GL_FRAGMENT_PROGRAM_ARB );

	GL_CleanUpTextureUnits( 0 );

    glMatrixMode( GL_MODELVIEW );
	glState.drawProjection = false;
    glColor4ub( 255, 255, 255, 255 );

    glDepthFunc( GL_LEQUAL );
    glDisable( GL_BLEND );
	RI.currentlight = NULL;
}

int R_AllocFrameBuffer( void )
{

    return -1; //buffers not support in glshim
    #ifndef __ANDROID__
	int i = tr.num_framebuffers;

	if( i >= MAX_FRAMEBUFFERS )
	{
		ALERT( at_error, "R_AllocateFrameBuffer: FBO limit exceeded!\n" );
		return -1; // disable
	}

	gl_fbo_t *fbo = &tr.frame_buffers[i];
	tr.num_framebuffers++;

	if( fbo->init )
	{
		ALERT( at_warning, "R_AllocFrameBuffer: buffer %i already initialized\n", i );
		return i;
	}

	int width, height;

	if( GL_Support( R_ARB_TEXTURE_NPOT_EXT ))
	{
		// allow screen size
		width = bound( 96, glState.width, 1024 );
		height = bound( 72, glState.height, 768 );
	}
	else
	{
		width = NearestPOW( glState.width, true );
		height = NearestPOW( glState.height, true );
		width = bound( 128, width, 1024 );
		height = bound( 64, height, 512 );
	}


	// create a depth-buffer
    glGenRenderbuffers( 1, &fbo->renderbuffer );
    glBindRenderbuffer( GL_RENDERBUFFER_EXT, fbo->renderbuffer );
    glRenderbufferStorage( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height );
    glBindRenderbuffer( GL_RENDERBUFFER_EXT, 0 );

	// create frame-buffer
    glGenFramebuffers( 1, &fbo->framebuffer );
    glBindFramebuffer( GL_FRAMEBUFFER_EXT, fbo->framebuffer );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo->renderbuffer );
    glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
    glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
    glBindFramebuffer( GL_FRAMEBUFFER_EXT, 0 );
	fbo->init = true;


	return i;
#endif
}

void R_FreeFrameBuffer( int buffer )
{
    #ifndef __ANDROID__
	if( buffer < 0 || buffer >= MAX_FRAMEBUFFERS )
	{
		ALERT( at_error, "R_FreeFrameBuffer: invalid buffer enum %i\n", buffer );
		return;
	}

	gl_fbo_t *fbo = &tr.frame_buffers[buffer];

    glDeleteRenderbuffers( 1, &fbo->renderbuffer );
    glDeleteFramebuffers( 1, &fbo->framebuffer );
	memset( fbo, 0, sizeof( *fbo ));
#endif
}

void GL_BindFrameBuffer( int buffer, int texture )
{
    #ifndef __ANDROID__
	gl_fbo_t *fbo = NULL;

	if( buffer >= 0 && buffer < MAX_FRAMEBUFFERS )
		fbo = &tr.frame_buffers[buffer];

	if( !fbo )
	{
        glBindFramebuffer( GL_FRAMEBUFFER_EXT, 0 );
		glState.frameBuffer = buffer;
		return;
	}

	if( glState.frameBuffer != buffer )
	{
        glBindFramebuffer( GL_FRAMEBUFFER_EXT, fbo->framebuffer );
		glState.frameBuffer = fbo->framebuffer;
	}		

	if( fbo->texture != texture )
	{
		// change texture attachment
		GLuint texnum = RENDER_GET_PARM( PARM_TEX_TEXNUM, texture );
        glFramebufferTexture2D( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texnum, 0 );
		fbo->texture = texture;
	}
#endif
}

void R_SetupOverbright( qboolean active )
{
	if( R_OVERBRIGHT() )
	{
		if( active )
		{
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
            glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
            glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB );
            glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE );
            glTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2 );
		}
		else
		{
            glTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1 );
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		}
	}
}
