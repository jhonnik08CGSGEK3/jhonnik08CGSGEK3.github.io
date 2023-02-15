/* FILE NAME: rndprim.c
 * PURPOSE: Function with primitives.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <stdio.h>
#include <windows.h>
#include <string.h>

#include "def.h"
#include "anim/rnd/rnd.h"
#include "anim/rnd/res/rndres.h"
#include "anim/anim.h"

/* Primitive normals evaluation function.
 * ARGUMENTS:
 *   - ek3VERTEX *V - vertex array struct.
 *   - INT NoofV - vertex array size.
 *   - INT *I - index array.
 *   - INT NoofI - index array size.
 * RETURNS:
 *   None.
 */
VOID EK3_RndPrimEvalNormals( ek3VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  INT i;

  for (i = 0; i < NoofV; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < NoofI; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
    V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
    V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }

  for (i = 0; i < NoofV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'EK3_RndPrimEvalNormals' function */

/* Primitive creation function.
 * ARGUMENTS:
 *   - ek3PRIM *Pr - primitive struct.
 *   - ek3PRIM_TYPE Type - drawing type.
 *   - ek3VERTEX *V - vertex array struct.
 *   - INT NoofV - vertex array size.
 *   - INT *I - index array.
 *   - INT NoofI - index array size.
 * RETURNS:
 *   (BOOL) - was the program successful or not.
 */
BOOL EK3_RndPrimCreate( ek3PRIM *Pr, ek3PRIM_TYPE Type,
                        ek3VERTEX *V, INT NoofV, INT *I, INT NoofI )
{
  memset(Pr, 0, sizeof(ek3PRIM));

  /* Vertex array and buffer initialization */
  glGenBuffers(1, &Pr->VBuf);
  glGenBuffers(1, &Pr->IBuf);
  glGenVertexArrays(1, &Pr->VA);

  glBindVertexArray(Pr->VA);

  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ek3VERTEX) * NoofV, V, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);

  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ek3VERTEX),
                        (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ek3VERTEX),
                        (VOID *)sizeof(VEC));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ek3VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ek3VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  Pr->NumOfElements = NoofI;
  Pr->Trans = MatrIdentity();
  Pr->Type = Type;
  Pr->MtlNo = 0;

  return TRUE;
} /* End of 'EK3_RndPrimCreate' function */

/* Primitive deletion function.
 * ARGUMENTS:
 *   - ek3PRIM *Pr - primitive struct to delete.
 * RETURNS:
 *   None.
 */
VOID EK3_RndPrimFree( ek3PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glDeleteBuffers(1, &Pr->IBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  memset(Pr, 0, sizeof(ek3PRIM));
} /* End of 'EK3_RndPrimFree' function */

/* Primitive drawing function.
 * ARGUMENTS:
 *   - ek3PRIM *Pr - primitive struct.
 *   - MATR World - world matrix.
 * RETURNS:
 *   None.
 */
VOID EK3_RndPrimDraw( ek3PRIM *Pr, MATR World )
{
  INT gl_prim_type, ProgId, loc;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, EK3_RndMatrVP),
       MatrNormTrans = MatrTranspose(MatrInverse(MatrMulMatr(Pr->Trans, World))),
       winv = MatrInverse(World);

  gl_prim_type = Pr->Type == EK3_RND_PRIM_TRIMESH ? GL_TRIANGLES : GL_TRIANGLE_STRIP;

  ProgId = EK3_RndMtlApply(Pr->MtlNo);
  /* Pass render uniforms */
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, World.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &EK3_RndCamLoc.X);

  /* Build projection */
  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);

  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
} /* End of 'EK3_RndPrimDraw' function */

/* Primitive load from file function.
 * ARGUMENTS:
 *   - ek3PRIM *Pr - primitive struct.
 *   - CHAR *FileName - name file.
 * RETURNS:
 *   (BOOL) - was the programm successful or not.
 */
BOOL EK3_RndPrimLoad( ek3PRIM *Pr, CHAR *FileName )
{
  INT nv, nf, nvold, nfold;
  INT *I;
  ek3VERTEX *V;
  FILE *F;
  CHAR Buf[1000];

  memset(Pr, 0, sizeof(ek3PRIM));
  F = fopen(FileName, "r");
  if (F == NULL)
    return FALSE;

  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  I = malloc(nf * 3 *  sizeof(INT));
  V = malloc(nv * sizeof(ek3VERTEX));
  nvold = nv, nfold = nf;

  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1 = 0, n2 = 0, n3 = 0;
      CHAR
        *f1 = "%d %d %d",
        *f2 = "%d/%*d %d/%*d %d/%*d",
        *f3 = "%d//%*d %d//%*d %d//%*d",
        *f4 = "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d";

      sscanf(Buf + 2, f4, &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, f3, &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, f2, &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, f1, &n1, &n2, &n3) == 3;
      if (n1 > 0)
        n1--;
      else if (n2 < 0)
        n1 = nvold - (-n1);
      if (n2 > 0)
        n2--;
      else if (n3 < 0)
        n2 = nvold - (-n2);
      if (n3 > 0)
        n3--;
      else if (n3 < 0)
        n3 = nvold - (-n3);

      I[nf++] = n1;
      I[nf++] = n2;
      I[nf++] = n3;
    }

  if (!EK3_RndPrimCreate(Pr, EK3_RND_PRIM_TRIMESH, V, nvold, I, nfold * 3))
  {
    free(I);
    free(V);
    fclose(F);
    return FALSE;
  }

  EK3_RndPrimEvalNormals(V, nvold, I, nfold * 3);
  free(I);
  free(V);
  fclose(F);
  return TRUE;
} /* End of 'EK3_RndPrimLoad' function */

/* END OF 'rndprim.c' FILE */