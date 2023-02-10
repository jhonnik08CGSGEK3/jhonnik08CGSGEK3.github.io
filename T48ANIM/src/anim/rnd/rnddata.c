/* FILE NAME: rnddata.c
 * PURPOSE: Variables data.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <windows.h>

#include "def.h"
#include "anim/rnd/rnd.h"

HWND EK3_hRndWnd;                 /* Work window handle */
HDC EK3_hRndDC;                   /* Work window memory device context */
HDC EK3_hRndGLRC;                 /* OpenGL rendering context handle */
INT EK3_RndFrameW, EK3_RndFrameH; /* Work window size */

FLT
  EK3_RndProjSize = 0.1,    /* Project plane fit square */
  EK3_RndProjDist = 0.1,    /* Distance to project plane from viewer (near) */
  EK3_RndProjFarClip = 300; /* Distance to project far clip plane (far) */

VEC EK3_RndCamLoc ;

MATR
  EK3_RndMatrView, /* View coordinate system matrix */
  EK3_RndMatrProj, /* Projection coordinate system matrix */
  EK3_RndMatrVP;   /* Stored (View * Proj) matrix */

/* END OF 'rnddata.c' FILE */