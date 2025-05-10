/**/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <glut.h>
#include <math.h>
#include <commondf.h>

#include "LAB.H"
#include "GFX.H"

#define MAX          50
#define MAX_ENEMIES  8

VEC Ava;
static INT NumOfEnemies;
static ENEMY Enemies[MAX_ENEMIES];
static INT Cab[MAX][MAX];
static INT Solution[MAX][MAX];

extern INT DeltaTime;
extern HWND hWnd;

static BOOL LabLoad( VOID )
{
  FILE *F;
  INT x = 0, y = 0, c;
  BOOL run = TRUE;

  if ((F = fopen("LAB1.LB", "r")) == NULL)
    return FALSE;

  NumOfEnemies = 0;

  while (run)
  {
    switch (c = fgetc(F))
    {
    case EOF:
      run = FALSE;
      break;
    case 'a':
      if (y < MAX && x < MAX)
        Cab[y][x] = 2;
      Ava.X = x;
      Ava.Y = y;
      x++;
      break;
    case 'e':
      if (y < MAX && x < MAX)
      {
        if (NumOfEnemies < MAX_ENEMIES)
        {
          Cab[y][x] = 3;
          Enemies[NumOfEnemies].Pos.X = x;
          Enemies[NumOfEnemies].Pos.Y = y;
          Enemies[NumOfEnemies].DeltaTime = (DOUBLE)(rand() % 10 + 1) / 3;
          Enemies[NumOfEnemies].LatestTime = 0.0;
          NumOfEnemies++;
        }
      }
      x++;
      break;
    case '\n':
      y++;
      x = 0;
      break;
    case ' ':
      ;
    case '*':
      ;
    default:
      if (y < MAX && x < MAX)
        Cab[y][x] = c == '*' ? 1 : 0;
      x++;
      break;
    }
  }

  return TRUE;
}

VOID LabInit( VOID )
{
  if (!LabLoad())
    printf("FAIL!"), getchar(), exit(0);
}

VOID LabDraw( VOID )
{
  INT i, j;

  for (i = 0; i < MAX - 20; i++)
    for (j = 0; j < MAX; j++)
      if (Cab[i][j] == 2)
        GFX_PutPixel(j, i, 0xFF00FF00);
      else if (Cab[i][j] == 3)
        GFX_PutPixel(j, i, 0xFFFF0000);
      else
        GFX_PutPixel(j, i, Cab[i][j] == 1 ? 0x7FFFFFFF : 0x3F000000);
}

static VOID CreateSolution( VOID )
{
  INT i, j, k;

  memset(Solution, -1, 4 * MAX * MAX);

  Solution[Ava.Y][Ava.X] = 0;

  for (k = 0; k < MAX * MAX; k++)
    for (i = 0; i < MAX; i++)
      for (j = 0; j < MAX; j++)
        if (Solution[j][i] < 0 && Cab[j][i] != 3 && Cab[j][i] != 1)
        {
          if (i < MAX - 1 && Solution[j][i + 1] >= 0)
            Solution[j][i] = Solution[j][i + 1] + 1;
          if (j < MAX - 1 && Solution[j + 1][i] >= 0)
            Solution[j][i] = Solution[j + 1][i] + 1;
          if (i > 0 &&       Solution[j][i - 1] >= 0)
            Solution[j][i] = Solution[j][i - 1] + 1;
          if (j > 0 &&       Solution[j - 1][i] >= 0)
            Solution[j][i] = Solution[j - 1][i] + 1;
        }
  /*for (i = 0; i < MAX; i++)
  {
    for (j = 0; j < MAX; j++)
      printf("%2d ", Solution[i][j]);
    printf("\n");
  }
  printf("\n\n"); */
}
static VOID AIStep( VOID )
{
  INT i;

  for (i = 0; i < NumOfEnemies; i++)
  { 
    if (clock() / (double)CLOCKS_PER_SEC - Enemies[i].LatestTime > Enemies[i].DeltaTime)
    {
      INT min = -1;

      CreateSolution();

      Enemies[i].LatestTime = clock() / (double)CLOCKS_PER_SEC;

      /*printf("%2d, %2d\n", Enemies[i].Pos.Y, Enemies[i].Pos.X);
       */
      if (Enemies[i].Pos.X < MAX  - 1 && 
          Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1] >= 0 && 
         (min == -1 || min > Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1]))
      {
        min = Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1];
        /* printf("%2d/1 ", min); */
      }
      if (Enemies[i].Pos.Y < MAX  - 1 && 
          Solution[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X] >= 0 && 
         (min == -1 || min > Solution[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X]))
      {
        min = Solution[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X];
        /* printf("%2d/2 ", min); */
      }
      if (Enemies[i].Pos.X > 0 && 
          Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1] >= 0 && 
         (min == -1 || min > Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1]))
      {
        min = Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1];
        /* printf("%2d/3 ", min); */
      }
      if (Enemies[i].Pos.Y > 0 && 
          Solution[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X] >= 0 && 
         (min == -1 || min > Solution[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X]))
      {
        min = Solution[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X];
        /* printf("%2d/4 ", min); */
      }
      

      /* printf("\n");  */
      /* Direct */
      if (min == Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1] && Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1] != 1 && Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X + 1] >= 0)
      {
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 0;
        Enemies[i].Pos.X++;
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 3;
        /* printf("1!\n %d %d\n", Enemies[i].Pos.Y, Enemies[i].Pos.X); */
      }
      else if (min == Solution[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X] && Cab[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X] != 1 && Solution[Enemies[i].Pos.Y + 1][Enemies[i].Pos.X] >= 0)
      {
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 0;
        Enemies[i].Pos.Y++;
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 3;
        /* printf("2!\n %d %d\n", Enemies[i].Pos.Y, Enemies[i].Pos.X); */
      }
      else if (min == Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1] && Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1] != 1 && Solution[Enemies[i].Pos.Y][Enemies[i].Pos.X - 1] >= 0)
      {
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 0;
        Enemies[i].Pos.X--;
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 3;
        //printf("3!\n %d %d\n", Enemies[i].Pos.Y, Enemies[i].Pos.X);
      }
      else if (min == Solution[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X] && Cab[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X] != 1 && Solution[Enemies[i].Pos.Y - 1][Enemies[i].Pos.X] >= 0)
      {
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 0;
        Enemies[i].Pos.Y--;
        Cab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 3;
        //printf("4!\n %d %d\n", Enemies[i].Pos.Y, Enemies[i].Pos.X);
      }
    }
  }
}

static BOOL CheckDeath( VOID )
{
  INT i;

  for (i = 0; i < NumOfEnemies; i++)
    if (Ava.X == Enemies[i].Pos.X && 
        Ava.Y == Enemies[i].Pos.Y)
        return TRUE;

  return FALSE;
}

VOID LabStep( VOID )
{
  /*AIStep();

  if (CheckDeath())
  {
    LabInit();
    return;
  }*/

  LabDraw();
}

VOID LabKeyboard( BYTE Key )
{
  /*
  printf("%d,%d,%d,%c\n", Ava.X, Ava.Y, Cab[Ava.Y][Ava.X], Key);
  */
    if ((Key == 'w' || Key == 'W') && Ava.Y > 0 && (!Cab[Ava.Y - 1][Ava.X] || Cab[Ava.Y - 1][Ava.X] == 3))
    {
        Cab[Ava.Y][Ava.X] = 0;
        Ava.Y--;

        if (CheckDeath())
        {

            MessageBox(hWnd, L"You are loose! LOOSER! :)", L"CGSG IP5 :: LAB3D", MB_ICONERROR | MB_OKCANCEL);
            LabInit();
            return;
        }

    Cab[Ava.Y][Ava.X] = 2;
  }
  if ((Key == 'a' || Key == 'A') && Ava.X > 0 && (!Cab[Ava.Y][Ava.X - 1] || Cab[Ava.Y][Ava.X - 1] == 3))
  {
    Cab[Ava.Y][Ava.X] = 0;
    Ava.X--;

    if (CheckDeath())
    {

        MessageBox(hWnd, L"You are loose! LOOSER! :)", L"CGSG IP5 :: LAB3D", MB_ICONERROR | MB_OKCANCEL);
        LabInit();
        return;
    }

    Cab[Ava.Y][Ava.X] = 2;
  }
  if ((Key == 's' || Key == 'S') && Ava.Y < MAX - 1 && (!Cab[Ava.Y + 1][Ava.X] || Cab[Ava.Y + 1][Ava.X] == 3))
  {
    Cab[Ava.Y][Ava.X] = 0;
    Ava.Y++;

    if (CheckDeath())
    {

        MessageBox(hWnd, L"You are loose! LOOSER! :)", L"CGSG IP5 :: LAB3D", MB_ICONERROR | MB_OKCANCEL);
        LabInit();
        return;
    }

    Cab[Ava.Y][Ava.X] = 2;
  }
  if ((Key == 'd' || Key == 'D') && Ava.X < MAX - 1 && (!Cab[Ava.Y][Ava.X + 1] || Cab[Ava.Y][Ava.X + 1] == 3))
  {
    Cab[Ava.Y][Ava.X] = 0;
    Ava.X++;

    if (CheckDeath())
    {

        MessageBox(hWnd, L"You are loose! LOOSER! :)", L"CGSG IP5 :: LAB3D", MB_ICONERROR | MB_OKCANCEL);
        LabInit();
        return;
    }

    Cab[Ava.Y][Ava.X] = 2;
  }
  if (Key == 'v' || Key == 'V')
      if (Cab[15][18] == 1)
          Cab[15][18] = 0;
      else if (Cab[15][18] == 0)
          Cab[15][18] = 1;
}

static VOID LabDraw3D(VOID)
{
    INT i, j;

    for (i = 0; i < MAX - 20; i++)
        for (j = 0; j < MAX; j++)
        {
            glPushMatrix();
            if (Cab[i][j] == 2)
            {
                glTranslated(j, fabs(sin(clock() / (double)CLOCKS_PER_SEC * 5)) * 2 + 0.5, i);
                glColor3d(0, 1, 0);
                glutSolidSphere(0.5, 30, 30);
                glPopMatrix();

                glPushMatrix();
                glColor3d(1, 1, 0);
                glScaled(1, 0.1, 1);
                glTranslated(j, -0.5, i);
                glutSolidCube(1.0);
            }
            else if (Cab[i][j] == 3)
            {
                glTranslated(j, 0.5, i);
                glColor3d(1, 0, 0); 
                glutSolidCube(1.0);
            }

            if (Cab[i][j] == 0)
            {
                glColor3d(1, 1, 0);
                glScaled(1, 0.1, 1);
                glTranslated(j, -0.5, i);
                glutSolidCube(1.0);
            }
            else if (Cab[i][j] == 1)
            {
                glTranslated(j, 0.5, i);
                glColor3d(1, 0.5, 0);
                glutSolidCube(1.0);
            }
            glPopMatrix();
        }
}

VOID LabStep3D(VOID)
{
    AIStep();

    if (CheckDeath())
    {
        MessageBox(hWnd, L"You are loose! LOOSER! :)", L"CGSG IP5 :: LAB3D", MB_ICONERROR | MB_OKCANCEL);
        LabInit();
        return;
    }

    LabDraw3D();
}