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
#define sizeofenemies 32
#define NumberofEnemies 5
int map[640/SizeOfSpaceperMAtrix][480/SizeOfSpaceperMAtrix]={0};
/*
volatile bool Close =false;
void Handler() {Close=true;}
volatile long counter=0;
void IncCtr(){counter++;}*/
void wait(int seconds)
{
   clock_t counter = clock();
   while( ( clock()-counter ) /CLOCKS_PER_SEC <= seconds ){};
}

int DrawingStatusBar(BITMAP *buffer,BITMAP *PlayerAttacks,int *key_a_Action,int *key_s_Action,int Lifewith,int PlayerMagicPower,int PlayerPower,int *x,int *y,int milles,int BossDistance,int NumberOfPowers)
{
        //int StatusTempx=0,StatusTampy=0;                    
        textprintf(buffer,font,0,0,makecol(255,255,255),"-Life-");
        if( Lifewith >0 && Lifewith <640 )
           rectfill(buffer, 0,10,Lifewith,15, makecol(0,255,0));
        else
        if(Lifewith>640)
        {
           rectfill(buffer, 0,10,610,15, makecol(0,255,0));
           textprintf(buffer,font,610,10,makecol(255,255,255),"X%d",Lifewith/640);
        }
        else
        if(Lifewith<=0)
            textprintf(buffer,font,0,10,makecol(255,255,255),"Dead");
        
        textprintf(buffer,font,0,20,makecol(255,255,255),"-Power-");
        if(640>PlayerPower>0)
          rectfill(buffer, 0,30,PlayerPower,35, makecol(128,0,255));
        else
        if(640<PlayerPower)
        {
           rectfill(buffer, 0,30,610,35, makecol(128,0,255));
           textprintf(buffer,font,610,30,makecol(255,255,255),"X%d",PlayerPower/640);
        }
        
        textprintf(buffer,font,0,40,makecol(255,255,255),"-MP:-");
        if(640>PlayerMagicPower>0)
            rectfill(buffer, 0,50,PlayerMagicPower,55, makecol(0,0,255));
        else
        if(PlayerMagicPower>640)
        {
           rectfill(buffer, 0,50,610,55, makecol(0,0,255));
           textprintf(buffer,font,610,50,makecol(255,255,255),"X%d",PlayerMagicPower/640);
        }
        
        textprintf(buffer,font,0,60,makecol(255,255,255),"A:");
        textprintf(buffer,font,20+10+20,60,makecol(255,255,255),">>>"); 
        masked_blit(PlayerAttacks,buffer,0,20*(*key_a_Action-1),20,55,20,20);
             
        textprintf(buffer,font,0,90,makecol(255,255,255),"S:");
        textprintf(buffer,font,20+20+10,90,makecol(255,255,255),">>>");
        masked_blit(PlayerAttacks,buffer,0,20*(*key_s_Action-1),20,90,20,20);
        
        if(key[KEY_SPACE])
        {
             textprintf(buffer,font,0,470,makecol(255,255,255),"Use Mouse");
             rectfill(buffer, mouse_x,mouse_y,mouse_x+10,mouse_y+10, makecol(255,255,255));
             for(int i=0;i<NumberOfPowers;i++)
             {
                masked_blit(PlayerAttacks,buffer,0,20*i,80+20*i,60,20,20);
                masked_blit(PlayerAttacks,buffer,0,20*i,80+20*i,80,20,20);
             if(PlatformCollision(mouse_x,mouse_y,80+20*i,60,10,10,20,20) )
                *key_a_Action=i+1; 
             if(PlatformCollision(mouse_x,mouse_y,80+20*i,80,10,10,20,20) )
                *key_s_Action=i+1; 
             }    
        }
}

