/* FILE NAME: rndres.h
 * PURPOSE: Resources functions prototypes and parameters.
 * PROGRAMMER: EK3
 * DATE: 06.02.2023
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "anim/rnd/rnd.h"

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

/* Shader functions headers */
static CHAR * EK3_RndShdLoadTextFromFile( CHAR *FileName );
static VOID EK3_RndShdLog( CHAR *FileNamePrefix, CHAR *Part, CHAR *Text );

INT EK3_RndShdLoad( CHAR *FileNamePrefix );
VOID EK3_RndShdFree( INT Prg );

VOID EK3_RndShdInit( VOID );
VOID EK3_RndShdClose( VOID );
INT EK3_RndShdAdd( CHAR *ShaderFileNamePrefix );
VOID EK3_RndShdUpdate( VOID );

/* Texture functions headers */
INT EK3_RndTextureAddImg( CHAR *Name, INT W, INT H, VOID *ImageData );
INT EK3_RndTextureAddFromFileG24( CHAR *FileName, CHAR *Name );
VOID EK3_RndTexInit( VOID );
VOID EK3_RndTexClose( VOID );

/* Material functions headers */
UINT EK3_RndMtlApply( INT MtlNo );
VOID EK3_RndMtlInit( VOID );
VOID EK3_RndMtlClose( VOID );
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