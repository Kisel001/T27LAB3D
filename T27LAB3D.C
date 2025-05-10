#define _CRT_SECURE_NO_WARNINGS


/* Ivan Pashkov, 10-5, 02.11.2023, IP5 */
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <time.h>
#include <commondf.h>

#include "GFX.H"
#include "LAB.H"

HWND hWnd; 
INT Zoom = 3;
DOUBLE SyncTime, FPS, DeltaTime;

extern VEC Ava;
extern DWORD GFX_Frame;
 
static VOID Reshape( INT NewW, INT NewH )
{
  DOUBLE size = 0.1, wp = size, hp = size;
  INT WinW, WinH;
 
  if (NewW > NewH)
    wp *= (DOUBLE)NewW / NewH;
  else
    hp *= (DOUBLE)NewH / NewW;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-wp / 2, wp / 2, -hp / 2, hp / 2, size, 3000);
  glMatrixMode(GL_MODELVIEW);
 
  WinW = NewW;
  WinH = NewH;
  GFX_FrameSetSize(GFX_W, GFX_H);
 
  glViewport(0, 0, NewW, NewH);
}

static VOID Timer( VOID )
{
  CHAR Buf[100];
  LONG t;
  static LONG StartTime = -1, FPSTime, FrameCount = 0, PauseTime = 0, OldTime;

  t = clock();
  if (StartTime == -1)
      StartTime = FPSTime = OldTime = t;
  
  DeltaTime = (t - OldTime) / (DOUBLE)CLOCKS_PER_SEC;
  SyncTime = (t - PauseTime - StartTime) / (DOUBLE)CLOCKS_PER_SEC;
  

  FrameCount++;
  if (t - FPSTime > 0.3 * CLOCKS_PER_SEC)
  {
    FPS = FrameCount / ((t - FPSTime) / (DOUBLE)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
    
    sprintf(Buf, "CGSG IP5 :: LAB3D GAME FPS: %.3f", FPS);
    SetWindowTextA(hWnd, Buf);
  }
  OldTime = t;
}

static VOID Display( VOID )
{
  static BOOL flag = 1;
  static DOUBLE x, y;

  /**/
  if (flag)
  {
      flag = 0;
      x = Ava.X - 3;
      y = Ava.Y - 3;
  }

  Timer();

  x += DeltaTime * (Ava.X - x);
  y += DeltaTime * (Ava.Y - y);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  /* 3D */
  glPushMatrix();
  glLoadIdentity();

  gluLookAt(x + 10, 25, y + 10, x, 1, y, 0, 1, 0);
  LabStep3D();

  glPopMatrix();
 
  /* 2D */
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  GFX_ClearBlack();
  LabStep();
  GFX_PrintTime();
  GFX_DrawFrame(); 

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

static VOID Keyboard( BYTE Key, INT x, INT y )
{
  int OldZoom;

  LabKeyboard(Key);

  /* EXIT FROM PROGRAMM IF KEY = ESC */
  if (Key == 27)
    exit(0);

  OldZoom = Zoom;
  if ((Key == '='  || Key == '+') && Zoom < 13)
    Zoom++, GFX_FrameSetSize(GFX_W, GFX_H);
  if ((Key == '_'  || Key == '-') && Zoom > 1)
    Zoom--, GFX_FrameSetSize(GFX_W, GFX_H);
}

VOID Special( INT X, INT Y, INT Z )
{
}

/*
VOID Reshape( INT NewW, INT NewH )
{
  INT WinW = NewW;
  INT WinH = NewH;

  GFX_FrameSetSize(WinW / Zoom, WinH / Zoom);
} */

VOID main( INT argc, CHAR *argv[] )
{
  /* Initialization */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  /* Create Window */
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(800, 800);
  glutCreateWindow("CGSG IP5 :: LAB3D GAME");
  hWnd = FindWindowA(NULL, "CGSG IP5 :: LAB3D GAME");

  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Base Draw */
  GFX_Init();
  LabInit();
  GFX_LoadFont();

  /* */
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(Special);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.30, 0.47, 0.8, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
      
  /* Cycle */
  glutMainLoop();
}