int PlayerPowers(int TypeOfAttack,BITMAP *buffer,BITMAP *PlayerAttacks,int *EnemiesLifewith,int *PlayerPower,int *PlayerAttack,int *BossLife,int *ImageY, int milles,int BossDistance,int *x, int *y,int *BossX,int *BossY,int ground,int *PlayerMagicPower,int *ImageMover)
{
    int i=0;
    enum Direction{down=0,left=32,right=64,up=96};
    *PlayerAttack=*PlayerPower/10;
    switch(TypeOfAttack)
    {
    case 1:
         if(*PlayerPower>5)
         {
         if(*ImageY==right)    
           {        
                //&&abs(*BossY-*y)<=154
                masked_blit(PlayerAttacks,buffer,20,0,*x+15,*y+10,20,20);
                if(map[*x/SizeOfSpaceperMAtrix+1][*y/SizeOfSpaceperMAtrix]>0||abs(*BossY-*y)<=154&&abs(*BossX-*x)<=132&&milles>=BossDistance)
                {   
                   masked_blit(PlayerAttacks,buffer,146,3,*x,*y-15,27,15);
                   if(milles<BossDistance)                                                         
                       *( EnemiesLifewith+map[*x/SizeOfSpaceperMAtrix+1][*y/SizeOfSpaceperMAtrix] )-=*PlayerAttack;
                   else
                   if(abs(*BossY-*y)<154&&abs(*BossX-*x)<132)
                       *BossLife-=*PlayerAttack;
                }
                else
                masked_blit(PlayerAttacks,buffer,146,17,*x,*y-15,30,20);
           }
           else
           if(*x/SizeOfSpaceperMAtrix>0&&*ImageY==left)
           {
                masked_blit(PlayerAttacks,buffer,0,0,*x-5,*y+10,20,20); 
                if(map[*x/SizeOfSpaceperMAtrix-1][*y/SizeOfSpaceperMAtrix]>0||abs(*BossY-*y)<=154&&abs(*BossX-*x)<=132&&milles>=BossDistance)
                {
                   masked_blit(PlayerAttacks,buffer,146,3,*x,*y-15,27,15);
                   if(milles<BossDistance)
                      *( EnemiesLifewith+map[*x/SizeOfSpaceperMAtrix-1][*y/SizeOfSpaceperMAtrix] )-=*PlayerAttack;
                   else
                   if(abs(*BossY-*y)<=154&&abs(*BossX-*x)<=132&&milles>=BossDistance)
                      *BossLife-=*PlayerAttack;
                }
                else
                masked_blit(PlayerAttacks,buffer,146,17,*x,*y-15,30,20);
           }
         *PlayerPower-=5;
         }
         break;
    case 2:
         if(*PlayerPower>5)
         {
            //masked_blit(PlayerAttacks,buffer,0,20,*x,*y,20,20);
            if(key[KEY_UP]&&*y>0)
            {
                *y-=20;
            }else
            if(key[KEY_DOWN]&&*y<ground)
            {
                *y+=5;
            }
            if(key[KEY_LEFT]&&*x>=0)
            {
                *x-=5;
                *ImageY=left;
            }else 
            if(key[KEY_RIGHT]&&*x<640)
            {
                *x+=5;
                *ImageY=right;
            }
            *PlayerPower-=5;
         }
         break;
    case 3://32,48
         if(*PlayerMagicPower>10)
         {
         int DirectionTemp=0;
         if(*ImageY==right)
            DirectionTemp=1;
         else
         if(*ImageY==left)
           DirectionTemp=-1;
           
           for(i=0;i<=8;i++)
           {
              //masked_blit(PlayerAttacks,buffer,32+32*(*ImageMover),48,*x+32+32*i,*y,32,32);
              //&&*x+i*32< 640&&*x+i*32>=0
              masked_blit(PlayerAttacks,buffer,32+32*(*ImageMover),48,*x+32*i*DirectionTemp,*y,32,32);
              if( map[(*x/SizeOfSpaceperMAtrix)+i][*y/SizeOfSpaceperMAtrix] !=0 && *x+i*SizeOfSpaceperMAtrix< 640 && *x+i*SizeOfSpaceperMAtrix>=0) 
              { 
                *PlayerMagicPower-=10;
                *( EnemiesLifewith+map[(*x/SizeOfSpaceperMAtrix)+i*DirectionTemp][*y/SizeOfSpaceperMAtrix] )-=50; 
              }//fix for the left side shoothing
              //else
              if(abs(*BossX - *x)<=32*8 && milles>=BossDistance)
              {
                 *BossLife-=50;
                 *PlayerMagicPower-=10;
              }
              
           }
         }
         break;
    case 4:
         if(*PlayerPower>20)
         {
           *PlayerPower-=20;
           *PlayerMagicPower+=10;
         }
         break;
         
    default : 
            return 0;
    }
    return 0;
}

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
    textprintf(buffer,font,0,480,makecol(255,255,255),"Loading...");
    blit(buffer,screen,0,0,0,0,640,480); 
    clock_t tStart= clock();
    clock_t tEnemySpawn=clock();
    clock_t tChargeAttackTimer=clock();
    
    
    //load images
    BITMAP *BackroundImageMoving= load_bitmap("images/simplebackground.bmp",NULL);
    BITMAP *PlayerImage= load_bitmap("images/player.bmp",NULL);
    BITMAP *PlayerAttacks= load_bitmap("images/Player Attacks 16bt.bmp",NULL);
    BITMAP *Enemies= load_bitmap("images/Enemies4game 16bit.bmp",NULL);
    BITMAP *Floor= load_bitmap("images/floor.bmp",NULL);
    BITMAP *ItemsInGame= load_bitmap("images/ItemsinGame.bmp",NULL);
    //if error in loading
    if(!BackroundImageMoving||!PlayerImage||!PlayerAttacks||!Enemies||!Floor||!ItemsInGame)
    {
    destroy_bitmap(PlayerAttacks);
    destroy_bitmap(BackroundImageMoving);
    destroy_bitmap(PlayerImage);
    destroy_bitmap(Enemies);
    destroy_bitmap(Floor);
    destroy_bitmap(ItemsInGame);
    allegro_message("Error Could not find image");
    return 0;
    }
    //intro to items in game
    masked_blit(ItemsInGame,buffer,0,0,0,0,640,122);
    textprintf(buffer,font,0,470,makecol(255,255,255),"Loaded");
    blit(buffer,screen,0,0,0,0,640,480);  
    readkey();  
    //intro to items in game
    
    
    //crate random number
    srand(time(NULL)*time(NULL));
    //intiasialisation
    enum Direction{down=0,left=32,right=64,up=96};
    int random=rand(),i, BackgroundX=0;;
    int x=0,y=480-32,ImageX=0,ImageY=down,randx=random%608,randy=random%448,level=1;
    int BackgroundRandX=0,BackgroundRandY=0,score=0,ground=480-64,gravity=5,velx=1,Lifewith=100;
    int milles=0,key_a_Action=1,key_s_Action=2,PlayerPowerMax=200,PlayerPower=PlayerPowerMax;
    int PlayerAttack=5+PlayerPower/10,PlayerMagicPowerMAX=100,PlayerMagicPower=PlayerMagicPowerMAX;
    int ImageMover=0,NumberOfPowers=3;
    //enemies variables
    int BossLifeMAX=640*4,BossLife=BossLifeMAX;
    int BossThunderBallYDraw=14,BossThunderBallXDraw=662;
    int BossThunderBallX=rand()%(640-132),BossThunderBallY=ground-67+32;
    int ChargeAttackRange=133,TimeAttackTimer=0;
    bool BossHasAshield = rand()%2==1 ;
    int BossX=640-132,BossY=ground-(150-32), EnemyImageX=0, YBossBallDirection=0;
    bool BossThunderBallTurnsDown= BossThunderBallY==300;
    bool BossThunderBallTurnsUp= BossThunderBallY==ground-67+32; 
    bool BossFight=false;
    int enemyspositionsx=0,enemyspositionsy=16, XEnemiesDirrection=0,BossDistance=500;
    bool enemyturningleft= enemyspositionsx==16, enemyturningright= enemyspositionsx==0;
    int XEnemySpace[NumberofEnemies],YEnemySpace[NumberofEnemies],EnemiesLifewith[NumberofEnemies] = {100};
    for(i=1;i<=NumberofEnemies;i++)
    {               
        XEnemySpace[i]=(rand()%(320/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix)+320;
        YEnemySpace[i]=ground;
        map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;
        EnemiesLifewith[i]=100;
    }//end of enemies variables
    
    //gameloop 
    while(!key[KEY_ESC]){
        
        //background moving and drawing
        if(key[KEY_RIGHT]&&x>=320&&milles<BossDistance)
        {
           ImageY=right;
           BackgroundX+=5;
           velx=5;
           milles++;
        }
        else
        velx=1;
        
        //moving pieces
        if(BackgroundX>246)
           BackgroundX=0;
        masked_blit(BackroundImageMoving,buffer,BackgroundX,0,0,ground-(133-32),246,133);
        masked_blit(Floor,buffer,BackgroundX,0,0,480-32,246,32);
        for(i=246-BackgroundX;i<640;i+=246)
        {
        masked_blit(BackroundImageMoving,buffer,0,0,i,ground-(133-32),246,133);
        masked_blit(Floor,buffer,0,0,i,480-32,246,32);
        }
        //end of background drawwing
        //controls
        if(PlayerPower<PlayerPowerMax)
        PlayerPower++;
        //while(counter>0){
        
        if(key[KEY_UP]&&y==ground){
           y-=70;
           //ImageY=up;
        }else
        if(key[KEY_DOWN]&&y<ground){
           y+=5;
           //ImageY=down;
        }
        if(key[KEY_LEFT]&&x>0){
           x-=5;
           ImageY=left;
        }else 
        if(key[KEY_RIGHT]&&(x<=320||milles==BossDistance)){
           x+=5;
           ImageY=right;
        }
        //begin A attack
        if(key[KEY_A])
        {
         PlayerPowers(key_a_Action,buffer,PlayerAttacks,&EnemiesLifewith[0],&PlayerPower,&PlayerAttack,&BossLife, &ImageY, milles, BossDistance, &x, &y, &BossX, &BossY,ground,&PlayerMagicPower,&ImageMover); 
        }//end of A attack
        //begin S attack
        if(key[KEY_S])
        {
         PlayerPowers(key_s_Action,buffer,PlayerAttacks,&EnemiesLifewith[0],&PlayerPower,&PlayerAttack,&BossLife, &ImageY, milles, BossDistance, &x, &y, &BossX, &BossY,ground,&PlayerMagicPower,&ImageMover); 
        }//end of S attack
        
        if(!key[KEY_RIGHT]&&!key[KEY_DOWN]&&!key[KEY_UP]&&!key[KEY_LEFT])
           ImageX=32;
        else
           ImageX+=32;
           if(ImageX>64)
             ImageX=0; 
        //gravity on player
        if(y<ground)
        y+=gravity;
        //keeping player in screen
             if(y>ground)
                   y=ground;
             if(x>640-32)
                   x=640-32;
             if(y<0)
                   y=0;
             if(x<0)
             {
                   x=0;
             }
         ImageMover++;
         if(ImageMover>8)
           ImageMover=0;          
        //end of screen
        
        //player hits an enemy
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]!=0 
        &&PlatformCollision(x,y,XEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ],YEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ],32,32,sizeofenemies,sizeofenemies) )
        {
             x-=SizeOfSpaceperMAtrix;
             Lifewith-=1;
        }//end of player hits an enemy        
        
        //begining of enemy drawing
        if(!BossFight)
           for(i=1;i<=NumberofEnemies;i++)
           { 
             if(milles<BossDistance)
             {
               if(XEnemySpace[i]>0&&EnemiesLifewith[i]>0)
               {
                  map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=0;
                  XEnemySpace[i]-=velx;
                  EnemyImageX+=32;//make it apear as if the enmy moves
                  if(EnemyImageX>=96)
                     EnemyImageX=0;
                  map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;               
                  masked_blit(Enemies,buffer,EnemyImageX,0,XEnemySpace[i],YEnemySpace[i],32,32);
                  textprintf(buffer,font,XEnemySpace[i],YEnemySpace[i],makecol(255,255,255),"%d", EnemiesLifewith[i]);
                  //rectfill(buffer, XEnemySpace[i]+enemyspositionsx,YEnemySpace[i]+enemyspositionsy,XEnemySpace[i]+sizeofenemies+enemyspositionsx, YEnemySpace[i]+sizeofenemies+enemyspositionsy, makecol(0,255,0));
               }
               else
               if(TimePassed( tEnemySpawn ,clock() ) >= 3  )//only allows one enemy to spawn every time
               {
                   tEnemySpawn=clock();
                   map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=0;
                   EnemiesLifewith[i]=100;     
                   XEnemySpace[i]=(rand()%(320/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix)+320; 
               }
               else
               map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=0;
             }       
             else
             {//erase all enemy possitions
                 map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=0;
                 BossFight=true;
             }
           }
        //end of enemy drawing
        if(BossLife<=0)
        {
              //firstBossitem
              if(BossLifeMAX<=640*4)
              {
                  textprintf(screen,font,x+32,y+15,makecol(255,255,255),"New Item Optained");
                  masked_blit(PlayerAttacks,screen,0,20*3,x+32,y,20,20);
                  wait(1);
              }
              //first Boss item
              BossFight=false;
              PlayerMagicPower+=PlayerMagicPowerMAX*(BossLifeMAX/640);
              Lifewith*=2;
              milles=0;
              BossDistance*=2;
              level++;
              BossLifeMAX*=2;
              BossLife=BossLifeMAX;
              NumberOfPowers=4;
        }
        //go to boss
        if(BossFight)
        {
           int ChargeAttackTemp=2;
           if(BossLife>0&&BossLife<635)
                 rectfill(buffer, 0,475,BossLife,480, makecol(255,0,0));
           else
           if(BossLife>0)
           {
             int BossLifeTemp=(BossLifeMAX-BossLife);
             if((BossLifeMAX-BossLife)>600)
                 BossLifeTemp=abs(BossLifeMAX-BossLife)-600*(BossLifeMAX/600-BossLife/600);
             rectfill(buffer, 0,475,600,480, makecol(255,0,0));
             rectfill(buffer, 0,475,600-BossLifeTemp,480, makecol(255,0,255));
             textprintf(buffer,font,610,470,makecol(255,0,0),"*%d",BossLife/640);
           }//end enemy life printing
           
           if(BossLife>=BossLifeMAX*1/2)
           {//begin phase one
               TimeAttackTimer=ChargeAttackTemp-(int)TimePassed( tChargeAttackTimer ,clock() );
               if(TimeAttackTimer<=0)
                   tChargeAttackTimer=clock();
           
               if(ImageY==right)
               {
                   BossX=640-132;
                   masked_blit(Enemies,buffer,218,121,BossX,BossY,132,154);
                   if(TimeAttackTimer>ChargeAttackTemp-1)
                           masked_blit(Enemies,buffer,104,46,BossX-ChargeAttackRange,ground-53,158,53);
                   if(abs(BossX-x)<=ChargeAttackRange)
                   {
                       textprintf(buffer,font,100,100,makecol(255,255,255),"Boss charge attack in %d",TimeAttackTimer);
                       if(TimeAttackTimer<=0)
                       {
                           Lifewith-=10;
                           x-=ChargeAttackRange;
                           //masked_blit(Enemies,buffer,104,46,BossX-132,BossY,132,154);
                       } 
                   }    
               }
               else
               if(ImageY==left)
               {
                    BossX=0;
                    masked_blit(Enemies,buffer,218+132,121,BossX,BossY,132,154);
                    if(TimeAttackTimer>ChargeAttackTemp-1)
                           masked_blit(Enemies,buffer,261,48,BossX+ChargeAttackRange,ground-53,158,58);
                    if(abs(BossX+132-x)<=ChargeAttackRange)
                    {
                         textprintf(buffer,font,100,100,makecol(255,255,255),"Boss charge attack in %d",TimeAttackTimer);
                         if(TimeAttackTimer<=0)
                         {
                             Lifewith-=10;
                             //masked_blit(Enemies,buffer,104,46,BossX+132,BossY,158,53);
                         }
                         
                    }
               }
           BossHasAshield=true; 
           //BossX=rand()%(640-132);   
           }//end of phase one
           else
           if(BossLife<=BossLifeMAX*1/2) 
           {
              if( PlatformCollision(x,y,BossThunderBallX,BossThunderBallY,32,32,66,66) || BossHasAshield&&PlatformCollision(x,y,BossX,BossY,32,32,132,154))
              {
                      Lifewith-=2;
                      if(BossHasAshield&&PlatformCollision(x,y,BossX,BossY,32,32,132,154))
                           BossX=rand()%(640-132);
                      if(x>32)
                           x-=32;
                      else
                           x+=32;
                      if(BossHasAshield)
                           BossLife+=2;
              }
              
              if(BossX>x)
              {
                  masked_blit(Enemies,buffer,218,121,BossX,BossY,132,154);
              }
              else
              if(BossX<x)
              {
                  masked_blit(Enemies,buffer,218+132,121,BossX,BossY,132,154);
              }
              //BossBallmovement
              if(BossThunderBallX>x)
                  BossThunderBallX-=1;
              else
              if(BossThunderBallX<x)
                  BossThunderBallX+=1;
                  
              BossThunderBallTurnsDown= BossThunderBallY==300;
              BossThunderBallTurnsUp= BossThunderBallY==ground-67+32;           
              
              if(BossThunderBallTurnsDown)
                  YBossBallDirection=1;
              else if(BossThunderBallTurnsUp)
                 YBossBallDirection=-1;
              
              BossThunderBallY+=YBossBallDirection;
              //end of enemydirrection*/
              masked_blit(Enemies,buffer,BossThunderBallXDraw,BossThunderBallYDraw,BossThunderBallX,BossThunderBallY,66,67);
              if(BossHasAshield)
              {
                  masked_stretch_blit(Enemies,buffer, BossThunderBallXDraw, BossThunderBallYDraw, 66, 67, BossX, BossY, 132, 154);
                  
              }
              BossThunderBallYDraw+=70;
              if(BossThunderBallYDraw>=215)
              {
                 BossThunderBallYDraw=14;
              }
              //attacktimer
              TimeAttackTimer=4-(int)TimePassed( tChargeAttackTimer ,clock() );
              textprintf(buffer,font,100,100,makecol(255,255,255),"Boss charge attack in %d",TimeAttackTimer);
              if(TimeAttackTimer<=0)//attack counter
              {
                   tChargeAttackTimer=clock();
                   BossY=ground+32-154;
                   BossX=rand()%(640-132);
                   /*if(BossX>x)
                      BossThunderBallX=rand()%(BossX-133);
                   else
                   if(BossX<x)
                      BossThunderBallX=rand()%(640-66-BossX-133)+BossX+133;*/
                   
                   BossHasAshield = rand()%2==1 ;
              }//end of attack counter
           
           }  
        //BossX>x||BossX<x
        }
        //end of boss
        
        textprintf(buffer,font,300,0,makecol(255,255,255),"milles=%d",BossDistance-milles );
        DrawingStatusBar(buffer,PlayerAttacks, &key_a_Action, &key_s_Action, Lifewith, PlayerMagicPower, PlayerPower,&x,&y,milles,BossDistance,NumberOfPowers);
        textprintf(buffer,font,500,0,makecol(255,255,255),"Timer=%f",TimePassed( tStart ,clock() ) );
        masked_blit(PlayerImage,buffer,ImageX,ImageY,x,y,32,32);
        textprintf(buffer,font,500,10,makecol(255,255,255),"Level:1-%d",level);
        //end of draw
        //counter-=1;
        //};
        
        
        
        blit(buffer,screen,0,0,0,0,640,480);  
        clear_bitmap(buffer);
        rest(10); 
    }
    destroy_bitmap(buffer);
    destroy_bitmap(BackroundImageMoving);
    destroy_bitmap(PlayerImage);
    destroy_bitmap(PlayerAttacks);
    destroy_bitmap(Enemies);
    destroy_bitmap(Floor);
    destroy_bitmap(ItemsInGame);
    return 0;
}
END_OF_MAIN();
        //rectfill(buffer,x,y,x+5,y+5,makecol(0,0,0));
//218,121
//132,154
