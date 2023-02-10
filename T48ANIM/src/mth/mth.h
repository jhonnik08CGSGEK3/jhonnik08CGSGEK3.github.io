/* FILE NAME: mth.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: EK3
 * LAST UPDATE: 17.01.2023
 */

#ifndef __mth_h_
#define __mth_h_

#include <commondf.h>

#include <math.h>

#pragma warning(disable: 4244)

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Base float point types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, W;
} VEC4;

/* Transformation matrix representation type */
typedef struct tagMATR
{
  FLT A[4][4]; /* Matrix elements */
} MATR;

#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

/* Swap two FLT numbers function.
 * ARGUMENTS:
 *   FLT *a, *b - numbers to be swapped
 * RETURNS:
 *   None.
 */
__inline VOID Swap( FLT *a, FLT *b )
{
  FLT tmp = *a;

  *a = *b;
  *b = tmp;
} /* End of 'Swap' function */

/* Swap two FLT numbers function.
 * ARGUMENTS:
 *   FLT *a, *b - numbers to be swapped
 * RETURNS:
 *   None.
 */
__inline VOID Swap1( FLT *a, FLT *b )
{
  FLT tmp = *a;

  *a = *b;
  *b = tmp;
} /* End of 'Swap1' function */

/* New vector setup function.
 * ARGUMENTS:
 *  - FLT X, Y, Z - New vector data
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC V = {X, Y, Z};

  return V;
} /* End of 'VecSet' function */

/* New vector setup with one component function.
 * ARGUMENTS:
 *  - FLT X - New vector data
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet1( FLT X )
{
  VEC V = {X, X, X};

  return V;
} /* End of 'VecSet1' function */

/* New 4D vector setup function.
 * ARGUMENTS:
 *  - FLT A, B, C, D - New vector data
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Set( FLT A, FLT B, FLT C, FLT D )
{
  VEC4 V = {A, B, C, D};

  return V;
} /* End of 'Vec4Set' function */

/* New 4D vector setup with one component function.
 * ARGUMENTS:
 *  - FLT A - New vector data
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Set1( FLT A )
{
  VEC4 V = {A, A, A, A};

  return V;
} /* End of 'Vec4Set1' function */

/* New 2D vector setup function.
 * ARGUMENTS:
 *  - FLT A, B - New vector data
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC2 Vec2Set( FLT A, FLT B )
{
  VEC2 V = {A, B};

  return V;
} /* End of 'Vec4Set' function */

/* New 2D vector setup with one component function.
 * ARGUMENTS:
 *  - FLT A - New vector data
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC2 Vec2Set1( FLT A )
{
  VEC2 V = {A, A};

  return V;
} /* End of 'Vec4Set1' function */

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Substract two vectors function.
 * ARGUMENTS:
 *   - vectors to be substracted:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector by number function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V1;
 *   - nubmer:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

/* Divide vector by number function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V1;
 *   - nubmer:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V1, FLT N )
{
  if (N != 0)
    return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
  else
    return V1;
} /* End of 'VecDivNum' function */

/* Inverting vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Dot product function.
 * ARGUMENTS:
 *   - vectors to determine dot product:
 *       VEC V1, V2;
 * RETURNS:
 *   (FLT) number.
 */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* Cross product function.
 * ARGUMENTS:
 *   - vectors to cross:
 *       VEC V1, V2;
 * RETURNS:
 *   (FLT) number.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Vector length counting function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (FLT) result lenght.
 */
__inline FLT VecLen2( VEC V )
{
  return VecDotVec(V, V);
} /* End of 'VecLen2' function */

/* Square root vector length counting function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (FLT) result length.
 */
__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'VecLen' function */

/* Vector normalizing function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* End of 'VecNormalize' function */

/* Point-vector transofrmation with matrix function.
 * ARGUMENTS:
 *   - source point-vector:
 *       VEC V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]),
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]),
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]));
} /* End of 'PointTransform' function */

/* Vector transofrmation with matrix function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0]),
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1]),
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]));
} /* End of 'VecTransform' function */

/* Vector by matrix multiplication (with homogenious divide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplication matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/* Vector by 4x4 matrix multiplication (with homogenious divide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplication matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr4x4( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][4] + V.Y * M.A[1][4] + V.Z * M.A[2][4] + M.A[3][4];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr4x4' function */

/* Vector add with return function.
 * ARGUMENTS:
 *   - VEC *VRes - result vector
 *   - VEC V2 - second vector
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVecEq( VEC *VRes, VEC V2 )
{
  VRes->X += V2.X;
  VRes->Y += V2.Y;
  VRes->Z += V2.Z;
  return *VRes;
} /* End of 'VecAddVecEq' function */

/* Vector projection on another vector function.
 * ARGUMENTS:
 * - VEC P - vector to be projected
 * - VEC Q - vector onto which 1st vector should be projected
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecProj( VEC P, VEC Q )
{
  FLT DotPr = VecDotVec(P, Q);

  return VecMulNum(VecSet(Q.X * DotPr, Q.Y * DotPr, Q.Z * DotPr), 1 / VecLen2(Q));
} /* End of 'VecProj' function */
/* Creating Unit Matrix function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (MATR) result (unit) matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  MATR M = UnitMatrix;

  return M;
} /* End of 'MatrIdentity' function */

/* Matrix creation function.
 * ARGUMENTS:
 *   - FLT A00..A33 - numbers to put in matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
                       FLT A10, FLT A11, FLT A12, FLT A13,
                       FLT A20, FLT A21, FLT A22, FLT A23,
                       FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return m;
} /* End of 'MatrSet' function */

