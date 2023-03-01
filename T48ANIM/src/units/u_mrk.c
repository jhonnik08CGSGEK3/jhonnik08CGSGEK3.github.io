/* FILE NAME   : u_mrk.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 01.03.2023
 * PURPOSE     : Marker unit functions.
 */

#include <time.h>

#include "units/units.h"

typedef struct tagUNIT_MRK UNIT_MRK;
struct tagUNIT_MRK
{
  EK3_UNIT_BASE_FIELDS;

  ek3PRIM Pr;
};

static VOID Init( UNIT_MRK *Uni, ek3ANIM *Ani )
{
  ek3MATERIAL mtl = EK3_RndMtlGetDef();

  mtl.ShdNo = EK3_RndShdAdd("bezie_surface");
  Uni->Pr.MtlNo = EK3_RndMtlAdd(&mtl);
}
static VOID Close( UNIT_MRK *Uni, ek3ANIM *Ani )
{
}
static VOID Render( UNIT_MRK *Uni, ek3ANIM *Ani )
{
}
static VOID Response( UNIT_MRK *Uni, ek3ANIM *Ani )
{
}

ek3UNIT * EK3_UnitMarkerCreate( VOID )
{
  ek3UNIT *Uni;
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_MRK))) == NULL)
    return NULL;
  Uni->Init = (VOID *)Init;
  Uni->Close = (VOID *)Close;
  Uni->Render = (VOID *)Render;
  Uni->Response = (VOID *)Response;
  return Uni;
}

/* END OF 'u_mrk.c' FILE */