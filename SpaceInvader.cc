#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include <stdio.h>
#include <stdlib.h>

 SoLoud::Soloud canal;
 //Declaración variables canal audio.
 SoLoud::Wav ejemplo1,ejemplo2;
 
 esat::SpriteHandle hoja;
 esat::SpriteHandle invA[2], invB[2], invC[2];
 int points = 0;
 char pointsSTR[25];
//////ESTRUCTURAS//////
 
 struct Player{

    int x=400;
    int y=500;
    esat::SpriteHandle sprite;

  }player1;
 
  struct marcianitos{
	
  int x, y;
  int tipo;
  esat::SpriteHandle sprite;
	
}enemies[50];



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

struct tDisparo{			//Estructura de los disparos

	esat::SpiteHandle cuerpo	
	int px1,py1;			//Esquina superior izq
	int px2,py2;			//Esquina inferior der
	int id;				//quien dispara 0=jugador | 1=marciano

}*/


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

    CargarEnemigos();

      for (int i=0; i<50; ++i){
    if (i< 10){
      enemies[i].sprite = invC[0];
    
    }else if (i>=10 && i<30){
      enemies[i].sprite = invB[0];
      
    }else if (i>=30 && i<50){
      enemies[i].sprite = invA[0];
    }
  } 

}

void InitMarcianos(){
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
      enemies[i*10+j].x=200+j*30;
      enemies[i*10+j].y=100+i*25;

    }
  } 
}


  void MovPlayer(void){

    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)&& player1.x>200){

      player1.x-=6;

    }else if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) && player1.x + esat::SpriteWidth(player1.sprite) < 600){
      player1.x+=6;

    }

  }
  
  


  void UpdateFrame(){
	
	esat::DrawText(100,50,"SCORE:");
	esat::DrawText(175,50,pointsSTR);
	esat::DrawText(580,50,"LIVES:");
	
    esat::DrawSprite(player1.sprite,player1.x,player1.y);
    for(int i=0;i<50;i++)
      esat::DrawSprite(enemies[i].sprite,enemies[i].x,enemies[i].y);
  
    

  }

int esat::main(int argc, char **argv) {
  
  


  double current_time,last_time;
  unsigned char fps=25;

  //Inicialicización sistema audio.
  //canal.init();
  //Carga audio en cada variable canal audio.
  //ejemplo1.load("./Recursos/Audio/ogg/dp_frogger_extra.ogg");
  //ejemplo2.load("./Recursos/Audio/ogg/dp_frogger_start.ogg");
 
  esat::WindowInit(800,600);
  WindowSetMouseVisibility(true);

  CargaSprites();
  InitMarcianos();
  
  esat::DrawSetTextFont("./Recursos/Fuentes/space_invaders.ttf");
  esat::DrawSetTextSize(25.0f);
  esat::DrawSetFillColor(255,255,255);

  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    
    last_time = esat::Time();
		itoa(points,pointsSTR,10);
    esat::DrawBegin();
    esat::DrawClear(0,0,0);
	

    MovPlayer();
  
	UpdateFrame();





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