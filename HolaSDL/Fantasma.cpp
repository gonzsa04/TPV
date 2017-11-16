#include "Fantasma.h"
#include "Game.h"

//constructora por defecto
Fantasma::Fantasma()
{
	destRect.w = destRect.h = destRect.x = destRect.y = dirX = dirY = fil = col = numTicks = numFrames = 0;
}

//constructora que establece todos los atributos de Fantasma
Fantasma::Fantasma(Game* game, int width, int height, int f, int c, int numT, int numF)
{
	this->game = game;
	texture = game->getTexture();
	renderer = game->getRenderer();
	destRect.w = width;//tamaño de fantasma
	destRect.h = height;
	Punto dirIni = posiblesDir[rand() % 4];
	dirX = dirIni.x * game->getTam();
	dirY = dirIni.y * game->getTam();
	fil = f;
	col = c;
	numTicks = numT;
	numFrames = numF;
}

//manda a la textura de fantasma que pinte un frame a elegir
void Fantasma::render()
{
	texture->renderFrame(renderer, destRect, fil, col);
}

//manda a la textura de Fantasma que se anime
void Fantasma::animate()
{
	texture->animation(renderer, destRect, numTicks, numFrames);
}

//actualiza la posicion de Fantasma a traves de sus direcciones actual y siguiente
void Fantasma::update()
{
	int i = 0;
	posiblesDir.resize(1);
	if (game->nextCell(destRect.x, destRect.y, game->getTam(), 0) != muro && !hayFantasma(game->getTam(), 0))
	{
		posiblesDir[i].x = game->getTam();
		posiblesDir[i].y = 0;
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, -game->getTam(), 0) != muro && !hayFantasma(-game->getTam(), 0))
	{
		posiblesDir[i].x = -game->getTam();
		posiblesDir[i].y = 0;
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, 0, game->getTam()) != muro && !hayFantasma(0, game->getTam()))
	{
		posiblesDir[i].x = 0;
		posiblesDir[i].y = game->getTam();
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, 0, -game->getTam()) != muro && !hayFantasma(0, -game->getTam()))
	{
		posiblesDir[i].x = 0;
		posiblesDir[i].y = -game->getTam();
		i++;
		posiblesDir.resize(i + 1);
	}
	if (i > 1) 
	{
		eliminaDir(dirX, dirY);
	}

	int randDir = rand() % posiblesDir.size();
	dirX = posiblesDir[randDir].x;
	dirY = posiblesDir[randDir].y;
	destRect.x += dirX;
	destRect.y += dirY;
}

//llamado cuando perdemos una vida
void Fantasma::morir()
{
	destRect.x = posIniX;
	destRect.y = posIniY;
}

void Fantasma::eliminaDir(int x, int y) 
{
	int i = 0;
	if (x != 0)x = -x;
	else y = -y;
	while (i < posiblesDir.size() && (posiblesDir[i].x != x && posiblesDir[i].y != y))i++;
	posiblesDir.erase(posiblesDir.begin() + i);
}

bool Fantasma::hayFantasma(int dX, int dY) 
{
	int i = 0;
	while (i < 4 && (game->getFantasmas(i).destRect.x != destRect.x + dX || 
		game->getFantasmas(i).destRect.y != destRect.y + dY)) i++;
	return (i < 4);
}

//establece la siguiente direccion a tomar (la tomaremos cuando podamos hacerlo)
void Fantasma::siguienteDir(int newDirX, int newDirY)
{
	dirX = newDirX;
	dirY = newDirY;
}

//establece la posicion del fantasma
void Fantasma::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}
