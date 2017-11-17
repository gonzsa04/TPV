#include "PacMan.h"
#include "Game.h"

//constructora por defecto
PacMan::PacMan()
{
	destRect.w = destRect.h = destRect.x = destRect.y = dirX = dirY = fil = col = numTicks = numFrames = 0;
}

//constructora que establece todos los atributos de PacMan
PacMan::PacMan(Game* game, int width, int height, int f, int c, int numT, int numF)
{
	this->game = game;
	texture = game->getTexture();
	renderer = game->getRenderer();
	destRect.w = width;//tamaño de pacman
	destRect.h = height;
	dirX = 0;
	dirY = 0;
	fil = f;
	col = c;
	numTicks = numT;
	numFrames = numF;
	vidas = 3;
	dirXSig = 0;
	dirYSig = 0;
}

//manda a la textura de pacman que pinte un frame a elegir
void PacMan::render()
{
	texture->renderFrame(renderer, destRect, fil, col);
}

//manda a la textura de pacman que se anime
void PacMan::animate()
{
	texture->animation(renderer, destRect, numTicks, numFrames);
}

//actualiza la posicion de pacman a traves de sus direcciones actual y siguiente
void PacMan::update()
{
	for (int i = 0; i < 4; i++)
		if (destRect.x == game->getFantasmas(i).getPosX() && destRect.y == game->getFantasmas(i).getPosY())
			morir();

	//si no hay un muro en la direccion siguiente, toma esa nueva direccion
	if (game->nextCell(destRect.x, destRect.y, dirXSig, dirYSig) != muro)
	{
		dirX = dirXSig;
		dirY = dirYSig;
	}
	//si no hay un muro en la direccion actual, avanzamos en esa direccion
	if (game->nextCell(destRect.x, destRect.y, dirX, dirY) != muro)
	{
		//si hay comida o vitamina nos las comemos
		if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == comida
			|| game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)
		{
			game->setCell((destRect.x + dirX) / game->getTam(), (destRect.y + dirY) / game->getTam(), vacio);
			game->setComida(-1);
		}
		destRect.x += dirX;
		destRect.y += dirY;
	}
	if (destRect.x >= (game->getTabCols()) * game->getTam())destRect.x = 0;//si salimos por la derecha entramos por la izquierda
	else if (destRect.x < 0)destRect.x = game->getTabCols() * game->getTam() - game->getTam();//y viceversa
	if (destRect.y >= (game->getTabFils()) * game->getTam())destRect.y = 0;//si salimos por abajo entramos por arriba
	else if (destRect.y < 0)destRect.y = game->getTabFils() * game->getTam() - game->getTam();//y viceversa
}

//llamado cuando perdemos una vida
void PacMan::morir()
{
	vidas--;//nos resta una vida y nos lleva a la posicion inicial
	destRect.x = posIniX;
	destRect.y = posIniY;
}

//establece la siguiente direccion a tomar (la tomaremos cuando podamos hacerlo)
void PacMan::siguienteDir(int newDirX, int newDirY)
{
	dirXSig = newDirX;
	dirYSig = newDirY;
}

//establece la posicion de pacman
void PacMan::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}

int PacMan::getPosX()
{
	return destRect.x;
}

int PacMan::getPosY()
{
	return destRect.y;
}

//devuelve el numero de vidas restantes
int PacMan::getVidas() { return vidas; }
