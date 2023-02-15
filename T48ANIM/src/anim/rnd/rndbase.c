/* FILE NAME: rndbase.c
 * PURPOSE: Render base functions.
 * PROGRAMMER: EK3
 * DATE: 13.02.2023
 */

#include <stdio.h>
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

  /* #ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
    0, NULL, GL_TRUE);
  #endif */ /* NDEBUG */
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
  wglDeleteContext(EK3_hRndGLRC);
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
  if (EK3_Anim.GlobalTime - ReloadTime > 3)
  {
    ReloadTime = EK3_Anim.GlobalTime;
    EK3_RndShdUpdate();
  }
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

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *      UINT Source;
 *   - error type:
 *      UINT Type;
 *   - error message id:
 *      UINT Id, 
 *   - message severity:
 *      UINT severity, 
 *   - message text length:
 *      INT Length, 
 *   - message text:
 *      CHAR *Message, 
 *   - user addon parameters pointer:
 *      VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message, const VOID *UserParam )
{
  INT len = 0;
  static CHAR Buf[10000];

  /* Ignore non-significant error/warning codes */
  if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
    return;
  len += sprintf(Buf + len, "Debug message (%i) %s\n", Id, Message);

  switch (Source)
  {
  case GL_DEBUG_SOURCE_API:
    len += sprintf(Buf + len, "Source: API\n");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    len += sprintf(Buf + len, "Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    len += sprintf(Buf + len, "Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    len += sprintf(Buf + len, "Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    len += sprintf(Buf + len, "Source: Application");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    len += sprintf(Buf + len, "Source: Other");
    break;
  }
  len += sprintf(Buf + len, "\n");

  switch (Type)
  {
  case GL_DEBUG_TYPE_ERROR:
    len += sprintf(Buf + len, "Type: Error");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    len += sprintf(Buf + len, "Type: Deprecated Behaviour");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    len += sprintf(Buf + len, "Type: Undefined Behaviour");
    break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    len += sprintf(Buf + len, "Type: Portability");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    len += sprintf(Buf + len, "Type: Performance");
    break;
  case GL_DEBUG_TYPE_MARKER:
    len += sprintf(Buf + len, "Type: Marker");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    len += sprintf(Buf + len, "Type: Push Group");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    len += sprintf(Buf + len, "Type: Pop Group");
    break;
  case GL_DEBUG_TYPE_OTHER:
    len += sprintf(Buf + len, "Type: Other");
    break;
  }
  len += sprintf(Buf + len, "\n");

  switch (Severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    len += sprintf(Buf + len, "Severity: high");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    len += sprintf(Buf + len, "Severity: medium");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    len += sprintf(Buf + len, "Severity: low");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    len += sprintf(Buf + len, "Severity: notification");
    break;
  }
  len += sprintf(Buf + len, "\n\n");

  OutputDebugString(Buf);
} /* End of 'glDebugOutput' function */

/* END OF 'rndbase.c' FILE */