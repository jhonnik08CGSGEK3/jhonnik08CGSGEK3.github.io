/* FILE NAME: rndgrid.c
 * PURPOSE: grid interaction functions.
 * PROGRAMMER: EK3
 * LAST UPDATE: 30.01.2023
*/

#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ek3GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL EK3_RndGridCreate( ek3GRID *G, INT W, INT H, ek3VERTEX *V )
{
  G->W = W;
  G->H = H;
  if ((G->V = malloc(W * H * sizeof(ek3VERTEX))) == NULL)
    return FALSE;

  memcpy(G->V, V, W * H * sizeof(ek3VERTEX));
  return TRUE;
}

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ek3GRID *G;
 * RETURNS: None.
 */
VOID EK3_RndGridFree( ek3GRID *G )
{
  free(G->V);
}

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       ek3GRID *G;
 * RETURNS: None.
 */
VOID EK3_RndGridAutoNormals( ek3VERTEX *V, INT W, INT H )
{
  INT i, j;

  for (i = 0; i < W * H; i++)
    V[i].N = VecSet1(0);

  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      ek3VERTEX
        *P00 = V + i * W + j,
        *P01 = V + i * W + j + 1,
        *P10 = V + (i + 1) * W + j,
        *P11 = V + (i + 1) * W + j + 1;
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }

  for (i = 0; i < W * H; i++)
    V[i].N = VecNormalize(V[i].N);
}

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ek3PRIM *Pr;
 *   - grid data:
 *       ek3GRID *G;
 * RETURNS: None.
 */
VOID EK3_RndPrimFromGrid( ek3PRIM *Pr, ek3GRID *G )
{
  INT *Ind, i, j, k;

  memset(Pr, 0, sizeof(ek3PRIM));
  if ((Ind = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return;
  for (i = 0, k = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = (i + 1) * G->W + j;
      Ind[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }
  EK3_RndPrimCreate(Pr, EK3_RND_PRIM_TRISTRIP, G->V, G->W * G->H,
    Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(Ind);
}

/* END OF 'rndgrid.c' FILE */