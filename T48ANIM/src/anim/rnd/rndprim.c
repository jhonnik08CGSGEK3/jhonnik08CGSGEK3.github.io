/* FILE NAME: rndprim.c
 * PURPOSE: Function with primitives.
 * PROGRAMMER: EK3
 * DATE: 11.01.2023
 */

#include <stdio.h>
#include <windows.h>
#include <string.h>
//#include <commondf.h>

#include "def.h"
#include "anim/rnd/rnd.h"

BOOL EK3_RndPrimCreate( ek3PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(ek3PRIM));
  size = sizeof(ek3VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
}

VOID EK3_RndPrimFree( ek3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(ek3PRIM));
}

VOID EK3_RndPrimDraw( ek3PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, EK3_RndMatrVP);

  /* Build projection */
  glLoadMatrixd(wvp.A[0]);

  /* Draw triangles */
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
    glVertex3dv(&Pr->V[Pr->I[i]].P.X);
  glEnd();
}

BOOL EK3_RndPrimLoad( ek3PRIM *Pr, CHAR *FileName )
{
  INT nv, nf;
  FILE *F;
  CHAR Buf[1000];

  memset(Pr, 0, sizeof(ek3PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  if (!EK3_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }

  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
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
        n1 = Pr->NumOfV - (-n1);
      if (n2 > 0)
        n2--;
      else if (n3 < 0)
        n2 = Pr->NumOfV - (-n2);
      if (n3 > 0)
        n3--;
      else if (n3 < 0)
        n3 = Pr->NumOfV - (-n3);

      Pr->I[nf++] = n1;
      Pr->I[nf++] = n2;
      Pr->I[nf++] = n3;
    }

  fclose(F);
  return TRUE;
}

/* END OF 'rndprim.c' FILE */