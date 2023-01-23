/* FILE NAME   : anim.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 17.01.2023
 * PURPOSE     : Animation functions. (yippee)
 */

#include "anim.h"

ek3ANIM EK3_Anim;

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
}

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
}

VOID EK3_AnimResize( INT W, INT H )
{
  EK3_Anim.W = W;
  EK3_Anim.H = H;
  EK3_RndResize(W, H);

  EK3_AnimRender();
}

VOID EK3_AnimCopyFrame( VOID )
{
  EK3_RndCopyFrame();
}

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
}

VOID EK3_AnimUnitAdd( ek3UNIT *Uni )
{
  if (EK3_Anim.NumOfUnits < EK3_MAX_UNITS)
    EK3_Anim.Units[EK3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &EK3_Anim);
}

VOID EK3_AnimInputInit( VOID )
{
  EK3_KeyboardInit();
  EK3_MouseInit();
}

VOID EK3_AnimInputResponse( VOID )
{
  EK3_KeyboardResponse();
  EK3_MouseResponse();
}

/* END OF 'anim.c' FILE */