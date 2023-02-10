 /* FILE NAME: rndres.c
 * PURPOSE: Resources functions.
 * PROGRAMMER: EK3
 * DATE: 06.02.2023
 */

#include "anim/rnd/res/rndres.h"

/* Resources initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndResInit( VOID )
{
  EK3_RndShdInit();
  EK3_RndTexInit();
  EK3_RndMtlInit();
} /* End of 'EK3_RndResInit' function */

/* Resources deinitialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndResClose( VOID )
{
  EK3_RndShdClose();
  EK3_RndTexClose();
  EK3_RndMtlClose();
} /* End of 'EK3_RndResClose' function */

/* END OF 'rndres.c' FILE */