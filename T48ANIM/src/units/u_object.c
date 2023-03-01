/* FILE NAME   : u_object.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 22.02.2023
 * PURPOSE     : Obj unit functions.
 */

#include <time.h>

#include "anim/rnd/rnd.h"
#include "units/units.h"

typedef struct tagUNIT_OBJECT UNIT_OBJECT;
struct tagUNIT_OBJECT
{
  EK3_UNIT_BASE_FIELDS;
  VEC Pos;
  ek3PRIMS Obj;
};

static VOID Init( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 0);
  EK3_RndPrimsLoad(&Uni->Obj, "bin/objects/banana.g3dm");
}
static VOID Close( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  EK3_RndPrimsFree(&Uni->Obj);
}
static VOID Render( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  MATR m = MatrTranslate(Uni->Pos);

  m = MatrMulMatr3(MatrTranslate(Uni->Pos),
                   MatrRotateY(140 * clock() / 1000.0),
                   MatrScale(VecSet1(10)));

  EK3_RndPrimsDraw(&Uni->Obj, m);
}
static VOID Response( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
}

ek3UNIT * EK3_UnitObjectCreate( VOID )
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

/* END OF 'u_object.c' FILE */