/* FILE NAME   : u_heli.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 20.01.2023
 * PURPOSE     : Cow unit functions.
 */
/*
#include <time.h>

#include "units/units.h"

typedef struct tagUNIT_BALL UNIT_BALL;
struct tagUNIT_BALL
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  VEC Ball[BALL_GRID_H][BALL_GRID_W];
  POINT pnts[BALL_GRID_H][BALL_GRID_W];
};

static VOID Init( UNIT_BALL *Uni, ek3ANIM *Ani )
{
}
static VOID Close( UNIT_BALL *Uni, ek3ANIM *Ani )
{
  EK3_RndPrimFree(&Uni->Object);
}
static VOID Render( UNIT_BALL *Uni, ek3ANIM *Ani )
{
  INT i, j, s = 5;
  MATR m = MatrIdentity();

  m = MatrMulMatr3(MatrTranslate(Uni->Pos),
                   MatrRotateY(140 * clock() / 1000.0),
                   MatrScale(VecSet1(-1)));

  for (i = 0; i < BALL_GRID_H; i++)
    for (j = 0; j < BALL_GRID_W; j++)
    {
      Uni->Ball[i][j] = VecTransform(Uni->Ball[i][j], m);
      Uni->pnts[i][j].x = Uni->Ball[i][j].X + W / 2;
      Uni->pnts[i][j].y = Uni->Ball[i][j].Y + H / 2;
    }
  EK3_RndPrimDraw(&Uni->Object, m);
}
static VOID Response( UNIT_BALL *Uni, ek3ANIM *Ani )
{
  FLT theta, phi;
  INT i, j;

  for (i = 0, theta = 0; i < BALL_GRID_H; i++, theta -= PI / (BALL_GRID_H - 1))
    for (j = 0, phi = 0; j < BALL_GRID_W; j++, phi += 2 * PI / (BALL_GRID_W - 1))
    {
      Uni->Ball[i][j].X = R * sin(theta) * sin(phi);
      Uni->Ball[i][j].Y = R * cos(theta);
      Uni->Ball[i][j].Z = R * sin(theta) * cos(phi);
    }
}

ek3UNIT * EK3_UnitBallCreate( VOID )
{
  ek3UNIT *Uni;
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)Init;
  Uni->Close = (VOID *)Close;
  Uni->Render = (VOID *)Render;
  Uni->Response = (VOID *)Response;
  return Uni;
}
*/

/* END OF 'u_bball.c' FILE */