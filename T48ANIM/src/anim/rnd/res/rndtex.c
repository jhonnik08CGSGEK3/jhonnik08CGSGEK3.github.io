/* FILE NAME: rndtex.c
 * PURPOSE: Textures usage functions.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <stdio.h>

#include "anim/rnd/rnd.h"

ek3TEXTURE EK3_RndTextures[EK3_MAX_TEXTURES];
INT EK3_RndTexturesSize;

/* Add texture image to texture array function.
 * ARGUMENTS:
 *  - CHAR *Name - texture name.
 *  - INT W, H - texture size.
 *  - VOID *Bits - image.
 * RETURNS:
 *   (INT) - array size, -1 if array is full.
 */
INT EK3_RndTextureAddImg( CHAR *Name, INT W, INT H, VOID *Bits )
{
  if (EK3_RndTexturesSize >= EK3_MAX_TEXTURES)
    return -1;

  /* Allocate texture space */
  glGenTextures(1, &EK3_RndTextures[EK3_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, EK3_RndTextures[EK3_RndTexturesSize].TexId);

  /* Upload texture */
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(EK3_RndTextures[EK3_RndTexturesSize].Name, Name, EK3_STR_MAX - 1);
  EK3_RndTextures[EK3_RndTexturesSize].W = W;
  EK3_RndTextures[EK3_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);

  return EK3_RndTexturesSize++;
} /* End of 'EK3_RndTextureAddImg' function */

/* Add texture to texture array from .G24 file function.
 * ARGUMENTS:
 *  - CHAR *FileName - texture file name.
 *  - CHAR *Name - texture name.
 * RETURNS:
 *   (INT) - array size, -1 if program didnt work.
 */
INT EK3_RndTextureAddFromFileG24( CHAR *FileName, CHAR *Name )
{
  FILE *F;

  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w = 0, h = 0;
    BYTE *mem;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    if ((mem = malloc(w * h * 3)) != NULL)
    {
      INT n;
      fread(mem, 3, w * h, F);

      glGenTextures(1, &EK3_RndTextures[EK3_RndTexturesSize].TexId);
      glBindTexture(GL_TEXTURE_2D, EK3_RndTextures[EK3_RndTexturesSize].TexId);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      /* glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR,
           GL_UNSIGNED_BYTE, mem); */
      n = log(w > h ? w : h) / log(2);
      n = n < 1 ? 1 : n;

      glTexStorage2D(GL_TEXTURE_2D, n, GL_RGB8, w, h);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                      GL_BGR, GL_UNSIGNED_BYTE, mem);

      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      free(mem);
    }
    else
      return -1;
    fclose(F);
  }
  else
    return -1;
  return EK3_RndTexturesSize++;
} /* End of 'EK3_RndTextureAddFromFileG24' function */

/* Texture initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndTexInit( VOID )
{
  EK3_RndTexturesSize = 0;
} /* End of 'EK3_RndTexInit' function */

/* Texture deinitialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < EK3_RndTexturesSize; i++)
    glDeleteTextures(1, &EK3_RndTextures[i].TexId);
  EK3_RndTexturesSize = 0;
} /* End of 'EK3_RndTexClose' function */

/* END OF 'rndtex.c' FILE */