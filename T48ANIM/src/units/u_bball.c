/* FILE NAME   : u_heli.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 20.01.2023
 * PURPOSE     : Cow unit functions.
 */

#include <time.h>

#include "units/units.h"

typedef struct tagUNIT_OBJECT UNIT_OBJECT;
struct tagUNIT_OBJECT
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  ek3PRIM Object;
};

static VOID Init( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  Uni->Pos = VecSet(5, 2, 2);
  EK3_RndPrimLoad(&Uni->Object, "bin/objects/helicopter.obj");
}
static VOID Close( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  EK3_RndPrimFree(&Uni->Object);
}
static VOID Render( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  MATR m = MatrIdentity();

  m = MatrMulMatr3(MatrTranslate(Uni->Pos),
                   MatrRotateY(140 * clock() / 1000.0),
                   MatrScale(VecSet(3, 3, 3)));

  EK3_RndPrimDraw(&Uni->Object, m);
}
static VOID Response( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
}

ek3UNIT * EK3_UnitHeliCreate( VOID )
{
  ek3UNIT *Uni;
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_OBJECT))) == NULL)
    return NULL;
  Uni->Init = (VOID *)Init;
  Uni->Close = (VOID *)Close;
  Uni->Render = (VOID *)Render;
  Uni->Response = (VOID *)Response;
  return Uni;
}

/* END OF 'u_bball.c' FILE */