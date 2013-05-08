#include<allegro.h>
#include<iostream>
#include<math.h>
#include<conio.h>
#include<time.h>
#include<sstream>
#include<cstring>
#include<string>


#ifndef Customfunctions
#define Customfunctions
bool Introtogame(BITMAP *buffer);

namespace GAMECLOCK
{
          double TimePassed(clock_t tStart,clock_t tEnd);
};

void WINGAME(BITMAP *buffer,BITMAP *BackroundImage );

namespace Collisions
{
bool Collision(int x, int y, int x2, int y2,int hight,int wight);
bool PlatformCollision(int x, int y, int x2, int y2,int hight1,int wight1,int hight2,int wight2);
};

void statusbar(BITMAP *buffer,int Warp,int score,int x,int y,int playerslife,int level);

namespace GravityWarp{
          int GravityWarpY(int TypeofPower,int mousey, int y,int warpeffecty,int gravity, int Warp);
          int GravityWarpX(int TypeofPower,int mousex, int x,int wrapeffectx,int xspeed,int Warp);
};

#endif
