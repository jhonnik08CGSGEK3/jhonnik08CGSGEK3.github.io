/* FILE NAME   : u_cow.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 18.01.2023
 * PURPOSE     : Cow unit functions.
 */

#include <time.h>

#include "units/units.h"

typedef struct tagUNIT_COW UNIT_COW;
struct tagUNIT_COW
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  ek3PRIM Cow;
};

static VOID Init( UNIT_COW *Uni, ek3ANIM *Ani )
{
  Uni->Pos = VecSet(0, -8, 3);
  EK3_RndPrimLoad(&Uni->Cow, "bin/objects/cow.obj");
}
static VOID Close( UNIT_COW *Uni, ek3ANIM *Ani )
{
  EK3_RndPrimFree(&Uni->Cow);
}
static VOID Render( UNIT_COW *Uni, ek3ANIM *Ani )
{
  MATR m = MatrIdentity();

  m = MatrMulMatr4(MatrTranslate(Uni->Pos),
                  MatrRotateY(-120 * clock() / 1000.0),
                  MatrRotateX(90 * clock() / 1000.0),
                  MatrRotateZ(30 * clock() / 1000.0));

  EK3_RndPrimDraw(&Uni->Cow, m);
}
static VOID Response( UNIT_COW *Uni, ek3ANIM *Ani )
{
}

ek3UNIT * EK3_UnitCowCreate( VOID )
{
  ek3UNIT *Uni;
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_COW))) == NULL)
    return NULL;
  Uni->Init = (VOID *)Init;
  Uni->Close = (VOID *)Close;
  Uni->Render = (VOID *)Render;
  Uni->Response = (VOID *)Response;
  return Uni;
}

/*
UNIT_CONTROL:
  Init:
    RndCamSet(uni->CamLoc = VecSet1(8), VecSet1(0), VecSet(0, 1, 0))

  Response:
    if (ani->keys[vk_control])
    {
      uni->camloc = vecaddvec(uni->camloc, vecmulnum(vecnormalize()))
    }
*/