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
//si es comible pintara uno y si no otro
void Fantasma::render()
{
	if(!comible) texture->renderFrame(renderer, destRect, fil, col);
	else texture->renderFrame(renderer, destRect, 4, 1);
}

//manda a la textura de Fantasma que se anime
void Fantasma::animate()
{
	texture->animation(renderer, destRect, numTicks, numFrames);
}

//actualiza la posicion de Fantasma a traves de sus direcciones actual y siguiente
void Fantasma::update()
{
	posiblesDirecciones();//establecemos las posibles direcciones que puede tomar el fantasma

	//escogemos una al azar y hacemos que vaya en esa direccion
	int randDir = rand() % posiblesDir.size();
	dirX = posiblesDir[randDir].x;
	dirY = posiblesDir[randDir].y;
	destRect.x += dirX;
	destRect.y += dirY;

	toroide();//controlamos si se sale del mapa
}

//llamado cuando pacman le come o cuando pacman pierde una vida
void Fantasma::morir()
{
	destRect.x = posIniX;//vuelve a su posicion inicial
	destRect.y = posIniY;
	comible = false;//deja de ser comible
}

//elimina la direccion contraria a la que lleva de la lista de posibles direcciones
void Fantasma::eliminaDir(int x, int y) 
{
	int i = 0;
	if (x != 0)x = -x;
	else y = -y;
	while (i < (int)posiblesDir.size() - 1 && (posiblesDir[i].x != x || posiblesDir[i].y != y))i++;
	posiblesDir.erase(posiblesDir.begin() + i);
}

//indica si hay fantasma o no en la siguiente casilla donde va a avanzar el fantasma
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

//rellena un vector con todas las posibles direcciones que puede tomar el fantasma
void Fantasma::posiblesDirecciones() 
{
	int i = 0;
	posiblesDir.resize(1);
	if (game->nextCell(destRect.x, destRect.y, game->getTam(), 0) != muro && !hayFantasma(game->getTam(), 0))
	{
		//si hacia la derecha no hay muro y no hay fantasmas, se añade derecha como posible direccion
		posiblesDir[i].x = game->getTam();
		posiblesDir[i].y = 0;
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, -game->getTam(), 0) != muro && !hayFantasma(-game->getTam(), 0))
	{
		//igual con izquierda, arriba y abajo
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

	if (i > 1) eliminaDir(dirX, dirY);//si hay mas de una posible direccion eliminamos la direccion opuesta a la que llevabamos
	//para que el fantasma no se de la vuelta sin motivo
	if (i != 0) { posiblesDir.erase(posiblesDir.begin() + posiblesDir.size() - 1); }//eliminamos la ultima direccion que estara vacia
}

//controla cuando los fantasmas se salen por los extremos del mapa
void Fantasma::toroide() 
{
	if (destRect.x >= (game->getTabCols()) * game->getTam())destRect.x = 0;//si salimos por la derecha entramos por la izquierda
	else if (destRect.x < 0)destRect.x = game->getTabCols() * game->getTam() - game->getTam();//y viceversa
	if (destRect.y >= (game->getTabFils()) * game->getTam())destRect.y = 0;//si salimos por abajo entramos por arriba
	else if (destRect.y < 0)destRect.y = game->getTabFils() * game->getTam() - game->getTam();//y viceversas
}

//establece la posicion del fantasma
void Fantasma::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}

//devuelven posicion del fantasma
int Fantasma::getPosX()
{
	return destRect.x;
}

int Fantasma::getPosY()
{
	return destRect.y;
}

//devuelven direccion del fantasma
int Fantasma::getDirX()
{
	return dirX;
}

int Fantasma::getDirY()
{
	return dirY;
}

//establece el fantasma a comible o no comible
void Fantasma::modifyComible(bool esComible)
{
	comible = esComible;
}

//devuelve si el fantasma es comible o no
bool Fantasma::getComible() { return comible; }