#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include <stdio.h>

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
 float pointsSTR=0;
 char puntuacion[10];

//////ESTRUCTURAS//////
 
 struct Player{

    int x=400;
    int y=500;
    esat::SpriteHandle sprite;

  }player1;
 
  struct marcianitos{
	
  int x, y;
  int tipo, anim=0;
  esat::SpriteHandle sprite;
	
  }enemies[50];

  struct especial{
	 int sx=50, sy=70;
	 esat::SpriteHandle red;
	 bool activ=false;
	 
  }navesp;

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

bool Col(int a1x, int a1y, int a2x, int a2y, int b1x, int b1y, int b2x, int b2y){

	if (b2x < a1x || b1x > a2x)
		return false;
	else if (b2y < a1y ||  b1y > a2y)
		return false;
	else
		return true;

}

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
navesp.red=esat::SubSprite(hoja, 0, 64, 48, 21);
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


  for(int i=0;i<5;i++){
   for(int j=0;j<10;j++){
     int t;
     if(i < 1){
		 	enemies[i*10+j].x=100+j*40 + 4;
      t=3;
	 }else if(i>=1 && i<3){
		enemies[i*10+j].x=100+j*40 + 1;
		t=2;
	}
      else{
		  enemies[i*10+j].x=100+j*40;
		t=1;
	  }

      enemies[i*10+j].tipo=t;
	  enemies[i*10+j].y=100+i*25;


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
  int a=100;
  int b=700;
  
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


  void MovPlayer(void){

    if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)&& player1.x>100){

      player1.x-=6;

    }else if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) && player1.x + esat::SpriteWidth(player1.sprite) < 700){
      player1.x+=6;
    }

  }
  
void EnemigoEspecial(){
	  if (rand()%1000+1<3 && navesp.activ==false){
		  navesp.activ=true;
	  }
  }
  
  void MostrarEspecial(){
	  
	  if (navesp.activ==true && navesp.sx< 700){
		  esat::DrawSprite (navesp.red, navesp.sx, navesp.sy);
		  navesp.sx+=8;
		  
	  }else{
		navesp.activ=false; 
		navesp.sx=50;
	  }
	  
  
  }
  
  


  void UpdateFrame(){
	
	
	esat::DrawText(100,50,"SCORE:");
	esat::DrawText(175,50,itoa(pointsSTR,puntuacion,10));
	esat::DrawText(580,50,"LIVES:");
	MostrarEspecial();
	
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
 
  esat::WindowInit(Wwhidth,Wheight);
  WindowSetMouseVisibility(true);

  CargaSprites();
  InitMarcianos();
  InitAnimacion();
  esat::DrawSetTextFont("./Recursos/Fuentes/space_invaders.ttf");
  esat::DrawSetFillColor(255,255,255,255);

  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    
    last_time = esat::Time();
    esat::DrawBegin();
    esat::DrawClear(0,0,0);
   
	CambiarAnimacion();
    MovPlayer();

	MovMarcianos();
    BordeMarcianos();
	EnemigoEspecial();

    UpdateFrame();
	
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
