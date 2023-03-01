/* FILE NAME   : units.h
 * PROGRAMMER  : EK3
 * LAST UPDATE : 17.01.2023
 * PURPOSE     : Unit examples creation functions headers.
 */

#ifndef __units_h_
#define __units_h_

#include "anim/anim.h"

ek3UNIT * EK3_AnimUnitCreate( INT Size );
ek3UNIT * EK3_UnitCowCreate( VOID );
ek3UNIT * EK3_UnitCtrlCreate( VOID );
ek3UNIT * EK3_UnitObjectCreate( VOID );
ek3UNIT * EK3_UnitGridCreate( VOID );
ek3UNIT * EK3_UnitMarkerCreate( VOID );

#endif /* __units_h_ */

/* END OF 'units.h' FILE */