/* Matrix translation function.
 * ARGUMENTS:
 *   - VEC T - vector to translate in matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC T )
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 T.X, T.Y, T.Z, 1);
} /* End of 'MatrTranslate' function */

/* Matrix scaling function.
 * ARGUMENTS:
 *   - VEC S - vector to scale in matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Z, 0,
                 0, 0, 0, 1);
} /* End of 'MatrScale' function */

/* Matrix rotation around X-axis function.
 * ARGUMENTS:
 *   - FLT AngleInDegree - rotation angle
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);

  return MatrSet(1, 0, 0, 0,
                 0, co, si, 0,
                 0, -si, co, 0,
                 0,  0, 0, 1);
} /* End of 'MatrRotateX' function */

/* Matrix rotation around X-axis function.
 * ARGUMENTS:
 *   - FLT AngleInDegree - rotation angle
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);

  return MatrSet(co, 0, -si, 0,
                 0, 1, 0, 0,
                 si, 0, co, 0,
                 0,  0, 0, 1);
} /* End of 'MatrRotateY' function */

/* Matrix rotation around X-axis function.
 * ARGUMENTS:
 *   - FLT AngleInDegree - rotation angle
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);

  return MatrSet(co, si, 0, 0,
                -si, co, 0, 0,
                  0,  0, 1, 0,
                  0,  0, 0, 1);
} /* End of 'MatrRotateZ' function */

/* Matrix translation function.
 * ARGUMENTS:
 *   - VEC T - vector to translate in matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( FLT AngleInDegree, VEC V1 )
{
  FLT
    A = D2R(AngleInDegree),
    si = sin(A), co = cos(A);
  VEC V = VecNormalize(V1);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),
        V.X * V.Y * (1 - co) + V.Z * si,
          V.X * V.Z * (1 - co) - V.Y * si, 0},
      {V.Y * V.X * (1 - co) - V.Z * si,
        co + V.Y * V.Y * (1 - co),
          V.Y * V.Z * (1 - co) + V.X * si, 0},
      {V.Z * V.X * (1 - co) + V.Y * si,
        V.Z * V.Y * (1 - co) - V.X * si,
          co + V.Z * V.Z * (1 - co),       0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* End of 'MatrRotate' function */

/* Matrix multiplication function.
 * ARGUMENTS:
 *   - MATR M1, M2 - matrixes to multiply
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r = {{{0}}};
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];

  return r;
} /* End of 'MatrMulMatr' function */

/* Matrix transposition function.
 * ARGUMENTS:
 *   - MATR M - matrix to transpose
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if (i != j)
        Swap1(&M.A[i][j], &M.A[j][i]);

  return M;
} /* End of 'MatrTranspose' function */

/* 3x3 Matrix determinant counting function.
 * ARGUMENTS:
 *   - FLT A11..A33 - matrix numbers
 * RETURNS:
 *   (FLT) determinant.
 */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* 4x4 Matrix determinant counting function.
 * ARGUMENTS:
 *   - MATR M - matrix
 * RETURNS:
 *   (FLT) determinant.
 */
__inline FLT MatrDeterm4x4( MATR M )
{
  return
    +M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                               M.A[2][1], M.A[2][2], M.A[2][3],
                               M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    +M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                               M.A[2][0], M.A[2][1], M.A[2][3],
                               M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm4x4' function */

/* Matrix Inverse function.
 * ARGUMENTS:
 *   - MATR M - matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm4x4(M);
  INT s[] = {1, -1}, i, j,
    P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};

  if (det == 0)
    return MatrIdentity();

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        s[(i + j) % 2] *
        MatrDeterm3x3(M.A[P[i][0]][P[j][0]], M.A[P[i][0]][P[j][1]], M.A[P[i][0]][P[j][2]],
                      M.A[P[i][1]][P[j][0]], M.A[P[i][1]][P[j][1]], M.A[P[i][1]][P[j][2]],
                      M.A[P[i][2]][P[j][0]], M.A[P[i][2]][P[j][1]], M.A[P[i][2]][P[j][2]]) / det;

  return r;
} /* End of 'MatrInverse' function */

/* Getting matrix view transformation function.
 * ARGUMENTS:
 *   - MATR M - matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    r = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecCrossVec(r, Dir);
  MATR m =
  {
    {
      {r.X, Up.X, -Dir.X, 0},
      {r.Y, Up.Y, -Dir.Y, 0},
      {r.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, r), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Getting matrix view ortho function.
 * ARGUMENTS:
 *   - MATR M - matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrOrtho( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  /*
  if (fabs(l) == fabs(r) || fabs(t) == fabs(b))
    return MatrSet(2 / r, 0, 0, 0,
                   0, 2 / t, 0, 0,
                   0, 0, -2 / (f - n), 0,
                   0, 0, -((f + n) / (f - n)), 1);
  */
  return MatrSet(2 / (r - l), 0, 0, 0,
                 0, 2 / (t - b), 0, 0,
                 0, 0, -2 / (f - n), 0,
                 -((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1);
} /* End of 'MatrOrtho' function */

/* Central projection matrix function.
 * ARGUMENTS:
 *   - MATR M - matrix
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustrum( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  return MatrSet(2 * n / (r - l), 0, 0, 0,
                 0, 2 * n / (t - b), 0, 0,
                 (r + l) / (r - l), (t + b) / (t - b), -((f + n) / (f - n)), -1,
                 0, 0, -2 * n * f / (f - n), 0);
} /* End of 'MatrView' function */

#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */
