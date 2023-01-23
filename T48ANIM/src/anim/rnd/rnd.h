/* FILE NAME: rnd.h
 * PURPOSE: Parameters for rendering and function headers.
 * PROGRAMMER: EK3
 * DATE: 18.01.2023
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>
#include <gl/wglext.h>

/*
#include <gl/gl.h>
#include <gl/glu.h>
*/

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#pragma comment(lib, "glew32s")

#include "def.h"

typedef struct tagek3VERTEX
{
  VEC P;  /* Vertex position */
} ek3VERTEX;

typedef struct tagek3PRIM
{
  ek3VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} ek3PRIM;

extern HWND EK3_hRndWnd;                 /* Work window handle */
extern HDC EK3_hRndDC;                   /* Work window memory device context */
extern HDC EK3_hRndGLRC;                 /* OpenGL rendering context handle */
extern INT EK3_RndFrameW, EK3_RndFrameH; /* Work window size */


extern FLT
  EK3_RndProjSize,    /* Project plane fit square */
  EK3_RndProjDist,    /* Distance to project plane from viewer (near) */
  EK3_RndProjFarClip; /* Distance to project far clip plane (far) */

extern MATR
  EK3_RndMatrView, /* View coordinate system matrix */
  EK3_RndMatrProj, /* Projection coordinate system matrix */
  EK3_RndMatrVP;   /* Stored (View * Proj) matrix */

VOID EK3_RndCamSet( VEC Loc, VEC At, VEC Up );

VOID EK3_RndInit( HWND hWnd );
VOID EK3_RndClose( VOID );

VOID EK3_RndResize( INT W, INT H );
VOID EK3_RndCopyFrame( VOID );

VOID EK3_RndStart( VOID );
VOID EK3_RndEnd( VOID );

BOOL EK3_RndPrimCreate( ek3PRIM *Pr, INT NoofV, INT NoofI );
VOID EK3_RndPrimFree( ek3PRIM *Pr );
VOID EK3_RndPrimDraw( ek3PRIM *Pr, MATR World );
VOID EK3_RndPrimCreateSphere( ek3PRIM *Pr, FLT R, INT W, INT H );
BOOL EK3_RndPrimLoad( ek3PRIM *Pr, CHAR *FileName );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */