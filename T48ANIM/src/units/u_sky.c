/* FILE NAME   : u_land.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 22.02.2023
 * PURPOSE     : Unit examples creation functions.
 */

#include "units/units.h"

typedef struct tagUNIT_SKY UNIT_SKY;
struct tagUNIT_SKY
{
  EK3_UNIT_BASE_FIELDS;
  VEC Pos;
  ek3PRIM Pr;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_SKY *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitInit( UNIT_SKY *Uni, ek3ANIM *Ani )
{
  EK3_RndShdAdd("marker_sphere");
} /* End of 'EK3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_SKY *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitClose( UNIT_SKY *Uni, ek3ANIM *Ani )
{
  //EK3_RndPrimFree(&Uni->Pr);
} /* End of 'EK3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_SKY *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitResponse( UNIT_SKY *Uni, ek3ANIM *Ani )
{
  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['W'])
  {
    INT modes[2];

    glGetIntegerv(GL_POLYGON_MODE, modes);
    if (modes[0] == GL_LINE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
} /* End of 'EK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_SKY *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitRender( UNIT_SKY *Uni, ek3ANIM *Ani )
{
} /* End of 'EK3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (UNIT_SKY *) pointer to created unit.
 */
ek3UNIT * EK3_UnitSkyCreate( VOID )
{
  ek3UNIT *Uni;

  /* Memory allocation */
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_SKY))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)EK3_UnitInit;
  Uni->Close = (VOID *)EK3_UnitClose;
  Uni->Render = (VOID *)EK3_UnitRender;
  Uni->Response = (VOID *)EK3_UnitResponse;

  return Uni;
} /* End of 'EK3_AnimUnitCreate' function */

/* END OF 'u_land.c' FILE */