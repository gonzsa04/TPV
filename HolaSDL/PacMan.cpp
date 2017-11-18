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
	colisiones();//miramos si se colisiona con un fantasma

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
			//si es una vitamina ponemos todos los fantasmas a comible
			if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)game->fantasmasComibles(true);
			game->setCell((destRect.x + dirX) / game->getTam(), (destRect.y + dirY) / game->getTam(), vacio);
			game->setComida(-1);
		}
		destRect.x += dirX;
		destRect.y += dirY;
	}

	toroide();//miramos si se sale del mapa
}

//mira si hay colisiones con algun fantasma
void PacMan::colisiones() 
{
	int i = 0;
	//colisiona con el si sus posiciones son iguales o si su siguiente posicion es igual que la del fantasma y la siguiente posicion del fantasma es la misma que la suya
	while (i < 4 && ((destRect.x != game->getFantasmas(i).getPosX() || destRect.y != game->getFantasmas(i).getPosY()) &&
		((destRect.x + dirX != game->getFantasmas(i).getPosX() || destRect.y + dirY != game->getFantasmas(i).getPosY()) ||
			(destRect.x != game->getFantasmas(i).getPosX() + game->getFantasmas(i).getDirX() || destRect.y != game->getFantasmas(i).getPosY() + game->getFantasmas(i).getDirY()))))
		i++;
	//si hay colision 
	if (i < 4) 
	{
		//si no son comibles se pierde una vida y tanto pacman como los fantasmas vuelven a su posicion inicial
		if (!game->getFantasmas(i).getComible()) 
		{
			morir();
			for (int i = 0; i < 4; i++)game->muereFantasma(i);
		}
		//si son comibles te comes a ese fantasma
		else game->muereFantasma(i);
	}
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

//establece la posicion de pacman
void PacMan::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}

//devuelve el numero de vidas restantes
int PacMan::getVidas() { return vidas; }
