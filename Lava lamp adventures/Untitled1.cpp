#include<allegro.h>
#include<iostream>
#include<math.h>
#include<conio.h>
#include<time.h>
#include<sstream>
#include<cstring>
#include<string>
using namespace std;

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
    int const numberofcanons=30;
    int centerofcanonx=0,canonnumber=0;
    int i,BonusMultiplier=1,x=rand()%640,y=rand()%475+1,movement=5,life=30,gravity=5,score=0;
    int bulletsy[numberofcanons]={0},weidht[numberofcanons],height[numberofcanons];
    srand(time(NULL)*time(NULL));
    //initialisation
    int red=rand()%255,blue=rand()%255,green=rand()%255;
    for(i=1;i<=numberofcanons;i++){//escojo el numero 1 para que asi el index sea el numero del ca~on
    height[i]=rand()%8+20/i;
    weidht[i]=height[i];}
    //end of initialisation
    
    textprintf(buffer,font,0,0,makecol(255,255,255),"Press any key to start");
    textprintf(buffer,font,x-20,y-20,makecol(255,255,255),"Player");
    circlefill(buffer, x+2.5, y+2.5, 2.5, makecol(255,0,255));
    blit(buffer,screen,0,0,0,0,640,480);
    clear_bitmap(buffer);
    readkey(); 
    //start of game
    clock_t tStart= clock();
    while(!key[KEY_ESC]&&life>0&&gravity<100){
        if(Collision(x,y,centerofcanonx,bulletsy[canonnumber],5,5,weidht[canonnumber],weidht[canonnumber])){
        life--;}
        //players controls  
        if(key[KEY_UP])
           y-=movement;
        else
        if(key[KEY_DOWN])
           y+=movement;
           
        if(key[KEY_LEFT])
           x-=movement;
        else 
        if(key[KEY_RIGHT])
           x+=movement;
    //player biding to scree
             if(y>475)
                   y=0;
             if(x>635)
                   x=0;
             if(y<0)
                   y=475;
             if(x<0)
                   x=635;
        
        if(score/30>30*BonusMultiplier){
        life++;
        gravity+=5;
        BonusMultiplier++;
        red=rand()%255;blue=rand()%255;green=rand()%255;
        for(i=1;i<=numberofcanons;i++){//escojo el numero 1 para que asi el index sea el numero del ca~on
        height[i]=rand()%8+20/i;
        weidht[i]=height[i];}}

        canonnumber=x*numberofcanons/635 +1;

        centerofcanonx=((635*canonnumber)/numberofcanons)-635/(numberofcanons*numberofcanons);
        for(i=1;i<=numberofcanons;i++){
        bulletsy[i]+=gravity-i;
        circlefill(buffer,(635*i)/numberofcanons-635/(numberofcanons*numberofcanons)+weidht[i] , bulletsy[i]+weidht[i], weidht[i], makecol(red*i/numberofcanons,green*i/numberofcanons,blue*i/numberofcanons));
        textprintf(buffer,font,(635*i)/numberofcanons-635/(numberofcanons*numberofcanons),470,makecol(255,255,255),"%d",i);
        if(bulletsy[i]>=480)
        bulletsy[i]=0;
        else if(bulletsy[i]<0)
        bulletsy[i]=480;
        }
        
        score=((double)(clock() - tStart)/CLOCKS_PER_SEC)*30;
        textprintf(buffer,font,0,0,makecol(255,255,255),"players life %d",life);
        textprintf(buffer,font,150,0,makecol(255,255,255),"score %d",score);
        textprintf(buffer,font,300,0,makecol(255,255,255),"gravity %d",gravity);
        circlefill(buffer, x+5/2, y+5/2, 5/2 , makecol(255,0,255));
        blit(buffer,screen,0,0,0,0,640,480);  
        clear_bitmap(buffer); 
        rest(30);
        }
        
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();
