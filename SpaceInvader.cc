#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 SoLoud::Soloud canal;
 //Declaración variables canal audio.
 SoLoud::Wav ejemplo1,ejemplo2;
 
 esat::SpriteHandle hoja;
 esat::SpriteHandle invA[2], invB[2], invC[2];

 int Wwhidth=800;
 int Wheight=600;

 int auxMov=1;
 int auxM=4;
 int temp=0;
 int shootcount=1;

//////ESTRUCTURAS//////
 
struct Player{

    int x=400;
    int y=500;
    esat::SpriteHandle sprite;
    bool shooting=false;

}player1;
 
struct marcianitos{
	
  int x, y;
  int tipo, anim=0;
  int r;            //factor random para disparar
  bool shooting, vivo=true;
  esat::SpriteHandle sprite;
	
}enemies[50];

struct tDisparo{      //Estructura de los disparos

  esat::SpriteHandle sprite;  
  int px1,py1;      //Esquina superior izq
  int px2,py2;      //Esquina inferior der
  int vel=1;        //velocidad del disparo

} shoot[11];



/*struct tJugador{			//Estructura nave del jugador

	esat::SpiteHandle cuerpo[2]	//Sprites de la nave 0=viva | 1=muerta
	int px1,py1;			//Esquina superior izq
	int px2,py2;			//Esquina inferior der
	bool cargado;			//true=listo para disparar | falso=no puede disparar
	int vidas;			//Intentos restantes

}

struct tEnemigo{			//Enemigo base

	esat::SpiteHandle cuerpo[2]	//Sprites de la nave 0 y 1 para la animación
	int px1,py1;			//Esquina superior izq
	int px2,py2;			//Esquina inferior der
	bool cargado=true;		//true=listo para disparar | falso=no puede disparar
	int puntos;			//Cantidad de puntos que se añaden al jugador al destruir a este enemigo
	int velocidad;			//velocidad de desplazamiento de la nave, aunmenta con los niveles

}

struct tEnemigoEspecial{		//Nave de bonificación

	esat::SpiteHandle cuerpo	
	int px1,py1;			//Esquina superior izq
	int px2,py2;			//Esquina inferior der
	int puntos=100;			//Cantidad de puntos que se añaden al jugador al destruir a este enemigo
	int salir;			//Factor random
	bool espera=true;		//true=La nave está en espera | la nave está activa en la pantalla

}

*/


//////FUNCIONES//////
  void CargarEnemigos(){
  
  invA[0]= esat::SubSprite(hoja, 0, 85, 24, 16);
  invA[1]= esat::SubSprite(hoja, 24, 85, 24, 16);
  invB[0]= esat::SubSprite(hoja, 48, 85, 22, 16);
  invB[1]= esat::SubSprite(hoja, 70, 85, 22, 16);
  invC[0]= esat::SubSprite(hoja, 92, 85, 16, 16);
  invC[1]= esat::SubSprite(hoja, 108, 85, 16, 16);

} 

void CargaSprites(){

    hoja=esat::SpriteFromFile("./Recursos/Imagenes/spritebox-sprite.png");
    player1.sprite=esat::SubSprite(hoja, 48, 64, 26, 16);
    for(int i=0;i<10;i++){
        shoot[i].sprite=esat::SubSprite(hoja, 118, 32, 6, 16);
    }

    CargarEnemigos();
	
		  
}
   


void InitAnimacion(){
		for (int i=0; i<50; ++i){
			if(i%2==0)
				enemies[i].anim = 1;
			
    }	
}

	void CambiarAnimacion(){
		
		for (int i=0; i<50; ++i){
	     if (i< 10){
        enemies[i].sprite = invC[enemies[i].anim];
    
        }else if (i>=10 && i<30){
          enemies[i].sprite = invB[enemies[i].anim];
          
        }else if (i>=30 && i<50){
          enemies[i].sprite = invA[enemies[i].anim];
    	   }
    		if(enemies[i].anim==0 && temp==5){
    			++enemies[i].anim;
    		}else if(temp==5)
    			--enemies[i].anim;
		
		}
	}
	
	void Temporizacion(){
		
		++temp;
		if(temp>5)
		temp=0;
		
	}
	

void InitMarcianos(){

  int a=Wwhidth/4;

  for(int i=0;i<5;i++){
   for(int j=0;j<10;j++){
     int t;
     if(i < 1)
      t=3;
    else if(i>=1 && i<3)
      t=2;
      else
      t=1;

      enemies[i*10+j].tipo=t;
      enemies[i*10+j].x=a+5+j*30;
      enemies[i*10+j].y=10+i*25;

    }
  } 
}

