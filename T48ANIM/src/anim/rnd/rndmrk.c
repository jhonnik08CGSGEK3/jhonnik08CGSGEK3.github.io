/* FILE NAME: rndmrk.c
 * PURPOSE: Marker render handle module.
 * PROGRAMMER: EK3
 * DATE: 01.03.2023
 */

#include "rnd.h"

/*** Marker semiglobal data ***/
static ek3PRIM
  EK3_RndMarkerPrimSphere,   /* Sphere marker primitive */
  EK3_RndMarkerPrimCylinder; /* Cylinder marker primitive */

/* Marker system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndMarkerInit( VOID )
{
  ek3MATERIAL mtl = EK3_RndMtlGetDef();

  EK3_RndPrimCreate(&EK3_RndMarkerPrimSphere, EK3_RND_PRIM_PATCH, NULL, 1, NULL, 0);
  EK3_RndMarkerPrimSphere.NumOfPatchPoints = 1;
  strncpy(mtl.Name, "Marker sphere material", EK3_STR_MAX);
  mtl.ShdNo = EK3_RndShdAdd("marker/sphere");
  EK3_RndMarkerPrimSphere.MtlNo = EK3_RndMtlAdd(&mtl);

  EK3_RndPrimCreate(&EK3_RndMarkerPrimCylinder, EK3_RND_PRIM_PATCH, NULL, 1, NULL, 0);
  EK3_RndMarkerPrimCylinder.NumOfPatchPoints = 1;
  strncpy(mtl.Name, "Marker ñylinder material", EK3_STR_MAX);
  mtl.ShdNo = EK3_RndShdAdd("marker/cylinder");
  EK3_RndMarkerPrimCylinder.MtlNo = EK3_RndMtlAdd(&mtl);
} /* End of 'EK3_RndMarkerInit' function */

/* Marker system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndMarkerClose( VOID )
{
  EK3_RndPrimFree(&EK3_RndMarkerPrimSphere);
  EK3_RndPrimFree(&EK3_RndMarkerPrimCylinder);
} /* End of 'EK3_RndMarkerClose' function */

/* Draw cylinder function.
 * ARGUMENTS:
 *   - cylinder start point:
 *       VEC P0;
 *   - cylinder start point radius:
 *       FLT R0;
 *   - cylinder end point:
 *       VEC P1;
 *   - cylinder end point radius:
 *       FLT R1;
 *   - cylinder color:
 *       VEC Color;
 *   - transform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
VOID EK3_RndMrkDrawCylinder( VEC P0, FLT R0, VEC P1, FLT R1, VEC Color, MATR Transform )
{
  EK3_RndShdAddonF[0] = R0;
  EK3_RndShdAddonF[1] = R1;
  EK3_RndShdAddonV[0] = P0;
  EK3_RndShdAddonV[1] = P1;
  EK3_RndShdAddonV[2] = Color;
  EK3_RndPrimDraw(&EK3_RndMarkerPrimCylinder, Transform);
} /* End of 'EK3_RndMrkDrawCylinder' function */

/* Draw sphere function.
 * ARGUMENTS:
 *   - sphere center point:
 *       VEC C;
 *   - sphere (ellipsoid) radiuses:
 *       VEC R;
 *   - sphere color:
 *       VEC Color;
 *   - sphere transform matrix:
 *       MATR Transform;
* RETURNS: None.
 */
VOID EK3_RndMrkDrawSphere( VEC C, VEC R, VEC Color, MATR Transform )
{
  EK3_RndShdAddonV[0] = C;
  EK3_RndShdAddonV[1] = R;
  EK3_RndShdAddonV[2] = Color;
  EK3_RndPrimDraw(&EK3_RndMarkerPrimSphere, Transform);
} /* End of 'EK3_RndMrkDrawSphere' function */

/* END OF 'rndmrk.c' FILE */