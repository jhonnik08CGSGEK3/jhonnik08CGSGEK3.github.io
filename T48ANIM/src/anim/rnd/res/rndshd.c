/* FILE NAME: rndshd.c
 * PURPOSE: Shaders usage functions.
 * PROGRAMMER: EK3
 * DATE: 10.02.2023
 */

#include <stdio.h>

#include "anim/rnd/res/rndres.h"

/* Shaders stock */
ek3SHADER EK3_RndShaders[EK3_MAX_SHADERS]; /* Array of shaders */
INT EK3_RndShadersSize;                    /* Shaders array store size */

/* Load text from file function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * EK3_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
  /* Get file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  /* Read text from file */
  rewind(F);
  fread(txt, 1, flen, F);
  txt[flen] = 0;

  fclose(F);
  return txt;
} /* End of 'EK3_RndShdLoadTextFromFile' function */

/* Store shader log to file function.
 * ARGUMENTS:
 *   - shader prefix file name:
 *       CHAR *FileNamePrefix;
 *   - shader handle part name:
 *       CHAR *Part;
 *   - logging text:
 *       CHAR *Text ;
 * RETURNS: None.
 */
static VOID EK3_RndShdLog( CHAR *FileNamePrefix, CHAR *Part, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/EK3{30}SHD.LOG", "a")) == NULL)
    return;
  fprintf(F, "%s/%s\n%s\n", FileNamePrefix, Part, Text);
  fclose(F);
} /* End of ' EK3_RndShdLog' function */

/* Shader program load function.
 * ARGUMENTS:
 *   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader program index (0 if error is occured).
 */
INT EK3_RndShdLoad( CHAR *FileNamePrefix )
{
  INT i, prg = 0, res;
  CHAR *txt;
  struct
  {
    CHAR *Name; /* Shader name */
    INT Type;   /* Shader type (see GL_***_SHADER) */
    INT Id;     /* Result shader Id */
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
  };
  INT NoofS = sizeof(shds) / sizeof(shds[0]);
  BOOL isok = TRUE;
  static CHAR Buf[30000];

  for (i = 0; i < NoofS && isok; i++)
  {
    /* Load file */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shds[i].Name);
    txt = EK3_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      EK3_RndShdLog(FileNamePrefix, shds[i].Name, "Error load file");
      isok = FALSE;
      break;
    }

    /* Create shader */
    if ((shds[i].Id = glCreateShader(shds[i].Type)) == 0)
    {
      EK3_RndShdLog(FileNamePrefix, shds[i].Name, "Error shader creation");
      free(txt);
      isok = FALSE;
      break;
    }

    /* Attach shader text */
    glShaderSource(shds[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      EK3_RndShdLog(FileNamePrefix, shds[i].Name, Buf);
      isok = FALSE;
      break;
    }
  }

  /* Create shader program */
  if (isok)
    if ((prg = glCreateProgram()) == 0)
    {
      EK3_RndShdLog(FileNamePrefix, "PROG", "Error program creation");
      isok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);

      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        EK3_RndShdLog(FileNamePrefix, "PROG", Buf);
        isok = FALSE;
      }
    }

  /* Error handle */
  if (!isok)
  {
    /* Delete shaders */
    for (i = 0; i < NoofS; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }

    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'EK3_RndShdLoad' function */

/* Shader program free function.
 * ARGUMENTS:
 *   - shader program index:
 *       INT Prg;
 * RETURNS: None.
 */
VOID EK3_RndShdFree( INT Prg )
{
  INT shds[5] = {0}, n, i;

  if (Prg == 0 || !glIsProgram(Prg))
    return;
  glGetAttachedShaders(Prg, 5, &n, shds);
  /* Delete shaders */
  for (i = 0; i < n; i++)
    if (shds[i] != 0 &&  glIsShader(shds[i]))
    {
      glDetachShader(Prg, shds[i]);
      glDeleteShader(shds[i]);
    }
  /* Delete program */
  glDeleteProgram(Prg);
} /* End of 'EK3_RndShdFree' function */

/* Shader initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdInit( VOID )
{
  EK3_RndShadersSize = 0;
  EK3_RndShdAdd("default");
} /* End of 'EK3_RndShdInit' function */

/* Shader deinitialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < EK3_RndShadersSize; i++)
    EK3_RndShdFree(EK3_RndShaders[EK3_RndShadersSize].ProgId);
  EK3_RndShadersSize = 0;
} /* End of 'EK3_RndShdClose' function */

/* Add shader to shader array function.
 * ARGUMENTS:
 *  - CHAR *ShaderFileNamePrefix - name of folder to add shaders from.
 * RETURNS:
 *   (INT) - array of shaders size.
 */
INT EK3_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  if (EK3_RndShadersSize >= EK3_MAX_SHADERS)
    return 0;

  strncpy(EK3_RndShaders[EK3_RndShadersSize].Name, ShaderFileNamePrefix, EK3_STR_MAX - 1);
  EK3_RndShaders[EK3_RndShadersSize].ProgId = EK3_RndShdLoad(ShaderFileNamePrefix);

  return EK3_RndShadersSize++;
} /* End of 'EK3_RndShdAdd' function */

/* Shader update function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < EK3_RndShadersSize; i++)
  {
    EK3_RndShdFree(EK3_RndShaders[i].ProgId);
    EK3_RndShaders[i].ProgId = EK3_RndShdLoad(EK3_RndShaders[i].Name);
  }
} /* End of 'EK3_RndShdUpdate' function */

/* END OF 'rndshd.c' FILE */