void MovMarcianos(){
  
  switch(auxMov){
    case 1:
      for(int i=0;i<10;i++){
        enemies[auxM*10+i].x+=4;
      }
      if(auxM==0)
        auxM=4;
      else
        auxM--;
      break;
    case 2:
     for(int i=0;i<10;i++){
        enemies[auxM*10+i].x-=4;
      }
      if(auxM==0)
        auxM=4;
      else
        auxM--;
      break;
    }

}

void BordeMarcianos(){
  int a=Wwhidth/4;
  int b=a*3;
  
  if(enemies[0].x < a){
    for(int i=0;i<50;i++){
        enemies[i].y+=4;
      }
    auxMov=1;
  }
  else if(enemies[9].x+esat::SpriteWidth(enemies[9].sprite)>b){
    for(int i=0;i<50;i++){
        enemies[i].y+=4;
      }
    auxMov=2;
  } 

}


  void MovPlayer(){

    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)&& player1.x>0){

      player1.x-=6;

    }else if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) && player1.x + esat::SpriteWidth(player1.sprite) < 800){
      player1.x+=6;
    }

  }
  
  
  


void UpdateFrame(){

    esat::DrawSprite(player1.sprite,player1.x,player1.y);
    if(player1.shooting){
      esat::DrawSprite(shoot[0].sprite,shoot[0].px1,shoot[0].py1);
    }
    
    for(int i=0;i<50;i++){
      esat::DrawSprite(enemies[i].sprite,enemies[i].x,enemies[i].y);
      if(enemies[i].shooting){
        esat::DrawSprite(shoot[shootcount].sprite,shoot[shootcount].px1,shoot[shootcount].py1);
      }
    }
      
}

void ShootPlayer(){

    if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)&& !player1.shooting){
      player1.shooting=true;
      shoot[0].px1=player1.x+(esat::SpriteWidth(player1.sprite)/2);
      shoot[0].py1=player1.y;
      shoot[0].px2=shoot[0].px1+esat::SpriteWidth(shoot[0].sprite);
      shoot[0].py2=shoot[0].py1+esat::SpriteHeight(shoot[0].sprite);
    }

    shoot[0].py1-=shoot[0].vel;
    shoot[0].py2-=shoot[0].vel;

    if(shoot[0].py2<0){
      player1.shooting=false;
    }

}

void ShootEnemy(){

    for(int i=0;i<50;i++){
      enemies[i].r=rand()%50;
      if( enemies[i].r<=10 ){
        if((i>=40 && enemies[i].vivo && !enemies[i].shooting) && shootcount<9){
          shootcount++;
          enemies[i].shooting=true;
          shoot[shootcount].px1=enemies[i].x+(esat::SpriteWidth(enemies[i].sprite)/2);
          shoot[shootcount].py1=enemies[i].y+(esat::SpriteHeight(enemies[i].sprite));
          shoot[shootcount].px2=shoot[shootcount].px1+esat::SpriteWidth(shoot[shootcount].sprite);
          shoot[shootcount].py2=shoot[shootcount].py1+esat::SpriteHeight(shoot[shootcount].sprite);
        }

      }
      shoot[shootcount].py1+=shoot[0].vel;
      shoot[shootcount].py2+=shoot[0].vel;
      if(shoot[shootcount].py1>Wheight && shootcount>0){
          enemies[i].shooting=false;
          shootcount--;
          printf("PASA\n");
        }
    }

}


int esat::main(int argc, char **argv) {
  
  


  double current_time,last_time;
  unsigned char fps=25;

  //Inicialicización sistema audio.
  //canal.init();
  //Carga audio en cada variable canal audio.
  //ejemplo1.load("./Recursos/Audio/ogg/dp_frogger_extra.ogg");
  //ejemplo2.load("./Recursos/Audio/ogg/dp_frogger_start.ogg");
 
  esat::WindowInit(Wwhidth,Wheight);
  WindowSetMouseVisibility(true);

  srand(time(NULL));
  CargaSprites();
  InitMarcianos();
  InitAnimacion();

  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    
    last_time = esat::Time();
    esat::DrawBegin();
    esat::DrawClear(0,0,0);
   
	CambiarAnimacion();
    MovPlayer();
    MovMarcianos();
    BordeMarcianos();
    ShootPlayer();
    ShootEnemy();
    UpdateFrame();
    printf("%d\n",shootcount);
	
	Temporizacion();



   /*
    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
      canal.play(ejemplo1); //Inicia reproducción canal
    }
    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
      ejemplo1.stop(); //Finaliza reproducción canal
    }
    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)){
      canal.play(ejemplo2);
    }
    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Space)){
      ejemplo2.stop();
    }
    */
    
    esat::DrawEnd();
	
	do{
    current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
    esat::WindowFrame();
  }
  
  esat::WindowDestroy();
   //Finalización sitema audio.
  canal.deinit();
  return 0;
}
