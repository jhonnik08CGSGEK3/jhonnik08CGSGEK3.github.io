/* FILE NAME   : u_land.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 10.02.2023
 * PURPOSE     : Unit examples creation functions.
 */

#include "units/units.h"

typedef struct tagUNIT_GRID UNIT_GRID;
struct tagUNIT_GRID
{
  EK3_UNIT_BASE_FIELDS;
  VEC Pos;
  ek3GRID G;
  ek3PRIM Pr;
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_GRID *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitInit( UNIT_GRID *Uni, ek3ANIM *Ani )
{
  INT w, h, x, y;
  ek3VERTEX *V;
  HBITMAP hBm;
  BITMAP bm;

  if ((hBm = LoadImage(NULL, "bin/pictures/hf.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if ((bm.bmBitsPixel == 8 || bm.bmBitsPixel == 24) &&
        (V = malloc(w * h * sizeof(ek3VERTEX))) != NULL)
    {
      BYTE *Bits = bm.bmBits;

      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x * (bm.bmBitsPixel / 8)];

          V[y * w + x].P = VecSet(x / (w - 1.0),
                                  hgt / 255.0,
                                  1 - y / (h - 1.0));
          V[y * w + x].C = Vec4Set(0.70, 0.50, 0.10, 1);
        }
      EK3_RndGridAutoNormals(V, w, h);
      EK3_RndGridCreate(&Uni->G, w, h, V);
      EK3_RndPrimFromGrid(&Uni->Pr, &Uni->G);
      free(V);
    }
  }
} /* End of 'EK3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_GRID *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitClose( UNIT_GRID *Uni, ek3ANIM *Ani )
{
  EK3_RndGridFree(&Uni->G);
  EK3_RndPrimFree(&Uni->Pr);
} /* End of 'EK3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       UNIT_GRID *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitResponse( UNIT_GRID *Uni, ek3ANIM *Ani )
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
 *       UNIT_GRID *Uni;
 *   - animation context:
 *       ek3ANIM *Ani;
 * RETURNS: None.
 */
static VOID EK3_UnitRender( UNIT_GRID *Uni, ek3ANIM *Ani )
{
  VEC S = {25, 25, 25};

  EK3_RndPrimDraw(&Uni->Pr, MatrScale(S));
} /* End of 'EK3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (UNIT_GRID *) pointer to created unit.
 */
ek3UNIT * EK3_UnitGridCreate( VOID )
{
  ek3UNIT *Uni;

  /* Memory allocation */
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_GRID))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)EK3_UnitInit;
  Uni->Close = (VOID *)EK3_UnitClose;
  Uni->Render = (VOID *)EK3_UnitRender;
  Uni->Response = (VOID *)EK3_UnitResponse;

  return Uni;
} /* End of 'EK3_AnimUnitCreate' function */

/* END OF 'u_land.c' FILE */