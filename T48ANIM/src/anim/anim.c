/* FILE NAME   : anim.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 10.02.2023
 * PURPOSE     : Animation functions.
 */

#include "anim.h"

ek3ANIM EK3_Anim; /* Global animation struct */

/* Animation initalization function.
 * ARGUMENTS:
 *   - HWND hWnd - window handle.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimInit( HWND hWnd )
{
  memset(&EK3_Anim, 0, sizeof(ek3ANIM));

  EK3_Anim.hWnd = hWnd;
  EK3_RndInit(hWnd);
  EK3_Anim.hDC = EK3_hRndDC;
  EK3_Anim.W = EK3_RndFrameW, EK3_Anim.H = EK3_RndFrameH;
  EK3_Anim.NumOfUnits = 0;

  EK3_AnimTimerInit();
  EK3_AnimInputInit();
} /* End of 'EK3_AnimInit' function */

/* Animation deinitalization function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < EK3_Anim.NumOfUnits; i++)
  {
    EK3_Anim.Units[i]->Close(EK3_Anim.Units[i], &EK3_Anim);
    free(EK3_Anim.Units[i]);
    EK3_Anim.Units[i] = NULL;
  }
  EK3_Anim.NumOfUnits = 0;
  EK3_RndClose();
  memset(&EK3_Anim, 0, sizeof(ek3ANIM));
} /* End of 'EK3_AnimClose' function */

/* Animation resize function.
 * ARGUMENTS:
 *   INT W, H - new screen sizes.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimResize( INT W, INT H )
{
  EK3_Anim.W = W;
  EK3_Anim.H = H;
  EK3_RndResize(W, H);

  EK3_AnimRender();
} /* End of 'EK3_AnimResize' function */

/* Animation copy frame function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimCopyFrame( VOID )
{
  EK3_RndCopyFrame();
} /* End of 'EK3_AnimCopyFrame' function */

/* Animation drawing function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimRender( VOID )
{
  INT i;

  EK3_AnimTimerResponse();
  EK3_AnimInputResponse();

  for (i = 0; i < EK3_Anim.NumOfUnits; i++)
    EK3_Anim.Units[i]->Response(EK3_Anim.Units[i], &EK3_Anim);

  EK3_RndStart();
  for (i = 0; i < EK3_Anim.NumOfUnits; i++)
    EK3_Anim.Units[i]->Render(EK3_Anim.Units[i], &EK3_Anim);
  EK3_RndEnd();
} /* End of 'EK3_AnimRender' function */

/* Animation unit add function.
 * ARGUMENTS:
 *   ek3UNIT *Uni - new unit.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimUnitAdd( ek3UNIT *Uni )
{
  if (EK3_Anim.NumOfUnits < EK3_MAX_UNITS)
    EK3_Anim.Units[EK3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &EK3_Anim);
} /* End of 'EK3_AnimUnitAdd' function */

/* Animation user input initalization function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimInputInit( VOID )
{
  EK3_KeyboardInit();
  EK3_MouseInit();
} /* End of 'EK3_AnimInputInit' function */

/* Animation user input deinitalization function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimInputResponse( VOID )
{
  EK3_KeyboardResponse();
  EK3_MouseResponse();
} /* End of 'EK3_AnimInputResponse' function */

/* Flip full screen function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID EK3_AnimFlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE; /* store current mode */
  static RECT SaveRC;               /* save old window size */

  if (!IsFullScreen)
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    IsFullScreen = TRUE;

    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRC);

    /* Get closest monitor */
    hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_NOTOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
  }

} /* End of 'EK3_AnimFlipFullScreen' function */

/* END OF 'anim.c' FILE */