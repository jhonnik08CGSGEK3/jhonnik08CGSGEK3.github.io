/* FILE NAME  : rndtrg.c
 * PURPOSE    : 3D animation project.
 *              Rendering system target handle module.
 * PROGRAMMER : EK3
 * LAST UPDATE: 27.02.2023
 */

#include "rnd.h"

/*** Target semiglobal data ***/

/* Number of rendertarget attachments */
#define EK3_RND_TARGETS 2

/* Target frame buffer Id */
static UINT EK3_RndTargetFBO;

/* Reference to render target attachment textures */
static INT EK3_RndTargetTexNo[EK3_RND_TARGETS];

/* Depth buffer render buffer Id */
static INT EK3_RndTargetRndBuf;

/*
static INT EK3_RndTargetShdNo;
static INT EK3_RndTargetMtlNo;
*/

/* Full screen render target primitive */
static ek3PRIM EK3_RndTargetScreenPrim;

/* Render target create frame buffer function.
 * ARGUMENTS:
 *   - frame buffer size (in pixels):
 *       INT W, H;
 * RETURNS: None.
 */
static VOID EK3_RndTargetCreate( INT W, INT H )
{
  INT i, status;
  INT DrawBuffer[EK3_RND_TARGETS];
  ek3TEXTURE *Tex = &EK3_RndTextures[EK3_RndMaterials[EK3_RndTargetScreenPrim.MtlNo].Tex[0]];

  /* Create FBO */
  glGenFramebuffers(1, &EK3_RndTargetFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, EK3_RndTargetFBO);

  /* Attach textures */
  for (i = 0; i < EK3_RND_TARGETS; i++)
  {
    /* Resize target texture */
    glDeleteTextures(1, &Tex[i].TexId);
    glGenTextures(1, &Tex[i].TexId);
    glBindTexture(GL_TEXTURE_2D, Tex[i].TexId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, W, H);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Tex[i].TexId, 0);
    DrawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(EK3_RND_TARGETS, DrawBuffer);

  /* Attach render buffer (depth) */
  glGenRenderbuffers(1, &EK3_RndTargetRndBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, EK3_RndTargetRndBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, W, H);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, EK3_RndTargetRndBuf);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    OutputDebugString("\nError create frame buffer\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} /* End of 'EK3_RndTargetCreate' function */

/* Render target free frame buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID EK3_RndTargetFree( VOID )
{
  INT i;

  glBindFramebuffer(GL_FRAMEBUFFER, EK3_RndTargetFBO);
  /* Detach textures */
  for (i = 0; i < EK3_RND_TARGETS; i++)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
  /* Detach render buffer (depth) */
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  glDeleteRenderbuffers(1, &EK3_RndTargetRndBuf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &EK3_RndTargetFBO);
  EK3_RndTargetFBO = 0;
  EK3_RndTargetRndBuf = 0;
} /* End of 'EK3_RndTargetFree' function */

/* Render target initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetInit( VOID )
{
  ek3MATERIAL mtl = EK3_RndMtlGetDef();
  INT i;
  CHAR Name[] = "Target0";

  for (i = 0; i < EK3_RND_TARGETS; i++)
  {
    Name[6] = '0' + i;
    mtl.Tex[i] = EK3_RndTexAddFmt(Name, EK3_RndFrameW, EK3_RndFrameH, GL_RGBA32F);
  }
  EK3_RndPrimCreate(&EK3_RndTargetScreenPrim, EK3_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  strncpy(mtl.Name, "Target material", EK3_STR_MAX);
  mtl.ShdNo = EK3_RndShdAdd("target0");
  EK3_RndTargetScreenPrim.MtlNo = EK3_RndMtlAdd(&mtl);
  EK3_RndTargetCreate(EK3_RndFrameW, EK3_RndFrameH);
} /* End of 'EK3_RndTargetInit' function */

/* Render target deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetClose( VOID )
{
  EK3_RndTargetFree();
  EK3_RndPrimFree(&EK3_RndTargetScreenPrim);
} /* End of 'EK3_RndTargetClose' function */

/* Render target resize function.
 * ARGUMENTS:
 *   - new render target size (in pixels):
 *       INT W, H;
 * RETURNS: None.
 */
VOID EK3_RndTargetResize( INT W, INT H )
{
  EK3_RndTargetFree();
  EK3_RndTargetCreate(W, H);
  glViewport(0, 0, W, H);
} /* End of 'EK3_RndTargetResize' function */

/* Start render target usage function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetStart( VOID )
{
  INT i;
  FLT dv = 1;
  VEC4 Col0 = {0.30, 0.47, 0.8, 1};

  glBindFramebuffer(GL_FRAMEBUFFER, EK3_RndTargetFBO);
  for (i = 0; i < EK3_RND_TARGETS; i++)
    glClearBufferfv(GL_COLOR, i, &Col0.X);
  glClearBufferfv(GL_DEPTH, 0, &dv);
  glViewport(0, 0, EK3_RndFrameW, EK3_RndFrameH);
} /* End of 'EK3_RndTargetStart' function */

/* Finalize render target output function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EK3_RndTargetEnd( VOID )
{
  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, EK3_RndFrameW, EK3_RndFrameH);
  glDisable(GL_DEPTH_TEST);
  EK3_RndPrimDraw(&EK3_RndTargetScreenPrim, MatrIdentity());
  glEnable(GL_DEPTH_TEST);
  glFinish();
} /* End of 'EK3_RndTargetEnd' function */

/* END OF 'rndtrg.c' FILE */