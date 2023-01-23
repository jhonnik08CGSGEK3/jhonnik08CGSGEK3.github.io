/* FILE NAME   : anim.h
 * PROGRAMMER  : EK3
 * LAST UPDATE : 18.01.2023
 * PURPOSE     : Unit and Anim structures and function prototypes.
 */

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define UNIT_BASE_FIELDS \
  VOID (*Init)( ek3UNIT *Uni, ek3ANIM *Ani );     \
  VOID (*Close)( ek3UNIT *Uni, ek3ANIM *Ani );    \
  VOID (*Response)( ek3UNIT *Uni, ek3ANIM *Ani ); \
  VOID (*Render)( ek3UNIT *Uni, ek3ANIM *Ani )

#define EK3_MAX_UNITS 3000

typedef struct tagek3UNIT ek3UNIT;
typedef struct tagek3ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  ek3UNIT * Units[EK3_MAX_UNITS];
  INT NumOfUnits;

  /* Timer parameters */
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL
    IsPause;

  /* Keyboard parameters */
  BYTE Keys[256];
  BYTE KeysOld[256];
  BYTE KeysClick[256];

  /* Mouse parameters */
  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  /* Joystick parameters */
  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1, 0..7] */
  DBL JX, JY, JZ, JR;                     /* Joystick axis */
} ek3ANIM;

struct tagek3UNIT
{
  UNIT_BASE_FIELDS;
};

extern ek3ANIM EK3_Anim;
INT EK3_MouseWheel;

/* Animation funcs */
VOID EK3_AnimInit( HWND hWnd );
VOID EK3_AnimClose( VOID );
VOID EK3_AnimResize( INT W, INT H );
VOID EK3_AnimCopyFrame( VOID );
VOID EK3_AnimRender( VOID );
VOID EK3_AnimUnitAdd( ek3UNIT *Uni );
VOID EK3_AnimFlipFullScreen( VOID );
VOID EK3_AnimExit( VOID );

/* Timer funcs */
VOID EK3_AnimTimerInit( VOID );
VOID EK3_AnimTimerResponse( VOID );

/* Input system funcs */
VOID EK3_KeyboardInit( VOID );
VOID EK3_KeyboardResponse( VOID );
VOID EK3_MouseInit( VOID );
VOID EK3_MouseResponse( VOID );

VOID EK3_AnimInputInit( VOID );
VOID EK3_AnimInputResponse( VOID );

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */