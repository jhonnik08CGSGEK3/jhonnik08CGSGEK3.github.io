/* FILE NAME: rndres.h
 * PURPOSE: Resources functions prototypes and parameters.
 * PROGRAMMER: EK3
 * DATE: 27.02.2023
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

/* Image representation type */
typedef struct tagek3IMAGE
{
  INT W, H;      /* Image size */
  DWORD *Pixels; /* Image pixels */
} ek3IMAGE;

/***
  Shaders handle functions
***/

extern VEC EK3_RndShdAddonV[8];
extern FLT EK3_RndShdAddonF[8];

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
INT EK3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

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
INT EK3_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType );

/* Add texture to texture array from .G24 file function.
 * ARGUMENTS:
 *  - CHAR *FileName - texture file name.
 *  - CHAR *Name - texture name.
 * RETURNS:
 *   (INT) - array size, -1 if program didnt work.
 */
INT EK3_RndTexAddFromFile( CHAR *FileName, CHAR *Name );

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

/* Default material creation function.
 * ARGUMENTS:
 *  - None.
 * RETURNS:
 *   (ek3MATERIAL) - default material struct.
 */
ek3MATERIAL EK3_RndMtlGetDef( VOID );

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
INT EK3_RndMtlAdd( ek3MATERIAL *Mtl );

/* Material getting from array function.
 * ARGUMENTS:
 *  - INT MtlNo - material number in array
 * RETURNS:
 *   (ek3MATERIAL) - material struct.
 */
ek3MATERIAL EK3_RndMtlGet( INT MtlNo );

/***
 * Image handle
 ***/

/* Create image function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 *   - new image size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL EK3_ImgCreate( ek3IMAGE *Img, INT W, INT H );

/* Free image function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 * RETURNS: None.
 */
VOID EK3_ImgFree( ek3IMAGE *Img );

/* Load image from file function.
 * ARGUMENTS:
 *   - image data:
 *       ek3IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL EK3_ImgLoad( ek3IMAGE *Img, CHAR *FileName );

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