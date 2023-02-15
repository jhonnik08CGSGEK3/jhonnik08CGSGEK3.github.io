/* FILE NAME   : u_heli.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 13.02.2023
 * PURPOSE     : Cow unit functions.
 */

#include <time.h>

#include "anim/rnd/res/rndres.h"
#include "units/units.h"

typedef struct tagUNIT_OBJECT UNIT_OBJECT;
struct tagUNIT_OBJECT
{
  EK3_UNIT_BASE_FIELDS;
  VEC Pos;
  ek3PRIM Object;
  INT TexNo;
};

static VOID Init( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  ek3MATERIAL mtl =
  {
    "Emerald", {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 76.8,
    0, {0},
    "", 0
  };

  Uni->Pos = VecSet(5, 2, 2);
  EK3_RndPrimLoad(&Uni->Object, "bin/objects/helicopter.obj");
  Uni->TexNo = mtl.Tex[0] = EK3_RndTextureAddFromFileG24("bin/pictures/M.G24", "object");

  Uni->Object.MtlNo = EK3_RndMtlAdd(mtl);
}
static VOID Close( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  EK3_RndPrimFree(&Uni->Object);
}
static VOID Render( UNIT_OBJECT *Uni, ek3ANIM *Ani )
{
  MATR m = MatrIdentity();

  m = MatrMulMatr3(MatrTranslate(Uni->Pos),
                   MatrRotateY(140 * clock() / 1000.0),
                   MatrScale(VecSet(3, 3, 3)));

  EK3_RndPrimDraw(&Uni->Object, m);
}
static VOID Response( UNIT_OBJECT *Uni, ek3ANIM *Ani )
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
}

ek3UNIT * EK3_UnitObjectCreate( VOID )
{
  ek3UNIT *Uni;
  if ((Uni = EK3_AnimUnitCreate(sizeof(UNIT_OBJECT))) == NULL)
    return NULL;
  Uni->Init = (VOID *)Init;
  Uni->Close = (VOID *)Close;
  Uni->Render = (VOID *)Render;
  Uni->Response = (VOID *)Response;
  return Uni;
}

/* END OF 'u_object.c' FILE */