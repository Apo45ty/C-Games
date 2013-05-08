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
#define SizeOfSpaceperMAtrix 32
#define sizeofenemies 16
#define NumberofEnemies 6
int map[640/SizeOfSpaceperMAtrix][480/SizeOfSpaceperMAtrix]={0};
/*
volatile bool Close =false;
void Handler() {Close=true;}
volatile long counter=0;
void IncCtr(){counter++;}*/


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
      
    set_window_title("Mario");
    //background
    bool fullscreen =false;
    
    if(fullscreen)
       set_gfx_mode(GFX_AUTODETECT,640,480,0,0);
    else
        set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
    //timer for the frames per second
    /*LOCK_VARIABLE(counter);
    LOCK_FUNCTION(IncCTR);
    install_int_ex(IncCtr,BPS_TO_TIMER(400));
    //closebutton
    //LOCK_FUNCTION(Handler);
    //set_close_button_callback(Handler);*/
};//end of initialisation


int main()
{
    Initialisation();
    BITMAP *buffer = create_bitmap(640,480);
    textprintf(buffer,font,0,0,makecol(255,255,255),"Loading...");
    blit(buffer,screen,0,0,0,0,640,480); 
    clock_t tStart= clock();
    clock_t tEnemyMove=clock();
    
    
    //load images
    BITMAP *BackroundImageMoving= load_bitmap("images/simplebackground.bmp",NULL);
    BITMAP *PlayerImage= load_bitmap("images/player.bmp",NULL);
    //if error in loading
    if(!BackroundImageMoving||!PlayerImage)
    {
    destroy_bitmap(BackroundImageMoving);
    destroy_bitmap(PlayerImage);
    allegro_message("Error Could not find image");
    return 0;
    }
    //crate random number
    srand(time(NULL)*time(NULL));
    //intiasialisation
    enum Direction{down=0,left=32,right=64,up=96};
    int random=rand(),i, BackgroundX=0;;
    int x=0,y=480-32,ImageX=0,ImageY=down,randx=random%608,randy=random%448,level=1;
    int BackgroundRandX=0,BackgroundRandY=0,score=0,ground=480-32,gravity=5,velx=1,life=100,milles=0;
    //enemies variables
    int enemyspositionsx=16,enemyspositionsy=16, XEnemiesDirrection=0;
    bool enemyturningleft= enemyspositionsx==16, enemyturningright= enemyspositionsx==0;
    int XEnemySpace[NumberofEnemies],YEnemySpace[NumberofEnemies],EnemiesLife[NumberofEnemies] = {100};
    for(i=1;i<=NumberofEnemies;i++)
    {               
        XEnemySpace[i]=(rand()%(640/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
        if(i>2)
            YEnemySpace[i]=480-SizeOfSpaceperMAtrix-(rand()%(64/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
        else
            YEnemySpace[i]=480-SizeOfSpaceperMAtrix;
        
        map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;
        EnemiesLife[i]=100;
    }//end of enemies variables
    
    //gameloop    
    while(!key[KEY_ESC]&&life>0){
        //while(counter>0){
        
        if(key[KEY_UP]&&(y==ground||x==0)){
           y-=70;
           //ImageY=up;
        }else
        if(key[KEY_DOWN]&&y<480-32){
           y+=5;
           //ImageY=down;
        }
        if(key[KEY_LEFT]&&x>0){
           x-=5;
           ImageY=left;
        }else 
        if(key[KEY_RIGHT]&&x<320){
           x+=5;
           ImageY=right;
        }
        
        if(!key[KEY_RIGHT]&&!key[KEY_DOWN]&&!key[KEY_UP]&&!key[KEY_LEFT])
           ImageX=32;
        else
           ImageX+=32;
           if(ImageX>64)
             ImageX=0; 
        if(y<ground)
        y+=gravity;
        if(y>ground)
        y=ground;
        
        //player hits an enemy
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]!=0 
        &&PlatformCollision(x,y,XEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ]+enemyspositionsx,YEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ]+enemyspositionsy,32,32,sizeofenemies,sizeofenemies) )
        {
        x-=SizeOfSpaceperMAtrix;
        }//end of player hits an enemy        
        //level change
        if(500*level-milles<0)
        {
                                 milles=0;
                                 level++;
        }
        //end of level change
        //background moving and drawing
        if(key[KEY_RIGHT]&&x>=320)
        {
           ImageY=right;
           BackgroundX+=5;
           velx=5;
           milles++;
        }
        else
        velx=1;
        //keeping player in screen
             if(y>ground)
                   y=ground;
             if(x>480-32)
                   x=480-32;
             if(y<0)
                   y=0;
             if(x<0)
             {
                   x=0;
                   life-=1*level;
             }
                   
        //end of screen
        if(BackgroundX>246)
           BackgroundX=0;
        masked_blit(BackroundImageMoving,buffer,BackgroundX,0,0,480-133,246,133);
        for(i=246-BackgroundX;i<640;i+=246)
        masked_blit(BackroundImageMoving,buffer,0,0,i,480-133,246,133);
        //end of background drawwing
        
        //begining of enemy drawing
        for(i=1;i<=NumberofEnemies;i++)
        { 
        if(XEnemySpace[i]>0)
        {
             map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=0;
             XEnemySpace[i]-=velx;
             map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;               
             rectfill(buffer, XEnemySpace[i]+enemyspositionsx,YEnemySpace[i]+enemyspositionsy,XEnemySpace[i]+sizeofenemies+enemyspositionsx, YEnemySpace[i]+sizeofenemies+enemyspositionsy, makecol(0,255,0));
        }
        else
           XEnemySpace[i]=(rand()%(500/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix)+140; 
           
        }
        //end of enemy drawing
        textprintf(buffer,font,300,0,makecol(255,255,255),"milles=%d",(500*level-milles) );
        textprintf(buffer,font,0,0,makecol(255,255,255),"Life=%d",life);
        textprintf(buffer,font,500,0,makecol(255,255,255),"Timer=%f",TimePassed( tStart ,clock() ) );
        masked_blit(PlayerImage,buffer,ImageX,ImageY,x,y,32,32);
        textprintf(buffer,font,620,470,makecol(255,255,255),"%d",level);
        //counter-=1;
        //};
        
        
        
        blit(buffer,screen,0,0,0,0,640,480);  
        clear_bitmap(buffer);
        rest(5); 
    }
    destroy_bitmap(buffer);
    destroy_bitmap(BackroundImageMoving);
    destroy_bitmap(PlayerImage);
    return 0;
}
END_OF_MAIN();
        //rectfill(buffer,x,y,x+5,y+5,makecol(0,0,0));
