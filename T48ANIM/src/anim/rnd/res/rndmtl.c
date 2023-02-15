/* FILE NAME: rndmtl.c
 * PURPOSE: Material functions.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include "anim/anim.h"

ek3MATERIAL EK3_RndMaterials[EK3_MAX_MATERIALS]; /* Array of materials */
INT EK3_RndMaterialsSize;

/* Default material creation function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   (ek3MATERIAL) - default material struct.
 */
ek3MATERIAL EK3_RndMtlGetDef( VOID )
{
  ek3MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.9, 0.9, 0.9},
    {0.3, 0.3, 0.3},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  return def_mtl;
} /* End of 'EK3_RndMtlGetDef' function */

/* Material applying function.
 * ARGUMENTS:
 *  - MtlNo - Material array position.
 * RETURNS:
 *   (UINT) - programm ID.
 */
UINT EK3_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  ek3MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= EK3_RndMaterialsSize)
    MtlNo = 0;
  mtl = &EK3_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= EK3_RndShadersSize)
    prg = 0;
  prg = EK3_RndShaders[prg].ProgId;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, EK3_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, EK3_Anim.GlobalTime);

  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, EK3_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'EK3_RndMtlApply' function */

/* Material initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndMtlInit( VOID )
{
  ek3MATERIAL mtl;

  mtl = EK3_RndMtlGetDef();

  EK3_RndMaterialsSize = 0;
  EK3_RndMtlAdd(mtl);
} /* End of 'EK3_RndMtlInit' function */

/* Material initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndMtlClose( VOID )
{
  EK3_RndMaterialsSize = 0;
} /* End of 'EK3_RndMtlClose' function */

/* Material adding in array function.
 * ARGUMENTS:
 *  - ek3MATERIAL Mtl - material struct to add in array.
 * RETURNS:
 *   (INT) - array of materials size.
 */
INT EK3_RndMtlAdd( ek3MATERIAL Mtl )
{
  if (EK3_RndMaterialsSize >= EK3_MAX_MATERIALS)
    return 0;

  EK3_RndMaterials[EK3_RndMaterialsSize] = Mtl;

  return EK3_RndMaterialsSize++;
} /* End of 'EK3_RndMtlAdd' function */

/* END OF 'rndmtl.c' FILE */