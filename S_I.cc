

struct marcianitos{
	int x, y;
	
};
typedef struct marcianitos marcianitos
marcianitos enemies[50];

	esat::SpriteHandle hoja, invA[2], invB[2], invC[2];

	hoja=esat::SpriteFromFile("./Tanques/Recursos/Imagenes/SpaceInvaders/spritebox-sprite.png");
	
	invA[0]= esat::SubSprite(hoja, 0, 85, 24, 16);
	invA[1]= esat::SubSprite(hoja, 24, 85, 24, 16);