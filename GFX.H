/* Ivan Pashkov, 10-5, 03.11.2023, IP5 */
#include <commondf.h>

extern INT GFX_W, GFX_H;

/* GFX.C */
VOID GFX_PutPixel( INT X, INT Y, DWORD rgb );
VOID GFX_DrawFrame( VOID );
VOID GFX_Clear( DWORD rgb );
VOID GFX_Init( VOID );
VOID GFX_Circle( INT X, INT Y, INT R, DWORD rgb );
VOID GFX_FrameSetSize(INT W, INT H);
VOID GFX_ClearBlack( VOID );
VOID GFX_PrintTime( VOID );

/* FONT */
VOID GFX_DrawLetters( INT X, INT Y, CHAR *str, DWORD rgb );
VOID GFX_DrawLetter( INT X, INT Y, CHAR ch, DWORD rgb );
VOID GFX_LoadFont( VOID );