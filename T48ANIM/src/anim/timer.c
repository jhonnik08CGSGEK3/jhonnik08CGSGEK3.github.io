/* FILE NAME   : timer.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 18.01.2023
 * PURPOSE     : Timer functions.
 */

#include <time.h>

#include "anim/anim.h"

static UINT64
  StartTime,    /* Start programm time */
  OldTime,      /* Previous time frame */
  OldTimeFPS,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */

VOID EK3_AnimTimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  EK3_Anim.IsPause = FALSE;
  EK3_Anim.FPS = 30.0;
  PauseTime = 0;
}

VOID EK3_AnimTimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  EK3_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  EK3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (EK3_Anim.IsPause)
  {
    EK3_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    EK3_Anim.DeltaTime = EK3_Anim.GlobalDeltaTime;
    EK3_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    EK3_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}

/* END OF 'timer.c' FILE */