/* FILE NAME   : units.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 17.01.2023
 * PURPOSE     : Unit examples creation functions.
 */

#include "anim/anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitInit( ek3UNIT *Uni, ek3ANIM *Ani )
{
} /* End of 'EK3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitClose( ek3UNIT *Uni, ek3ANIM *Ani )
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
static VOID EK3_UnitResponse( ek3UNIT *Uni, ek3ANIM *Ani )
{
} /* End of 'EK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ek3UNIT *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitRender( ek3UNIT *Uni, ek3ANIM *Ani )
{
} /* End of 'EK3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ek3UNIT *) pointer to created unit.
 */
ek3UNIT * EK3_AnimUnitCreate( INT Size )
{
  ek3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ek3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = EK3_UnitInit;
  Uni->Close = EK3_UnitClose;
  Uni->Response = EK3_UnitResponse;
  Uni->Render = EK3_UnitRender;

  return Uni;
} /* End of 'EK3_AnimUnitCreate' function */

/* END OF 'unit.c' FILE */