#include "PacMan.h"
#include "Game.h"

//constructora por defecto
PacMan::PacMan() 
{
	destRect.w = destRect.h = destRect.x = destRect.y = dirX = dirY = fil = col = numTicks = numFrames = 0;
}

//establece todos los atributos de PacMan
PacMan::PacMan(Game* game, int width, int height, int posX, int posY, int dX,
	int dY, int f, int c, int numT, int numF)
{
	this->game = game;
	texture = game->getTexture();
	renderer = game->getRenderer();
	destRect.w = width;
	destRect.h = height;
	destRect.x = posIniX = posX*20;
	destRect.y = posIniY = posY*20;
	dirX = dX;
	dirY = dY;
	fil = f;
	col = c;
	numTicks = numT;
	numFrames = numF;
	vidas = 3;
	dirXSig = 0;
	dirYSig = 0;
}

//manda a la textura de la entidad que pinte un frame a elegir
void PacMan::render()
{
	texture->renderFrame(renderer, destRect, fil, col);
}

//manda a la textura de la entidad que se anime
void PacMan::animate()
{
	texture->animation(renderer, destRect, numTicks, numFrames);
}

//actualiza la posicion de la entidad a traves de su direccion y velocidad
void PacMan::update()
{
	if (game->nextCell(destRect.x, destRect.y, dirXSig, dirYSig)) 
	{
		dirX = dirXSig;
		dirY = dirYSig;
	}
	if (game->nextCell(destRect.x, destRect.y, dirX, dirY)) 
	{
		destRect.x += dirX * 20;
		destRect.y += dirY * 20;
	}
}

void PacMan::morir()
{
	vidas--;
	destRect.x = posIniX;
	destRect.y = posIniY;
}

void PacMan::siguienteDir(int newDirX, int newDirY) 
{
	dirXSig = newDirX;
	dirYSig = newDirY;
}

int PacMan::getVidas() { return vidas; }
