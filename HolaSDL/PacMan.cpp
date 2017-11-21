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
	texture = game->getTexture(1);
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
	texture->animation(renderer, destRect, angle, numTicks, numFrames);
}

//actualiza la posicion de pacman a traves de sus direcciones actual y siguiente
void PacMan::update()
{
	//si no hay un muro en la direccion siguiente, toma esa nueva direccion
	if (game->nextCell(destRect.x, destRect.y, dirXSig, dirYSig) != muro)
	{
		dirX = dirXSig;
		dirY = dirYSig;
		if (dirX < 0)Gira(180);//giramos a pacman en la direccion correspondiente
		else if (dirX > 0)Gira(0);
		else if (dirY < 0)Gira(270);
		else if (dirY > 0)Gira(90);
	}
	//si no hay un muro en la direccion actual, avanzamos en esa direccion
	if (game->nextCell(destRect.x, destRect.y, dirX, dirY) != muro)
	{
		//si hay comida o vitamina nos las comemos
		if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == comida
			|| game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)
		{
			//si es una vitamina ponemos todos los fantasmas a comible
			if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)game->fantasmasComibles(true);
			game->setCell((destRect.x + dirX) / game->getTam(), (destRect.y + dirY) / game->getTam(), vacio);
			game->setComida(-1);
			game->addScore(10);
		}
		destRect.x += dirX;
		destRect.y += dirY;
	}

	toroide();//miramos si se sale del mapa
}

//controla cuando pacman se sale de los bordes del mapa
void PacMan::toroide()
{
	if (destRect.x >= (game->getTabCols()) * game->getTam())destRect.x = 0;//si salimos por la derecha entramos por la izquierda
	else if (destRect.x < 0)destRect.x = game->getTabCols() * game->getTam() - game->getTam();//y viceversa
	if (destRect.y >= (game->getTabFils()) * game->getTam())destRect.y = 0;//si salimos por abajo entramos por arriba
	else if (destRect.y < 0)destRect.y = game->getTabFils() * game->getTam() - game->getTam();//y viceversa
}

//llamado cuando perdemos una vida
void PacMan::morir()
{
	vidas--;//nos resta una vida y nos lleva a la posicion inicial
	if (vidas <= 0) game->GameOver();//si nos quedamos sin vidas perdemos
	else
	{
		dirX = dirY = dirXSig = dirYSig = 0;
		destRect.x = posIniX;
		destRect.y = posIniY;
	}
}

//establece la siguiente direccion a tomar (la tomaremos cuando podamos hacerlo)
void PacMan::siguienteDir(int newDirX, int newDirY)
{
	dirXSig = newDirX;
	dirYSig = newDirY;
}

void PacMan::Gira(double ang)
{
	angle = ang;
}

//establece la posicion de pacman
void PacMan::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}

//devuelven posicion del pacman
int PacMan::getPosX()
{
	return destRect.x;
}

int PacMan::getPosY()
{
	return destRect.y;
}

//devuelven direccion del pacman
int PacMan::getDirX()
{
	return dirX;
}

int PacMan::getDirY()
{
	return dirY;
}

//devuelve el numero de vidas restantes
int PacMan::getVidas() { return vidas; }
