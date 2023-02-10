/* FILE NAME: rndbase.c
 * PURPOSE: render base functions.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <windows.h>

#include "anim/rnd/rnd.h"
#include "anim/rnd/res/rndres.h"
#include "anim/anim.h"

/* View and ViewProj matrixes initialization function.
 * ARGUMENTS:
 *  - VEC Loc, At, Up - camera vectors.
 * RETURNS:
 *   None.
 */
VOID EK3_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  EK3_RndMatrView = MatrView(Loc, At, Up);
  EK3_RndMatrVP = MatrMulMatr(EK3_RndMatrView, EK3_RndMatrProj);
  EK3_RndCamLoc = Loc;
} /* End of 'EK3_RndCamSet' function */

/* Change projection size function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndProjSet( VOID )
{
  FLT rx, ry;

  rx = ry = EK3_RndProjSize;

  /* Correct aspect ratio */
  if (EK3_RndFrameW > EK3_RndFrameH)
    rx *= (FLT)EK3_RndFrameW / EK3_RndFrameH;
  else
    ry *= (FLT)EK3_RndFrameH / EK3_RndFrameW;

  EK3_RndMatrProj = MatrFrustrum(-rx / 2, rx / 2, -ry / 2, ry / 2,
                                EK3_RndProjDist, EK3_RndProjFarClip);
  EK3_RndMatrVP = MatrMulMatr(EK3_RndMatrView, EK3_RndMatrProj);
} /* End of 'EK3_RndProjSet' function */

/* Render initialization function.
 * ARGUMENTS:
 *  - HWND hWnd - window handle.
 * RETURNS:
 *   None.
 */
VOID EK3_RndInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  EK3_hRndWnd = hWnd;

  /* Prepare frame compatible device context */
  EK3_hRndDC = GetDC(hWnd);

  /* OpenGL initialization */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(EK3_hRndDC, &pfd);
  DescribePixelFormat(EK3_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(EK3_hRndDC, i, &pfd);

  /* OpenGL init: rendering context setup */
  EK3_hRndGLRC = wglCreateContext(EK3_hRndDC);
  wglMakeCurrent(EK3_hRndDC, EK3_hRndGLRC);

  if (glewInit() != GLEW_OK)
    exit(0);

  /* Render parameters setup */
  glClearColor(0.28, 0.47, 0.8, 1);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  EK3_RndResInit();

  EK3_RndProjDist = EK3_RndProjSize = 0.1;
  EK3_RndProjFarClip = 300;

  EK3_RndFrameW = 30;
  EK3_RndFrameH = 30;
  EK3_RndCamSet(VecSet(13, 25, 18), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'EK3_RndInit' function */

/* Render deinitialization function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndClose( VOID )
{
  EK3_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(EK3_hRndDC);
  ReleaseDC(EK3_hRndWnd, EK3_hRndDC);
} /* End of 'EK3_RndClose' function */

/* Render resize function.
 * ARGUMENTS:
 *   - INT W, H - new screen size.
 * RETURNS:
 *   None.
 */
VOID EK3_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

  /* Saving frame sizes */
  EK3_RndFrameW = W;
  EK3_RndFrameH = H;

  /* Counting projection */
  EK3_RndProjSet();
} /* End of 'EK3_RndResize' function */

/* Render copy frame function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndCopyFrame( VOID )
{
  SwapBuffers(EK3_hRndDC);
} /* End of 'EK3_RndCopyFrame' function */

/* Render start draw function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndStart( VOID )
{
  static DBL ReloadTime = 0;

  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Set defaults */

} /* End of 'EK3_RndStart' function */

/* Render draw end function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndEnd( VOID )
{
  glFinish();
} /* End of 'EK3_RndEnd' function */

/* END OF 'rndbase.c' FILE */