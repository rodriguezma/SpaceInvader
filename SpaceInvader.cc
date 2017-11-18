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
 SoLoud::Wav disparo, movimiento[4],Pdeath,Edeath,ufo;
 
 esat::SpriteHandle hoja, explosion,explosionNave;
 esat::SpriteHandle invA[2], invB[2], invC[2];
 esat::SpriteHandle barrera[5];
 esat::SpriteTransform ST[4];
 int Wwhidth=800;
 int Wheight=600;

 int auxMov=1;
 int auxM=4;
 int temp=0;
 float pointsSTR=0;
 char puntuacion[10];
 int movsound=0;

//////ESTRUCTURAS//////
 
 struct Player{

    int x=400;
    int y=500;
    esat::SpriteHandle sprite;
    bool shooting=false;
    int lives=4;

  }player1;
 
  struct marcianitos{
	
  int x, y;
  int tipo, anim=0;
  int r;            //factor random para disparar
  bool shooting, vivo=true;
  bool frontline=false;
  esat::SpriteHandle sprite;
	
}enemies[50];

struct tDisparo{      //Estructura de los disparos

  esat::SpriteHandle sprite;  
  int px1,py1;      //Esquina superior izq
  int px2,py2;      //Esquina inferior der
  int vel=4;        //velocidad del disparo
  bool activ=false;   
  int InxM;    

} shoot[10];

struct tDisparo j1disparo;

  struct especial{
	 int sx=50, sy=70;
	 esat::SpriteHandle red;
	 bool activ=false;
	 int dir=0;
  }navesp;
  
  struct Proteccion{
	  int mx,my=450;
	  int estado=0;
	  esat::SpriteHandle protec;  
  }muro[4];

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

void CargarMuro(){
	barrera[0]= esat::SubSprite(hoja, 44, 32, 44, 32);
	barrera[1]= esat::SubSprite(hoja, 0, 0, 44, 32);
	barrera[2]= esat::SubSprite(hoja, 44, 0, 44, 32);
	barrera[3]= esat::SubSprite(hoja, 88, 0, 36, 32);
	barrera[4]= esat::SubSprite(hoja, 0, 32, 44, 32);	
}

void CargaSprites(){
    hoja=esat::SpriteFromFile("./Recursos/Imagenes/spritebox-sprite.png");
    player1.sprite=esat::SubSprite(hoja, 48, 64, 26, 16);
    navesp.red=esat::SubSprite(hoja, 0, 64, 48, 20);

    for(int i=0;i<10;i++){
        shoot[i].sprite=esat::SubSprite(hoja, 118, 32, 6, 16);
    }
    j1disparo.sprite=esat::SubSprite(hoja, 118, 32, 6, 16);
    CargarEnemigos();

    explosion=esat::SubSprite(hoja,74,64,26,16);
    explosionNave=esat::SubSprite(hoja,88,32,30,17);
    CargarMuro();

		  
    }
	
void CargarAudio(){
	Edeath.load ("./Recursos/InvadersSounds/shoot.wav");
	Pdeath.load ("./Recursos/InvadersSounds/explosion.wav");
	disparo.load ("./Recursos/InvadersSounds/invaderkilled.wav");
	ufo.load("./Recursos/InvadersSounds/ufo_lowpitch.wav");
	movimiento[0].load("./Recursos/InvadersSounds/fastinvader1.wav");
	movimiento[1].load("./Recursos/InvadersSounds/fastinvader2.wav");
	movimiento[2].load("./Recursos/InvadersSounds/fastinvader3.wav");
	movimiento[3].load("./Recursos/InvadersSounds/fastinvader4.wav");
}

  
   
void EscalarMuro(){
	ST[0].x=105;
	for (int i=0; i<4; ++i){
		ST[i].y=430;
		ST[i].scale_x= 2;
		ST[i].scale_y= 1.5;
		muro[i].protec= barrera[muro[i].estado];
		if (i>0)
			ST[i].x= ST[i-1].x+170;
		
		}	
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
	if(temp>25)
		temp=0;
		
	}
	
void SonidoMov(){
	if (temp==25){
		canal.play(movimiento[movsound]);
		++movsound%=4;
	}	
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

    for (int i=40;i<50;i++)
      enemies[i].frontline=true;


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
		  canal.play(ufo);
	  }
  }
  
void MostrarEspecial(){
	  
	  if (navesp.activ == true && navesp.sx < 700 && navesp.dir == 0){
		  esat::DrawSprite (navesp.red, navesp.sx, navesp.sy);
		  navesp.sx+=10;
		  
	  }else if (navesp.activ == true && navesp.sx >= 700 && navesp.dir == 0){
		  navesp.dir=1; navesp.activ=false;
	  }
    if (navesp.activ == true && navesp.sx > 100 && navesp.dir == 1){
		  esat::DrawSprite (navesp.red, navesp.sx, navesp.sy);
		  navesp.sx-=10;
	  }else if (navesp.activ == true && navesp.sx <= 100 && navesp.dir == 1){
		  navesp.dir=0; navesp.activ=false;
	  }
		
}
	  

 void UpdateFrame(){
		
	esat::DrawText(100,50,"SCORE:");
	esat::DrawText(175,50,itoa(pointsSTR,puntuacion,10));
	esat::DrawText(580,50,"LIVES:");
	MostrarEspecial();


  esat::DrawSprite(player1.sprite,player1.x,player1.y);
   
  for(int i=0;i<50;i++){
    if(enemies[i].vivo)
    esat::DrawSprite(enemies[i].sprite,enemies[i].x,enemies[i].y); 
  
  if(player1.shooting){
    esat::DrawSprite(j1disparo.sprite,j1disparo.px1,j1disparo.py1);
  }
  
  for(int i=0;i<50;i++)
    if(enemies[i].shooting){
      esat::DrawSprite(shoot[i%10].sprite,shoot[i%10].px1,shoot[i%10].py1);
    }
  }
    for(int i=0; i<4; ++i)
    esat::DrawSprite(muro[i].protec, ST[i]);
}

void ShootPlayer(){

    if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)&& !player1.shooting){
      player1.shooting = true;
      j1disparo.activ = true;
      j1disparo.px1=player1.x + (esat::SpriteWidth(player1.sprite)/2)-esat::SpriteWidth(j1disparo.sprite)/2;
      j1disparo.py1=player1.y - esat::SpriteHeight(j1disparo.sprite);
      j1disparo.px2=j1disparo.px1 + esat::SpriteWidth(j1disparo.sprite);
      j1disparo.py2=j1disparo.py1 + esat::SpriteHeight(j1disparo.sprite);
	  canal.play(disparo);

    }
}

void ShootEnemy(){

    for(int i=0;i<50;i++){
      enemies[i].r=rand()%1000;
      if( enemies[i].r<=5 ){
        if(enemies[i].vivo && !enemies[i].shooting && enemies[i].frontline){
          shoot[i%10].activ=true;
          enemies[i].shooting=true;
          shoot[i%10].px1=enemies[i].x+(esat::SpriteWidth(enemies[i].sprite)/2)-esat::SpriteWidth(shoot[i%10].sprite)/2;
          shoot[i%10].py1=enemies[i].y+(esat::SpriteHeight(enemies[i].sprite));
          shoot[i%10].px2=shoot[i%10].px1+esat::SpriteWidth(shoot[i%10].sprite);
          shoot[i%10].py2=shoot[i%10].py1+esat::SpriteHeight(shoot[i%10].sprite);
          shoot[i%10].InxM=i;
        }

      }
   }
 }

void MovDisparos(){

if (j1disparo.activ){
  j1disparo.py1-=j1disparo.vel;
  j1disparo.py2-=j1disparo.vel;
}

for(int i=0;i<50;i++){
  if(enemies[i].shooting){
    shoot[i%10].py1+=shoot[i%10].vel;
    shoot[i%10].py2+=shoot[i%10].vel;
  }
}
}

void BordesDisparos(){

      if(j1disparo.activ && j1disparo.py1<0){
        player1.shooting = false;
        j1disparo.activ = false;
      }

      for(int i=0;i<10;i++){
        if(shoot[i].activ && shoot[i].py2>600){
          enemies[shoot[i].InxM].shooting = false;
          shoot[i].activ = false;
        }
      }

}

void ColMarcianos(){

  if(j1disparo.activ){
    for(int j=0;j<50;j++){
      if(Col(j1disparo.px1,j1disparo.py1,
        j1disparo.px2,j1disparo.py2,
        enemies[j].x,enemies[j].y,
        enemies[j].x + esat::SpriteWidth(enemies[j].sprite),
        enemies[j].y + esat::SpriteHeight(enemies[j].sprite)) && enemies[j].vivo){
          player1.shooting=false;
          j1disparo.activ=false;
          enemies[j].vivo=false;
          esat::DrawSprite(explosion,enemies[j].x,enemies[j].y);
		  canal.play(Edeath);
        
          if(enemies[j].frontline){
            int a=j-10;
            while(!enemies[a].vivo)
              a-=10;
            if(a>=0)
              enemies[a].frontline=true;
          }
        }
      }
    }
  }

void ColPlayer(){
  for(int i=0;i<10;i++){
    if(shoot[i].activ && Col(shoot[i].px1,shoot[i].py1,
        shoot[i].px2,shoot[i].py2,
        player1.x,player1.y,
        player1.x + esat::SpriteWidth(player1.sprite),
        player1.y + esat::SpriteHeight(player1.sprite))){
          --player1.lives;
          esat::DrawSprite(explosionNave,player1.x-2,player1.y-1);
          esat::Sleep(3000);
          player1.x=400;

    }
  }
}

void ColMuros(){

    //x -> 2
    //y -> 1.5
}

int esat::main(int argc, char **argv) {
  
  double current_time,last_time;
  unsigned char fps=25;

  
  canal.init();
  
 
  esat::WindowInit(Wwhidth,Wheight);
  WindowSetMouseVisibility(true);

  CargaSprites();
  InitMarcianos();
  InitAnimacion();
  EscalarMuro();
  CargarAudio();
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

  ShootPlayer();
  ShootEnemy();
  MovDisparos();
  BordesDisparos();
  ColMarcianos();
  ColPlayer();
  SonidoMov();


    UpdateFrame();
	
	Temporizacion();


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
