/* FILE NAME   : input.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 20.01.2023
 * PURPOSE     : Input system functions.
 */

#include "units/units.h"

VOID EK3_KeyboardInit( VOID )
{
  INT i;

  GetKeyboardState(EK3_Anim.Keys);
  for (i = 0; i < 256; i++)
    EK3_Anim.Keys[i] >>= 7;
  memcpy(EK3_Anim.KeysOld, EK3_Anim.Keys, 256);
}

VOID EK3_KeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(EK3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    EK3_Anim.Keys[i] >>= 7;
    EK3_Anim.KeysClick[i] = EK3_Anim.Keys[i] && !EK3_Anim.KeysOld[i];
  }
  memcpy(EK3_Anim.KeysOld, EK3_Anim.Keys, 256);
}

VOID EK3_MouseInit( VOID )
{
  EK3_Anim.Mx = EK3_Anim.My = EK3_Anim.Mz =
    EK3_Anim.Mdx = EK3_Anim.Mdy = EK3_Anim.Mdz = EK3_MouseWheel = 0;
}

VOID EK3_MouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(EK3_Anim.hWnd, &pt);

  EK3_Anim.Mdx = pt.x - EK3_Anim.Mx;
  EK3_Anim.Mdy = pt.y - EK3_Anim.My;
  EK3_Anim.Mdz = EK3_MouseWheel;

  EK3_Anim.Mx = pt.x;
  EK3_Anim.My = pt.y;
  EK3_Anim.Mz += EK3_MouseWheel;
  EK3_MouseWheel = 0;
}

/* END OF 'input.c' FILE */