#include<allegro.h>
#include<iostream>
#include<math.h>
#include<conio.h>
#include<time.h>
#include<sstream>
#include<cstring>
#include<string>

#include "Customfunctions.h"
using namespace GAMECLOCK;
using namespace std;
using namespace Collisions;
using namespace GravityWarp;

volatile bool Close =false;
void Handler() {Close=true;}


void Initialisation(void){
     allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound(DIGI_AUTODETECT ,MIDI_AUTODETECT,"A");
    
    
    int depth;
    if ((depth = desktop_color_depth()) != 0) {
         set_color_depth(depth);
      }
      
    set_window_title("GravityWarp");
    //background
    bool fullscreen =false;//full game will me fullscreen
    
    if(fullscreen)
       set_gfx_mode(GFX_AUTODETECT,640,480,0,0);
    else
        set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
};//end of initialisation

   
bool objects(int random,bool objectsstart,BITMAP *buffer,int mousey,int mousex,int gravity,int xspeed,int wrapeffectx,int wrapeffecty,int TypeofPower,int ground,int x,int y,bool Collisioncheck){
    static int const numberofobjects=30;
    int healthpacks=numberofobjects%5;
    int i,j,numberofstaticplatforms=((numberofobjects-1)/2);
    int damageblocks=numberofstaticplatforms%5;
    static int objx[numberofobjects],objy[numberofobjects];
    if(objectsstart)
    for(i=0;i<numberofobjects;i++){
    objx[i]=(random*i)%635;
    objy[i]=(random*i)%475;}
    else if(Collisioncheck){
    for(i=0;i<numberofobjects;i++)
             if(PlatformCollision(x,y, objx[i], objy[i],5,5,5,5))
                                        return true;
                                        
    return false;
    }else
    for(i=0;i<numberofobjects;i++){
             if(!PlatformCollision(x,y, objx[i], objy[i],5,5,5,5)&&i<=numberofstaticplatforms){//||TypeofPower==(1||2)  to make staticplatforms move                      
             objx[i]+=GravityWarpX(TypeofPower, mousex, objx[i], wrapeffectx, xspeed, 0);
             objy[i]+=GravityWarpX(TypeofPower, mousey, objy[i], wrapeffecty, gravity, 0);}                                                                    
             if(objy[i]>ground)
                   objy[i]=ground;
             if(objx[i]>635)
                   objx[i]=635;
             if(objy[i]<0)
                   objy[i]=0;
             if(objx[i]<0)
                   objx[i]=0;
             //so the las blocks are static blocks
             //and the the ones before the healthpacks are damageblocks
             if(i<(numberofobjects-damageblocks-healthpacks))
             rectfill(buffer,objx[i],objy[i],objx[i]+5,objy[i]+5,makecol(0,0,255));
             else 
             rectfill(buffer,objx[i],objy[i],objx[i]+5,objy[i]+5,makecol(255,0,0));}

}    

