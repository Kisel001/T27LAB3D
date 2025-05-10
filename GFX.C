#define _CRT_SECURE_NO_WARNINGS

/* Ivan Pashkov, 10-5, 01.11.2023, IP5 */
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <windows.h>
#include <commondf.h>
#include "GFX.H"

#define GFX_FRAME_W 130
#define GFX_FRAME_H 33

INT GFX_W = GFX_FRAME_W, GFX_H = GFX_FRAME_H;

DWORD *GFX_Frame;
static CHAR GFX_Font[4096];

extern INT Zoom;

VOID GFX_PutPixel( INT X, INT Y, DWORD rgb )
{
  if (X < 0 || Y < 0 || X >= GFX_W || Y >= GFX_H)
    return;
  
  GFX_Frame[GFX_W * Y + X] = rgb;
}

VOID GFX_Circle( INT X, INT Y, INT R, DWORD rgb )
{
  INT i, j, d = 2 * R + 1, R2 = R * R;

  for (j = 0; j < d; j++)
    for (i = 0; i < d; i++)
      if ((i - R) * (i - R) + (j - R) * (j - R) <= R2)
        GFX_PutPixel(i + X - R, j + Y - R, rgb);
}

VOID GFX_Init( VOID )
{
  GFX_Frame = malloc(GFX_W * GFX_H * sizeof(DWORD));
  GFX_ClearBlack();
}

VOID GFX_DrawFrame( VOID )
{
  glPixelZoom(Zoom, -Zoom);
  glRasterPos2d(-1, 1);
  glDrawPixels(GFX_W, GFX_H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, GFX_Frame);
}

/* Clear Real Window Func */
VOID GFX_Clear( DWORD rgb )
{
  INT i, j;

  for (i = 0; i < GFX_W; i++)
    for (j = 0; j < GFX_H; j++)
      GFX_PutPixel(i, j, rgb);
}

VOID GFX_ClearBlack( VOID )
{
  memset(GFX_Frame, 0x00, GFX_W * GFX_H * 4);
}

VOID GFX_LoadFont( VOID )
{
  FILE *F;
  INT i = 0;

  F = fopen("MyFont.FNT", "rb");

  if (F == NULL)
    return;
  
  fread(GFX_Font, 16, 256, F);
  fclose(F);
}

VOID GFX_DrawLetter( INT X, INT Y, CHAR ch, DWORD rgb )
{
  INT i, j;

  for (i = 0; i < 16; i++)
    for (j = 0; j < 8; j++)
      if ((GFX_Font[16 * (BYTE)ch + i] & (0x80 >> j)) != 0)
        GFX_PutPixel(X + j, Y + i, rgb);
}

VOID GFX_DrawLetters( INT X, INT Y, CHAR *str, DWORD rgb )
{
  INT i = 0, save = X;

  while (str[i] != 0)
  {
    if (str[i] == (BYTE)'\n')
      Y += 16, X = save;
    else
      GFX_DrawLetter(X , Y, str[i], rgb), X += 8;
    i++;
  }
}

VOID GFX_PrintTime( VOID )
{
  SYSTEMTIME TIME;
  char Buf[100];

  GetLocalTime(&TIME);

  sprintf(Buf, "%.02d:%.02d:%.02d\n%.02d.%.02d.%d", TIME.wHour, TIME.wMinute, TIME.wSecond, TIME.wDay, TIME.wMonth, TIME.wYear);
  GFX_DrawLetters(51, 0, Buf, 0x7FFF00FF);
} 

VOID GFX_FrameSetSize( INT W, INT H )
{
  if (GFX_Frame != NULL)
    free(GFX_Frame);

  GFX_W = W;
  GFX_H = H;

  GFX_Frame = malloc(4 * GFX_W * GFX_H);
  
  if (GFX_Frame == NULL)
      return;
  
  memset(GFX_Frame, 0, 4 * GFX_W * GFX_H);
}