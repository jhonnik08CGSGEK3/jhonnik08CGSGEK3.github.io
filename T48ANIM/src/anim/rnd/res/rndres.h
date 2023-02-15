/* FILE NAME: rndres.h
 * PURPOSE: Resources functions prototypes and parameters.
 * PROGRAMMER: EK3
 * DATE: 06.02.2023
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

#define EK3_STR_MAX 300
#define EK3_MAX_SHADERS 30
#define EK3_MAX_TEXTURES 47
#define EK3_MAX_MATERIALS 50

/* Shader program store type */
typedef struct tagek3SHADER
{
  CHAR Name[EK3_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} ek3SHADER;

/* Texture representation type */
typedef struct tagek3TEXTURE
{
  CHAR Name[EK3_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} ek3TEXTURE;

/* Material representation type */
typedef struct tagek3MATERIAL
{
  CHAR Name[EK3_STR_MAX];

  /* Illumination coefficients */
  VEC Ka, Kd, Ks; /* Ambient, diffuse, specular coefficients */
  FLT Ph;         /* Phong power coefficient */
  FLT Trans;      /* Transparency factor */
  INT Tex[8];     /* Texture references from texture table */

  /* Shader string */
  CHAR ShaderStr[EK3_STR_MAX];
  INT ShdNo;
} ek3MATERIAL;

/***
  Shaders handle functions
***/

/* Shader initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdInit( VOID );

/* Shader deinitialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdClose( VOID );

/* Add shader to shader array function.
 * ARGUMENTS:
 *  - CHAR *ShaderFileNamePrefix - name of folder to add shaders from.
 * RETURNS:
 *   (INT) - array of shaders size.
 */
INT EK3_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Shader update function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndShdUpdate( VOID );

/***
  Textures handle functions
***/

/* Add texture image to texture array function.
 * ARGUMENTS:
 *  - CHAR *Name - texture name.
 *  - INT W, H - texture size.
 *  - VOID *Bits - image.
 * RETURNS:
 *   (INT) - array size, -1 if array is full.
 */
INT EK3_RndTextureAddImg( CHAR *Name, INT W, INT H, VOID *ImageData );

/* Add texture to texture array from .G24 file function.
 * ARGUMENTS:
 *  - CHAR *FileName - texture file name.
 *  - CHAR *Name - texture name.
 * RETURNS:
 *   (INT) - array size, -1 if program didnt work.
 */
INT EK3_RndTextureAddFromFileG24( CHAR *FileName, CHAR *Name );

/* Texture initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndTexInit( VOID );

/* Texture deinitialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndTexClose( VOID );

/***
  Materials handle functions
***/

/* Material applying function.
 * ARGUMENTS:
 *  - MtlNo - Material array position.
 * RETURNS:
 *   (UINT) - programm ID.
 */
UINT EK3_RndMtlApply( INT MtlNo );

/* Material initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndMtlInit( VOID );

/* Material initialization function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   None.
 */
VOID EK3_RndMtlClose( VOID );

/* Material adding in array function.
 * ARGUMENTS:
 *  - ek3MATERIAL Mtl - material struct to add in array.
 * RETURNS:
 *   (INT) - array of materials size.
 */
INT EK3_RndMtlAdd( ek3MATERIAL Mtl );

/* Resources function headers */
VOID EK3_RndResInit( VOID );
VOID EK3_RndResClose( VOID );

extern ek3SHADER EK3_RndShaders[EK3_MAX_SHADERS];       /* Array of shaders */
extern INT EK3_RndShadersSize;                          /* Size of shaders array */
extern ek3TEXTURE EK3_RndTextures[EK3_MAX_TEXTURES];    /* Array of textures */
extern INT EK3_RndTexturesSize;                         /* Size of textures array */
extern ek3MATERIAL EK3_RndMaterials[EK3_MAX_MATERIALS]; /* Array of materials */
extern INT EK3_RndMaterialsSize;                        /* Size of materials array */

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */