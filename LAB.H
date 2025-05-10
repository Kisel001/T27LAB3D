#include <commondf.h>

typedef double DBL;

typedef struct tagVEC
{
  INT X;
  INT Y;
} VEC;	

typedef struct tagENEMY
{
  VEC Pos;
  DBL DeltaTime;
  DBL LatestTime;
} ENEMY;

VOID LabInit( VOID );
VOID LabKeyboard( BYTE Key );
VOID LabStep( VOID );
VOID LabStep3D( VOID );