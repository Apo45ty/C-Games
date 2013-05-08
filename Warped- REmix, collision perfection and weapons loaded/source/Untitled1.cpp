#include<allegro.h>
#include<iostream>
#include<math.h>
#include<conio.h>
#include<time.h>
#include<sstream>
#include<cstring>
#include<string>
using namespace std;
#define SizeOfSpaceperMAtrix 32
#define sizeofenemies 16
#define NumberofEnemies 10
int map[640/SizeOfSpaceperMAtrix][480/SizeOfSpaceperMAtrix]={0};



bool Collision(int x, int y, int x2, int y2,int hight1,int wight1,int hight2,int wight2){
     if(x+wight1<x2||y+hight1<y2||x>x2+wight2||y>y2+hight2)
      return false;
      else
      return true;}

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

int main()
{
    Initialisation();
    BITMAP *buffer = create_bitmap(640,480); 
    textprintf(buffer,font,0,0,makecol(255,255,255),"Loading...");
    blit(buffer,screen,0,0,0,0,640,480);
    srand(time(NULL)*time(NULL));
    //creating drawing items and platforms once on screen just before game starts
     int i;
     map[0][0]=1001;
     map[0][1]=1000;
     const int NumberOfPlatforms=10;
     int PlatformsX[NumberOfPlatforms],PlatformsY[NumberOfPlatforms];
     
     for(i=1;i<=NumberOfPlatforms;i++){
        MovePlatform:
        PlatformsX[i]=(rand()%(640/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
        PlatformsY[i]=(rand()%(301/SizeOfSpaceperMAtrix))*(SizeOfSpaceperMAtrix)+96;
        //PlatformsLenght[i]=(rand()%(400/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
        if(map[ PlatformsX[i]/SizeOfSpaceperMAtrix ][ PlatformsY[i]/SizeOfSpaceperMAtrix ]==0&&PlatformsY[i]<480)
            map[ PlatformsX[i]/SizeOfSpaceperMAtrix ][ PlatformsY[i]/SizeOfSpaceperMAtrix ]=1003;
        else
             goto MovePlatform;
     }//end of platform
    //exit variables
    int ExitX=rand()%(450-SizeOfSpaceperMAtrix)+100,ExitY=rand()%(100)+150;
    map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]=1002;
    //player variables
    bool PlayerLooksLeft=true;
    int sizeofplayerslegs=0,sizeofplayersbody=16,ground=480-sizeofplayersbody-sizeofplayerslegs;
    int leftwall=640-sizeofplayersbody,jump=64,BulletsMax=100,PlasmaChargeMax=100,Bullets=BulletsMax,PlasmaCharge=PlasmaChargeMax;
    int x=rand()%636,y=rand()%475+1,PlayersXmovement=0,life=30,gravity=5,score=0,TimeMultiplier=1,level=1;
    //enemies variables
    int EnemiesOnPlatforms=rand()%(NumberOfPlatforms+1);
    int XEnemySpace[NumberofEnemies],YEnemySpace[NumberofEnemies],EnemiesLife[NumberofEnemies] = {100};
    for(i=1;i<=NumberofEnemies;i++)
    
    {
    
     if(EnemiesOnPlatforms<i)
     {                         
        Reasing:
        XEnemySpace[i]=(rand()%(640/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
        if(XEnemySpace[i]>640)
           goto Reasing;
        YEnemySpace[i]=480-SizeOfSpaceperMAtrix;
        map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;
        EnemiesLife[i]=100;}
     else
     {
       XEnemySpace[i]=PlatformsX[i];
       YEnemySpace[i]=PlatformsY[i]-64;
       map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;
       EnemiesLife[i]=100; 
     }
    
    }
    
    int enemyspositionsx=16,enemyspositionsy=16, XEnemiesDirrection=0;
    bool enemyturningleft= enemyspositionsx==16, enemyturningright= enemyspositionsx==0;
    //end ofvariables
    textprintf(buffer,font,0,0,makecol(255,255,255),"Press any key to start");
    textprintf(buffer,font,x-20,y-20,makecol(255,255,255),"Player");
    circlefill(buffer, x+sizeofplayersbody/2, y+sizeofplayersbody/2, sizeofplayersbody/2, makecol(255,0,255));
    blit(buffer,screen,0,0,0,0,640,480);
    clear_bitmap(buffer);
    readkey();
    //start of game
    clock_t tStart= clock();
    while(!key[KEY_ESC]&&life>0){
        //players controls  
        if(key[KEY_P])
        score+=500;
        if(key[KEY_UP]&&(y==ground||x>=leftwall||x==0||map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix+1]==1003 || map[(x)/SizeOfSpaceperMAtrix][(y)/SizeOfSpaceperMAtrix+2] == 1003 || map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix+2] ==1003) )
            y-=jump-abs(PlayersXmovement); 
        else
        if(key[KEY_DOWN])
           y+=2*gravity;
           
        
        if(key[KEY_LEFT]){
           PlayersXmovement=-5;
           PlayerLooksLeft=true;
        }else 
        if(key[KEY_RIGHT]){
           PlayersXmovement=5;
           PlayerLooksLeft=false;
        }else {
           PlayersXmovement=0;}
        
        if(key[KEY_Q]&&Bullets>0)
            if(PlayerLooksLeft&&map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix]!=0&&map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix]<=NumberofEnemies)
                {
                EnemiesLife[ map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix] ]-=10;
                Bullets--;
                }
            else 
               if(!PlayerLooksLeft&&map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix]!=0&&map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix]<=NumberofEnemies)
                  {
                  EnemiesLife[ map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix] ]-=10; 
                  Bullets--;
                  } 
             else 
                Bullets--;  
          
        if(key[KEY_W]&&PlasmaCharge>0)  
        {
               rectfill(buffer, mouse_x,mouse_y,mouse_x+5, mouse_y+5, makecol(128,128,128)); 
               textprintf(buffer,font,400,30,makecol(255,255,255),"Pick a Target whit the mouse");       
               if(mouse_b & 1&&map[ mouse_x/SizeOfSpaceperMAtrix ][ mouse_y/SizeOfSpaceperMAtrix ] !=0 )
               {
                          map[ mouse_x/SizeOfSpaceperMAtrix ][ mouse_y/SizeOfSpaceperMAtrix ]=0;
                          PlasmaCharge-=10;
               }             
        }      
        if(key[KEY_A]&&y==ground)
        {  
                if(PlayerLooksLeft&&map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix]!=0&&map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix]<=NumberofEnemies)
                {
                EnemiesLife[ map[(x/SizeOfSpaceperMAtrix)-1][y/SizeOfSpaceperMAtrix] ]-=1;
                rectfill(buffer, x,y+1,x-5, y+3, makecol(128,128,128));
                }
            else 
               if(!PlayerLooksLeft&&map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix]!=0&&map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix]<=NumberofEnemies)
                  {
                  EnemiesLife[ map[(x/SizeOfSpaceperMAtrix)+1][y/SizeOfSpaceperMAtrix] ]-=1; 
                  rectfill(buffer, x+sizeofplayersbody,y+1,x+sizeofplayersbody+5, y+3, makecol(128,128,128));
                  } 
        } 
        
        
        x+=PlayersXmovement;
        y+=gravity;
    //end of controls      
    //player is on top of platform
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix+1]==1003 || map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix+1] == 1003 )
              {
              y-=gravity;
              }
    //player biding to scree
             if(y>ground)
                   y=ground;
             if(x>leftwall)
                   x=leftwall;
             if(y<0)
                   y=0;
             if(x<0)
                   x=0;
                   
        //enemy movement           
        enemyturningright= enemyspositionsx==0;
        enemyturningleft= enemyspositionsx==SizeOfSpaceperMAtrix -sizeofenemies;
        if(enemyturningleft)
           XEnemiesDirrection=-1;
           
        else if(enemyturningright)
           XEnemiesDirrection=1;
        enemyspositionsx+=XEnemiesDirrection;
        
        //player toches the exit
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]==1002 || map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix] == 1002)
              {
              clock_t tStart= clock();
              score+=60;           
              map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] =0;
              map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix]=0;
              restard2:
              ExitX=rand()%(450-SizeOfSpaceperMAtrix)+100;
              ExitY=rand()%(100)+150;
              if(map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]==0)
                 map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]=1002;
              else 
                goto restard2;
              }
        //playerpicks up bullets
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]==1001 || map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix] == 1001)
              {
              Bullets+=30;           
              map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] =0;
              map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix]=0;
              }
         //playerpicks up health
        if( map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]==1000 || map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix] == 1000 )
              {
              life+=30;           
              map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] =0;
              map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix]=0;
              }
        //player hits an enemy
        if( (map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix]||map[(x+sizeofplayersbody)/SizeOfSpaceperMAtrix][(y+sizeofplayersbody)/SizeOfSpaceperMAtrix])!=0 
        &&Collision(x,y,XEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ]+enemyspositionsx,YEnemySpace[ map[x/SizeOfSpaceperMAtrix][y/SizeOfSpaceperMAtrix] ]+enemyspositionsy,sizeofplayersbody,sizeofplayersbody,sizeofenemies,sizeofenemies) ){
        life-=1*level;
        x-=SizeOfSpaceperMAtrix;}
        
        //begin enemy drawing
        for(i=1;i<=NumberofEnemies;i++)
        {
        
          if(EnemiesLife[i]>0&&map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]!=0)
            {
            //rectfill(buffer, XEnemySpace[i],YEnemySpace[i],XEnemySpace[i]+SizeOfSpaceperMAtrix, YEnemySpace[i]+SizeOfSpaceperMAtrix, makecol(255,0,0));
            rectfill(buffer, XEnemySpace[i]+enemyspositionsx,YEnemySpace[i]+enemyspositionsy,XEnemySpace[i]+sizeofenemies+enemyspositionsx, YEnemySpace[i]+sizeofenemies+enemyspositionsy, makecol(0,255,0));
            textprintf(buffer,font,XEnemySpace[i],YEnemySpace[i],makecol(255,255,255),"%d",EnemiesLife[i]);
            }
          else 
          {
            score+=30;
            map[ XEnemySpace[i]/SizeOfSpaceperMAtrix ][ YEnemySpace[i]/SizeOfSpaceperMAtrix ]=0;
            XEnemySpace[i]=(rand()%(640/SizeOfSpaceperMAtrix+1))*(SizeOfSpaceperMAtrix);
            YEnemySpace[i]=480-SizeOfSpaceperMAtrix;
            map[XEnemySpace[i]/SizeOfSpaceperMAtrix][YEnemySpace[i]/SizeOfSpaceperMAtrix]=i;
            EnemiesLife[i]=100;
          }
        }//end enemy drawing
        
        //begin platform drawing
        for(i=1;i<=NumberOfPlatforms;i++){
        rectfill(buffer, PlatformsX[i],PlatformsY[i]-SizeOfSpaceperMAtrix,PlatformsX[i]+SizeOfSpaceperMAtrix, PlatformsY[i], makecol(255,255,255));
        }
        //level change
        if(score>500*level)
        {
        level++;
        PlasmaCharge+=30;               
        }
        //time bonus
        if( (clock() - tStart)/CLOCKS_PER_SEC > 30*TimeMultiplier )
        { 
            map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]=0;
            restard3:
            ExitX=rand()%(450-SizeOfSpaceperMAtrix)+100;
            ExitY=rand()%(100)+150;
            if(map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]==0)
            map[ExitX/SizeOfSpaceperMAtrix][ExitY/SizeOfSpaceperMAtrix]=1002;
            else
             goto restard3;
            map[0][0]=1001;
            map[0][1]=1000;
            TimeMultiplier++;
            textprintf(buffer,font,0,32,makecol(255,255,255),"B");
            PlasmaCharge+=5;
        }
        //end of bonus
        rectfill(buffer, x,y,x+sizeofplayersbody, y+sizeofplayersbody, makecol(0,0,255));
        rectfill(buffer, ExitX,ExitY,ExitX+sizeofplayersbody, ExitY+sizeofplayersbody, makecol(255,0,255));
        textprintf(buffer,font,0,0,makecol(255,255,255),"HP:%d",life);
        textprintf(buffer,font,50,0,makecol(255,255,255),"Bullets:%d Plasma:%d",Bullets,PlasmaCharge);
        textprintf(buffer,font,250,0,makecol(255,255,255),"Score %d",score);
        textprintf(buffer,font,620,470,makecol(255,255,255),"%2d",level);
        textprintf(buffer,font,430,0,makecol(255,255,255),"Use Q for Bullets");
        textprintf(buffer,font,430,20,makecol(255,255,255),"Use W for plasma");
        textprintf(buffer,font,430,10,makecol(255,255,255),"Use A onground to attack");
        textprintf(buffer,font,0,10,makecol(255,255,255),"B");
        
        blit(buffer,screen,0,0,0,0,640,480);  
        clear_bitmap(buffer); 
        rest(30);}
        
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();

/*
map array meaning 
1000 health pack
1001 bullets
1002 exit
1003 platform
1004 plasma powerup
1005 gunpowerup
1006 life powerup
*/
