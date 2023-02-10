/* FILE NAME   : timer.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 10.02.2023
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

/* Timer intitialization function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
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
} /* End of 'EK3_AnimTimerInit' function */

/* Timer response function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID EK3_AnimTimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  EK3_Anim.GlobalTime = (FLT)(t.QuadPart - StartTime) / TimePerSec;
  EK3_Anim.GlobalDeltaTime = (FLT)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (EK3_Anim.IsPause)
  {
    EK3_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    EK3_Anim.DeltaTime = EK3_Anim.GlobalDeltaTime;
    EK3_Anim.Time = (FLT)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    EK3_Anim.FPS = FrameCounter * TimePerSec / (FLT)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'EK3_AnimTimerResponse' function */

/* END OF 'timer.c' FILE */