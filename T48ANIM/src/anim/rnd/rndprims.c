/* FILE NAME  : rndprims.c
 * PURPOSE    : 3D animation project.
 *              Rendering system primitives handle module.
 * PROGRAMMER : EK3
 * LAST UPDATE: 22.02.2023
 */

#include <stdio.h>
#include <string.h>

#include "anim/anim.h"

/***
 * Primitives support
 ***/

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL EK3_RndPrimsCreate( ek3PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(ek3PRIMS));
  if ((Prs->Prims = malloc(sizeof(ek3PRIM) * NumOfPrims)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(ek3PRIM) * NumOfPrims);
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'EK3_RndPrimsCreate' function */

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 * RETURNS: None.
 */
VOID EK3_RndPrimsFree( ek3PRIMS *Prs )
{
  if (Prs->Prims != NULL)
    free(Prs->Prims);
  memset(Prs, 0, sizeof(ek3PRIMS));
} /* End of 'EK3_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID EK3_RndPrimsDraw( ek3PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Trans, World);

  /* Draw all nontransparent primitives */ 
  //EK3_RndShdAddonI[0] = Prs->NumOfPrims;
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (EK3_RndMtlGet(Prs->Prims[i].MtlNo).Trans == 1)
    {
      //EK3_RndShdAddonI[1] = i;
      EK3_RndPrimDraw(&Prs->Prims[i], m);
    }

  /* Draw all transparent primitives */ 
  glEnable(GL_CULL_FACE);

  /* Draw all front-face-culling */ 
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (EK3_RndMtlGet(Prs->Prims[i].MtlNo).Trans != 1)
    {
      //EK3_RndShdAddonI[1] = i;
      EK3_RndPrimDraw(&Prs->Prims[i], m);
    }

  /* Draw all back-face-culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (EK3_RndMtlGet(Prs->Prims[i].MtlNo).Trans != 1)
    {
      //EK3_RndShdAddonI[1] = i;
      EK3_RndPrimDraw(&Prs->Prims[i], m);
    }

  glDisable(GL_CULL_FACE);
} /* End of 'EK3_RndPrimsDraw' function */

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ek3PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL EK3_RndPrimsLoad( ek3PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, p, m, t;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  /* Allocate memory and load whole file to memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;

  if (!EK3_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }
  /* Load primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    DWORD MtlNo;              /* Material number in table below (in material section) */
    ek3VERTEX *V;
    INT *Ind;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;

    V = (ek3VERTEX *)ptr;
    ptr += sizeof(ek3VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;

    EK3_RndPrimCreate(&Prs->Prims[p], EK3_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = EK3_RndMaterialsSize + MtlNo;
    if (p == 0)
      Prs->MinBB = Prs->Prims[p].MinBB, Prs->MaxBB = Prs->Prims[p].MaxBB;
    else
    {
      if (Prs->MinBB.X > Prs->Prims[p].MinBB.X)
        Prs->MinBB.X = Prs->Prims[p].MinBB.X;
      if (Prs->MaxBB.X < Prs->Prims[p].MaxBB.X)
        Prs->MaxBB.X = Prs->Prims[p].MaxBB.X;

      if (Prs->MinBB.Y > Prs->Prims[p].MinBB.Y)
        Prs->MinBB.Y = Prs->Prims[p].MinBB.Y;
      if (Prs->MaxBB.Y < Prs->Prims[p].MaxBB.Y)
        Prs->MaxBB.Y = Prs->Prims[p].MaxBB.Y;

      if (Prs->MinBB.Z > Prs->Prims[p].MinBB.Z)
        Prs->MinBB.Z = Prs->Prims[p].MinBB.Z;
      if (Prs->MaxBB.Z < Prs->Prims[p].MaxBB.Z)
        Prs->MaxBB.Z = Prs->Prims[p].MaxBB.Z;
    }
  }

  /* Load materials */
  for (m = 0; m < NumOfMaterials; m++)
  {
    struct
    {
      CHAR Name[300]; /* Material name */
      /* Illumination coefficients */
      VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
      FLT Ph;             /* Phong power coefficient – shininess */
      FLT Trans;          /* Transparency factor */
      DWORD Tex[8];       /* Texture references 
                           * (8 time: texture number in G3DM file, -1 if no texture) */
      /* Shader information */
      CHAR ShaderString[300]; /* Additional shader information */
      DWORD Shader;       /* Shader number (uses after load into memory) */
    } *fmat;
    ek3MATERIAL mtl;

    fmat = (VOID *)ptr;
    ptr += sizeof(*fmat);
    strncpy(mtl.Name, fmat->Name, EK3_STR_MAX);
    mtl.Ka = fmat->Ka;
    mtl.Kd = fmat->Kd;
    mtl.Ks = fmat->Ks;
    mtl.Ph = fmat->Ph;
    mtl.Trans = fmat->Trans;
    for (t = 0; t < 8; t++)
      mtl.Tex[t] = fmat->Tex[t] == -1 ? -1 : EK3_RndTexturesSize + fmat->Tex[t];
    EK3_RndMtlAdd(&mtl);
  }

  /* Load textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name;
    DWORD W, H, C;

    Name = (CHAR *)ptr;
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    EK3_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }

  free(mem);
  return TRUE;
} /* End of 'EK3_RndPrimsLoad' function */

/* END OF 'rndprims.c' FILE */