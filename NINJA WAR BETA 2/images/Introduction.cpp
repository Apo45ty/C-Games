#include "Customfunctions.h"

bool Introtogame(BITMAP *buffer){
     using namespace Collisions;
     //mouse_x;
     //mouse_y;
     //show_mouse(buffer);
     //mouse_b & 1 left
     //mouse_b & 2 right
     //load controls
    BITMAP *Controls1= load_bitmap("images/Controls-1.bmp",NULL);
    BITMAP *Controls2= load_bitmap("images/Controls-2.bmp",NULL);
    if(!Controls1||!Controls2){
        destroy_bitmap(Controls1);
        destroy_bitmap(Controls2);
        allegro_message("Error Could not find image");
        return true;} 
    
     BITMAP *BackgroundImage = load_bitmap("images/MAINMENU.bmp",NULL);
     BITMAP *Icons = load_bitmap("images/menu(2).bmp",NULL); 
     BITMAP *Cursor = load_bitmap("images/Sun.bmp",NULL); 
     if(!BackgroundImage||!Icons||!Cursor )
    {
    destroy_bitmap(BackgroundImage);
    destroy_bitmap(Icons);
    destroy_bitmap(Cursor); 
    allegro_message("Error Could not find image");
    }
     int menuxpossition=20,menuypossition=120;
     bool StartGame=false,Close=false;
     while(!StartGame&&!key[KEY_ESC]&&!Close)
     {
      masked_blit(BackgroundImage,buffer,0,0,0,0,640,480);  
      masked_blit(Icons,buffer,0,120,menuxpossition,menuypossition,120,20);
      masked_blit(Icons,buffer,0,40,menuxpossition,menuypossition+40,120,20);
      masked_blit(Icons,buffer,0,80,menuxpossition,menuypossition+80,120,20);
      masked_blit(Icons,buffer,0,0,menuxpossition,menuypossition+120,120,20);
      masked_blit(Icons,buffer,0,160,menuxpossition,menuypossition+160,120,20);
      //draw mouse and everything mouse related
      if( Collision( mouse_x, mouse_y, menuxpossition, menuypossition,20,120) ){
           textprintf(buffer,font,300,120,makecol(255,255,255),"Click to Start A New Game"); 
           if(mouse_b & 1 ) 
           StartGame=true;  
      }else
      if( Collision( mouse_x, mouse_y, menuxpossition, menuypossition+40,20,120) ){
           textprintf(buffer,font,300,120,makecol(255,255,255),"Click to View Credits");  
      }else
      if( Collision( mouse_x, mouse_y, menuxpossition, menuypossition+80,20,120) ){
           textprintf(buffer,font,300,120,makecol(255,255,255),"Click to View Highscores");  
      }else
      if( Collision( mouse_x, mouse_y, menuxpossition, menuypossition+120,20,120) ){
           textprintf(buffer,font,300,120,makecol(255,255,255),"CLICK HERE IMPORTANT");
           if(mouse_b & 1 ){
           do{
           blit(Controls1,buffer,0,0,0,0,640,480);
           blit(buffer,screen,0,0,0,0,640,480); 
           clear_bitmap(buffer);}while(!key[KEY_RIGHT]); 
           do{ 
           blit(Controls2,buffer,0,0,0,0,640,480);
           blit(buffer,screen,0,0,0,0,640,480); 
           clear_bitmap(buffer); }while(!key[KEY_SPACE]); 
           }
           
      }else
      if( Collision( mouse_x, mouse_y, menuxpossition, menuypossition+160,20,120) ){
           textprintf(buffer,font,300,120,makecol(255,255,255),"Click to Exit Game");
           if(mouse_b & 1 )
           return true;  
      };
      
      //(buffer,mouse_x,mouse_y,mouse_x+5,mouse_y+5,makecol(255,255,255));
     masked_blit(Cursor,buffer,0,0,mouse_x,mouse_y,32,32);
     //draw to screen
      blit(buffer,screen,0,0,0,0,640,480);  
      clear_bitmap(buffer);
      };
      destroy_bitmap(BackgroundImage);
      destroy_bitmap(Icons);
      destroy_bitmap(Cursor); 
      destroy_bitmap(Controls1);
      destroy_bitmap(Controls2);  
      return Close;  
};//end of intro
namespace Collisions
{
bool Collision(int x, int y, int x2, int y2,int hight,int wight){
     if(x+wight<x2||y+hight<y2||x>x2+wight||y>y2+hight)
      return false;
      else{
      return true;
      }
};///colision funtion
bool PlatformCollision(int x, int y, int x2, int y2,int hight1,int wight1,int hight2,int wight2){
     if(x+wight1<x2||y+hight1<y2||x>x2+wight2||y>y2+hight2)
      return false;
      else{
      return true;
      }
};///colision funtion
};//end of collisions

