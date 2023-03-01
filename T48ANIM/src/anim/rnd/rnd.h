/* FILE NAME: rnd.h
 * PURPOSE: Parameters for rendering and function headers.
 * PROGRAMMER: EK3
 * DATE: 27.02.2023
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32")

#include "res/rndres.h"

typedef struct tagek3VERTEX
{
  VEC P;  /* Position */
  VEC2 T; /* Texture coordinate */
  VEC N;  /* Normal */
  VEC4 C; /* Color (r, g, b, a) */
} ek3VERTEX;

typedef enum tagek3PRIM_TYPE
{
  EK3_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  EK3_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  EK3_RND_PRIM_LINES,    /* Line segments (by 2 points) */
  EK3_RND_PRIM_POINTS,   /* Array of points */
  EK3_RND_PRIM_PATCH,    /* ??? */
} ek3PRIM_TYPE;

typedef struct tagek3PRIM
{
  ek3PRIM_TYPE Type;
  INT
    VA,               /* Vertex array Id */
    VBuf,             /* Vertex buffer Id */
    IBuf;             /* Index buffer Id */
  INT NumOfElements,  /* Number of indices/vertices */
    NumOfPatchPoints; /* Number of patches */

  VEC MinBB, MaxBB;   /* Bound box corner coordinates */

  MATR Trans;         /* Additional transformation matrix */
  INT MtlNo;
} ek3PRIM;

/* Primitive collection data type */
typedef struct tagek3PRIMS
{
  INT NumOfPrims;   /* Number of primitives in array */
  ek3PRIM *Prims;   /* Array of primitives */
  MATR Trans;       /* Common transformation matrix */
  VEC MinBB, MaxBB; /* Bound box corner coordinates */
} ek3PRIMS;

/* Grid topology representation type */
typedef struct tagek3GRID
{
  INT W, H;      /* Grid size (in vertices) */
  ek3VERTEX *V;  /* Array (2D) of vertex */
} ek3GRID;

extern HWND EK3_hRndWnd;                 /* Work window handle */
extern HDC EK3_hRndDC;                   /* Work window memory device context */
extern HGLRC EK3_hRndGLRC;                 /* OpenGL rendering context handle */
extern INT EK3_RndFrameW, EK3_RndFrameH; /* Work window size */


extern FLT
  EK3_RndProjSize,    /* Project plane fit square */
  EK3_RndProjDist,    /* Distance to project plane from viewer (near) */
  EK3_RndProjFarClip; /* Distance to project far clip plane (far) */

extern VEC EK3_RndCamLoc;

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

BOOL EK3_RndPrimCreate( ek3PRIM *Pr, ek3PRIM_TYPE Type,
                        ek3VERTEX *V, INT NoofV, INT *I, INT NoofI );
VOID EK3_RndPrimFree( ek3PRIM *Pr );
VOID EK3_RndPrimDraw( ek3PRIM *Pr, MATR World );
VOID EK3_RndPrimCreateSphere( ek3PRIM *Pr, FLT R, INT W, INT H );
BOOL EK3_RndPrimLoad( ek3PRIM *Pr, CHAR *FileName );

/***
 * Primitives handle functions
 ***/

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL EK3_RndPrimsCreate( ek3PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 * RETURNS: None.
 */
VOID EK3_RndPrimsFree( ek3PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID EK3_RndPrimsDraw( ek3PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL EK3_RndPrimsLoad( ek3PRIMS *Prs, CHAR *FileName );

BOOL EK3_RndGridCreate( ek3GRID *G, INT W, INT H, ek3VERTEX *V );
VOID EK3_RndGridFree( ek3GRID *G );
VOID EK3_RndGridAutoNormals( ek3VERTEX *V, INT W, INT H );
VOID EK3_RndPrimFromGrid( ek3PRIM *Pr, ek3GRID *G );

VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message, const VOID *UserParam );

/***
 * Target handle functions
 ***/

/* Render target initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetInit( VOID );

/* Render target deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetClose( VOID );

/* Render target resize function.
 * ARGUMENTS:
 *   - new render target size (in pixels):
 *       INT W, H;
 * RETURNS: None.
 */
VOID EK3_RndTargetResize( INT W, INT H );

/* Start render target usage function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetStart( VOID );

/* Finalize render target output function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetEnd( VOID );

/***
 * Marker handle functions
 ***/

/* Marker system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndMarkerInit( VOID );

/* Marker system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndMarkerClose( VOID );

/* Draw cylinder function.
 * ARGUMENTS:
 *   - cylinder start point:
 *       VEC P0;
 *   - cylinder start point radius:
 *       FLT R0;
 *   - cylinder end point:
 *       VEC P1;
 *   - cylinder end point radius:
 *       FLT R1;
 *   - cylinder color:
 *       VEC Color;
 *   - stransform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
VOID EK3_RndMrkDrawCylinder( VEC P0, FLT R0, VEC P1, FLT R1, VEC Color, MATR Transform );

/* Draw sphere function.
 * ARGUMENTS:
 *   - sphere center point:
 *       VEC C;
 *   - sphere (ellipsoid) radiuses:
 *       VEC R;
 *   - cylinder color:
 *       VEC Color;
 *   - stransform matrix:
 *       MATR Transform;
* RETURNS: None.
 */
VOID EK3_RndMrkDrawSphere( VEC C, VEC R, VEC Color, MATR Transform );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */