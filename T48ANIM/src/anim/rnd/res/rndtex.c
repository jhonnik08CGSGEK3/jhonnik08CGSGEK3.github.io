/* FILE NAME: rndtex.c
 * PURPOSE: Textures usage functions.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <stdio.h>

#include "anim/rnd/rnd.h"

ek3TEXTURE EK3_RndTextures[EK3_MAX_TEXTURES];
INT EK3_RndTexturesSize;

/* Add texture from file stock function.
 * ARGUMENTS:
 *   - Texture name:
 *       CHAR *FileName;
 *   - Image size:
 *       INT W, H;
 *   - Bytes per pixel:
 *       INT C;
 *   - Image data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) Texture stock number (0 if error is occured).
 */
INT EK3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  if (EK3_RndTexturesSize >= EK3_MAX_TEXTURES)
    return -1;

  /* Allocate texture space */
  glGenTextures(1, &EK3_RndTextures[EK3_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, EK3_RndTextures[EK3_RndTexturesSize].TexId);

  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);

  glGenerateMipmap(GL_TEXTURE_2D);

  /* Upload texture */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  /* Add to stock */
  strncpy(EK3_RndTextures[EK3_RndTexturesSize].Name, Name, EK3_STR_MAX - 1);
  EK3_RndTextures[EK3_RndTexturesSize].W = W;
  EK3_RndTextures[EK3_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);
  return EK3_RndTexturesSize++;
} /* End of 'EK3_RndTexAddImg' function */

/* Add texture by OpenGL low-level format to stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size in pixels:
 *       INT W, H;
 *   - OpenGL texture element data type:
 *       INT GLType;
 * RETURNS:
 *   (INT) texture stock number (0 if error is occured).
 */
INT EK3_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType )
{
  if (EK3_RndTexturesSize >= EK3_MAX_TEXTURES)
    return -1;

  /* Setup OpenGL texture */
  glGenTextures(1, &EK3_RndTextures[EK3_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, EK3_RndTextures[EK3_RndTexturesSize].TexId);

  glTexStorage2D(GL_TEXTURE_2D, 1, GLType, W, H);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  /* Add to stock */
  EK3_RndTextures[EK3_RndTexturesSize].W = W;
  EK3_RndTextures[EK3_RndTexturesSize].H = H;
  strncpy(EK3_RndTextures[EK3_RndTexturesSize].Name, Name, EK3_STR_MAX - 1);
  return EK3_RndTexturesSize++;
} /* End of 'EK3_RndTexAddFmt' function */

INT EK3_RndTexAddFromFile( CHAR *FileName, CHAR *Name )
{
  INT res = -1;
  ek3IMAGE Img;

  if (EK3_ImgLoad(&Img, FileName))
  {
    res = EK3_RndTexAddImg(Name, Img.W, Img.H, 4, Img.Pixels);
    EK3_ImgFree(&Img);
  }
  return res;
} /* End of 'EK3_RndTexAddFromFile' function */

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