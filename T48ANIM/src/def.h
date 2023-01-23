/* FILE NAME: DEF.H
 * PURPOSE: 3D animation project.
 *          Common declaration module.
 * PROGRAMMER: EK3
 * DATE: 11.01.2023
 */

#ifndef __def_h_
#define __def_h_

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>

#include "mth/mth.h"

/* 64 bit integer data type */
typedef long long INT64;
typedef unsigned long long UINT64;

#define MatrMulMatr3(A, B, C) (MatrMulMatr(MatrMulMatr(A, B), C))
#define MatrMulMatr4(A, B, C, D) (MatrMulMatr(MatrMulMatr(A, B), MatrMulMatr(C, D)))
#define MatrMulMatr5(A, B, C, D, E) (MatrMulMatr(MatrMulMatr4(A, B, C, E), E))

#endif /* __def_h_ */

/* END OF 'def.h' FILE */