namespace GAMECLOCK
{
          double TimePassed(clock_t tStart,clock_t tEnd)
          {
          return ((double)(tEnd - tStart)/CLOCKS_PER_SEC);
          };
};//end of gameclock

void WINGAME(BITMAP *buffer,BITMAP *BackroundImage ){
allegro_message("YAY you beat the demo !!! sorry but have not made any new porn sprites, enjoy this bonus prees escape to end");
int velx=0,vely=0,x=0,y=0;

while(!key[KEY_ESC])
{
        if(key[KEY_UP]&&y>0)
           vely=-5;
        else
        if(key[KEY_DOWN]&&y<1440)
           vely=5;
        else
        vely=0;
           
        if(key[KEY_LEFT]&&x>0)
           velx=-5;
        else 
        if(key[KEY_RIGHT]&&x<1280)
           velx=5;
        else
        velx=0;
                      
                      
      x+=velx;
      y+=vely;     
      masked_blit(BackroundImage,buffer,x,y,0,0,640,480);
      blit(buffer,screen,0,0,0,0,640,480);  
      clear_bitmap(buffer); 
};
};

void statusbar(BITMAP *buffer,int Warp,int score,int x,int y,int playerslife,int level){
        textprintf(buffer,font,120,0,makecol(255,255,255),"Warp=%d",Warp);
        textprintf(buffer,font,0,0,makecol(255,255,255),"Score=%d",score);
        textprintf(buffer,font,500,20,makecol(255,255,255),"x=%d  y=%d",x,y);
        textprintf(buffer,font,300,0,makecol(255,255,255),"Life=%d",playerslife);
        textprintf(buffer,font,400,0,makecol(255,255,255),"Level=%d",level);
};
namespace GravityWarp{
          int WarpPowerFactor=50;
          int GravityWarpY(int TypeofPower,int mousey, int y,int warpeffecty,int gravity, int Warp){
          ////left button
          if(TypeofPower== 1 && mousey<y){
                           return -warpeffecty-warpeffecty*(Warp/WarpPowerFactor);
          }else
          if(TypeofPower== 1 && mousey>y){
                           return warpeffecty+warpeffecty*(Warp/WarpPowerFactor);
          }else 
          //repelwhitmouse right button
          if(TypeofPower== 2 && mouse_y<y){
                           return warpeffecty+warpeffecty*(Warp/WarpPowerFactor);
          }else
          if(TypeofPower== 2 && mouse_y>y){
                           return -warpeffecty-warpeffecty*(Warp/WarpPowerFactor);
          }else if(TypeofPower== 3)
                           return -gravity-warpeffecty*(Warp/WarpPowerFactor);
          else
               return gravity;};
          
          int GravityWarpX(int TypeofPower,int mousex, int x,int wrapeffectx,int xspeed,int Warp){        
          if(TypeofPower== 1 &&  mouse_x<x)
                           return -wrapeffectx-wrapeffectx*(Warp/WarpPowerFactor);
          else if(TypeofPower== 1 &&  mouse_x>x)
               return wrapeffectx+wrapeffectx*(Warp/WarpPowerFactor);
          //repelwhitmouse right button
          else if(TypeofPower== 2 && mouse_x<x)
               return wrapeffectx+wrapeffectx*(Warp/WarpPowerFactor);
          else if(TypeofPower== 2 &&  mouse_x>x)
               return -wrapeffectx-wrapeffectx*(Warp/WarpPowerFactor);
          else if(TypeofPower== 3)
               return -xspeed-wrapeffectx*(Warp/WarpPowerFactor);
          else
               return xspeed;}; 
};

















//garbagecode
/*
class Characters
{
  private:
        //funciones para los jugadores
        int life,score,damage;
        
  public:
       Characters(int life,int score,int damage);
       
        int Attacked() { return (life-damage);}
       
};
Characters::Characters(int life,int score,int damage){};

//wouldhave player background sound'
SAMPLE *backgroundmusic;
    backgroundmusic= load_sample( "Sound/song1.wav");
    if(!backgroundmusic)
    {
      destroy_sample(backgroundmusic);
       allegro_message("Error Could not find soundfile");
      return 0;
    }
    //play_sample(backgroundmusic,100,128,1000,true);
    destroy_sample(backgroundmusic);
*/
