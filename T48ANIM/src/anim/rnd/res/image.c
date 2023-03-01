/* FILE NAME: image.c
 * PROGRAMMER: EK3
 * LAST UPDATE: 20.02.2023
 * PURPOSE: 3D rendering programm.
 *          Image handle implementation file.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* Create image function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 *   - new image size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL EK3_ImgCreate( ek3IMAGE *Img, INT W, INT H )
{
  memset(Img, 0, sizeof(ek3IMAGE));
  if ((Img->Pixels = malloc(4 * W * H)) == NULL)
    return FALSE;
  Img->W = W;
  Img->H = H;
  return TRUE;
} /* End of 'EK3_ImgCreate' function */

/* Free image function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 * RETURNS: None.
 */
VOID EK3_ImgFree( ek3IMAGE *Img )
{
  if (Img->Pixels != NULL)
    free(Img->Pixels);
  memset(Img, 0, sizeof(ek3IMAGE));
} /* End of 'EK3_ImgCreate' function */

/* Load image from file function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL EK3_ImgLoad( ek3IMAGE *Img, CHAR *FileName )
{
  INT x, y;
  HBITMAP hBm;
  memset(Img, 0, sizeof(ek3IMAGE));

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)))
  {
    BITMAP bm;

    GetObject(hBm, sizeof(bm), &bm);
    if (bm.bmBitsPixel != 8 && bm.bmBitsPixel != 24 && bm.bmBitsPixel != 32)
    {
      DeleteObject(hBm);
      return FALSE;
    }
    if (!EK3_ImgCreate(Img, bm.bmWidth, bm.bmHeight))
    {
      DeleteObject(hBm);
      return FALSE;
    }

    for (y = 0; y < bm.bmHeight; y++)
      for (x = 0; x < bm.bmWidth; x++)
      {
        DWORD c;
        BYTE *ptr = (BYTE *)bm.bmBits + bm.bmWidthBytes * y + x * bm.bmBitsPixel / 8;

        if (bm.bmBitsPixel == 8)
          c = COM_MAKELONG0123(ptr[0], ptr[0], ptr[0], 255);
        else if (bm.bmBitsPixel == 24 || bm.bmBitsPixel == 32)
          c = COM_MAKELONG0123(ptr[0], ptr[1], ptr[2], 255);
        else
          c = 0xFFFFFFFF;

        Img->Pixels[y * Img->W + x] = c;
      }
    DeleteObject(hBm);
    return TRUE;
  }
  else
  {
    FILE *F;

    if ((F = fopen(FileName, "rb")) != NULL)
    {
      INT W = 0, H = 0, flen, C = 0;
      BYTE *mem;

      fread(&W, 2, 1, F);
      fread(&H, 2, 1, F);
      fseek(F, 0, SEEK_END);
      flen = ftell(F);
      fseek(F, 4, SEEK_SET);

      if (flen == 4 + W * H * 3)
        C = 3;
      else if (flen == 4 + W * H * 4)
        C = 4;
      else if (flen == 4 + W * H)
        C = 1;
      if (C != 0 && (mem = malloc(W * H * C)) != NULL)
      {
        fread(mem, C, W * H, F);

        if (EK3_ImgCreate(Img, W, H))
        {
          for (y = 0; y < H; y++)
            for (x = 0; x < W; x++)
            {
              DWORD c;
              BYTE *ptr = (BYTE *)mem + W * C * y + x * C;

              if (C == 1)
                c = COM_MAKELONG0123(ptr[0], ptr[0], ptr[0], 255);
              else if (C == 3)
                c = COM_MAKELONG0123(ptr[0], ptr[1], ptr[2], 255);
              else if (C == 4)
                c = COM_MAKELONG0123(ptr[0], ptr[1], ptr[2], ptr[3]);
              else
                c = 0xFFFFFFFF;
              Img->Pixels[y * Img->W + x] = c;
            }
        }
        free(mem);
      }
      fclose(F);
    }
    return Img->Pixels != NULL;
  }
} /* End of 'EK3_ImgCreate' function */

/* END OF 'image.c' FILE */