int main()
{
    Initialisation();
    LOCK_FUNCTION(Handler);
    set_close_button_callback(Handler);
    BITMAP *buffer = create_bitmap(640,480);
    Close=Introtogame(buffer); 
    textprintf(buffer,font,0,0,makecol(255,255,255),"Loading...");
    blit(buffer,screen,0,0,0,0,640,480);
    
    //start of game clock
    clock_t tStart= clock();
    //sound
    set_volume(255,128);
    //load images
    srand(time(NULL)*time(NULL));
    //intiasialisation
    //enum Direction{down=0,left=32,right=64,up=96};
    int random=rand(),gravity=3,xspeed=0,warpeffecty=2,wrapeffectx=5;
    int x=0,y=0,score=0,ground=475,WarpMax=200,Warp=WarpMax;
    int TypeofPower=0,playerslife=100,level=1;
    int Exitx=random%300+100, Exity=random%300;
    bool objectsstart=true;
    objects(random,objectsstart,buffer,mouse_y,mouse_x,gravity,xspeed,wrapeffectx,warpeffecty,TypeofPower,ground,x,y,false);
    objectsstart=false;
    //gameloop   
    while(!key[KEY_ESC]&&!Close){
        
        if(Warp!=WarpMax)
        Warp+=1;
        //atrack whit mouse
        //remember to chek if has enough warp power to use mouse
                         if(mouse_b & 1&&Warp>4){
                            Warp-=4;         
                            TypeofPower=1;}
                         else if(mouse_b & 2 && Warp>4){
                            Warp-=4;         
                            TypeofPower=2;}
                          else
                          TypeofPower=0; 
                                    
        //end warp functions and mouse attacks
          //character velocity asining
          bool WasInTheAir = y<=(ground-51);
          bool collisionwhitobjects=objects(random,objectsstart,buffer,mouse_y,mouse_x,gravity,xspeed,wrapeffectx,warpeffecty,TypeofPower,ground,x,y,true)||PlatformCollision(x,y, mouse_x, mouse_y,5,5,5,5);
          if(!collisionwhitobjects){
              y+=GravityWarpY(0, mouse_y, y, warpeffecty, gravity, Warp); 
              x+=GravityWarpX(0, mouse_x, x, wrapeffectx, xspeed, Warp);}
              
          else if(collisionwhitobjects && TypeofPower==(1||2) ){
              y+=GravityWarpY(TypeofPower, mouse_y, y, warpeffecty, gravity, Warp); 
              x+=GravityWarpX(TypeofPower, mouse_x, x, wrapeffectx, xspeed, Warp);}
           
              
      //players controls  
        if(key[KEY_W]&&(y>=ground||x==0||x==635||collisionwhitobjects))
           y-=50;
        else
        if(key[KEY_S]&&y<ground)
           y+=5;
           
        if(key[KEY_A]&&x>0&&!collisionwhitobjects)
           x-=5;
        else 
        if(key[KEY_D]&&x<635&&!collisionwhitobjects)
           x+=5;
        if(key[KEY_P])
           score+=500;         
      //keep player inside the box if(!PlatformCollision(x,y, objx[i], objy[i],5,5,5,5))
      if(y>ground)
      y=ground;
      if(x>635)
      x=635;
      if(y<0)
      y=0;
      if(x<0)
      x=0;
      //falldamage
      if(x!=0&&x!=635&&!collisionwhitobjects&&y>=(ground-60)&&y<=(ground-51)&&WasInTheAir&&TypeofPower==0)
              playerslife-=1;
    
          //player scores
          if(PlatformCollision(x,y, Exitx, Exity,5,5,10,10)){
          score+=30;
          Exitx=rand()%300+100; 
          Exity=rand()%300;}
          //player wins
          if(score>=500*level){
          level++;
          playerslife+=50;
          random=rand();
          objects(random,true,buffer,mouse_y,mouse_x,gravity,xspeed,wrapeffectx,warpeffecty,TypeofPower,ground,x,y,false);}
          //player loses
          if(playerslife==0){
          playerslife=100;
          score=0;}
          //buffer updates and screen updates
        statusbar(buffer,Warp,score,x,y,playerslife,level);
        textprintf(buffer,font,500,0,makecol(255,255,255),"Timer=%f",TimePassed( tStart ,clock() ) );
        objects(random,objectsstart,buffer,mouse_y,mouse_x,gravity,xspeed,wrapeffectx,warpeffecty,TypeofPower,ground,x,y,false);
        
        rectfill(buffer,Exitx,Exity,Exitx+10,Exity+10,makecol(255,0,255));
        rectfill(buffer,x,y,x+5,y+5,makecol(255,255,255));
        rectfill(buffer,mouse_x,mouse_y,mouse_x+5,mouse_y+5,makecol(0,255,0));
        
        blit(buffer,screen,0,0,0,0,640,480);  
        clear_bitmap(buffer); 
        rest(60);
    }
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();
        //rectfill(buffer,x,y,x+5,y+5,makecol(0,0,0));
