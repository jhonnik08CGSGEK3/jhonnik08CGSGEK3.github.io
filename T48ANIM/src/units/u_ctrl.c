/* FILE NAME   : units.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 17.01.2023
 * PURPOSE     : Unit examples creation functions.
 */

#include <stdio.h>

#include "units/units.h"

typedef struct tagUNIT_CTRL UNIT_CTRL;
struct tagUNIT_CTRL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamAt;
  FLT Speed, AngleSpeed;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitInit( UNIT_CTRL *Uni, ek3ANIM *Ani )
{
  Uni->CamLoc = VecSet1(8);
  Uni->CamAt = VecSet1(0);
  EK3_RndCamSet(Uni->CamLoc, Uni->CamAt, VecSet(0, 1, 0));
  Uni->Speed = 5;
  Uni->AngleSpeed = -10;
} /* End of 'EK3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitClose( UNIT_CTRL *Uni, ek3ANIM *Ani )
{
} /* End of 'EK3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitResponse( UNIT_CTRL *Uni, ek3ANIM *Ani )
{
  if (Ani->Keys[VK_CONTROL])
  {
    Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamLoc,
      Ani->GlobalDeltaTime * -0.05 * Ani->Mdz));
    Uni->CamLoc = PointTransform(Uni->CamLoc,
      MatrMulMatr(MatrRotateY(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx +
        Uni->Speed * (EK3_Anim.Keys[VK_RIGHT] - EK3_Anim.Keys[VK_LEFT])),
                  MatrRotateX(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdy +
        Uni->Speed * (EK3_Anim.Keys[VK_UP] - EK3_Anim.Keys[VK_DOWN]))));
    /*
    Uni->CamLoc = PointTransform(Uni->CamLoc, MatrTranslate(
      VecSet(Ani->DeltaTime * Uni->Speed * (EK3_Anim.Keys[VK_RIGHT] - EK3_Anim.Keys[VK_LEFT]),
             Ani->DeltaTime * Uni->Speed * (EK3_Anim.Keys[VK_UP] - EK3_Anim.Keys[VK_DOWN]),
             0)));
    Uni->CamAt = PointTransform(Uni->CamAt, MatrTranslate(
      VecSet(Ani->DeltaTime * Uni->Speed * (EK3_Anim.Keys[VK_RIGHT] - EK3_Anim.Keys[VK_LEFT]),
             Ani->DeltaTime * Uni->Speed * (EK3_Anim.Keys[VK_UP] - EK3_Anim.Keys[VK_DOWN]),
             0)));
     */

    EK3_RndCamSet(Uni->CamLoc, Uni->CamAt, VecSet(0, 1, 0));
  }
} /* End of 'EK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitRender( UNIT_CTRL *Uni, ek3ANIM *Ani )
{
  static CHAR Buf[102];

  sprintf(Buf, "FPS: %.5f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'EK3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
ek3UNIT * EK3_UnitCtrlCreate( VOID )
{
  ek3UNIT *Uni;

  /* Memory allocation */
  if ((Uni = malloc(sizeof(UNIT_CTRL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)EK3_UnitInit;
  Uni->Close = (VOID *)EK3_UnitClose;
  Uni->Response = (VOID *)EK3_UnitResponse;
  Uni->Render = (VOID *)EK3_UnitRender;

  return Uni;
} /* End of 'VG4_AnimUnitCreate' function */

/* END OF 'unit.c' FILE */