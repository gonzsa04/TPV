#include <fstream>
#include "GameMAP.h"
#include "Game.h"

using namespace std;

//constructora por defecto
GameMAP::GameMAP()
{
	this->fils = 0;
	this->cols = 0;
}

//constructora "de verdad"
GameMAP::GameMAP(int fils, int cols, Game* game)
{
	//inicializa sus parametros
	this->fils = fils;
	this->cols = cols;
	this->game = game;
	cells = new MapCell*[fils];//da tamaño al array de casillas
	for (int i = 0; i < fils; i++)cells[i] = new MapCell[cols];
	textures = game->getTexture();//coge el renderer y la textura de game
	renderer = game->getRenderer();
}

//devuelve la casilla de la poscion dada
MapCell GameMAP::getCell(int fils, int cols)
{
	return cells[fils][cols];
}

//lee de archivo un mapa y modifica el array de casillas para que sea igual
//llamado desde render
void GameMAP::leeArchivo(string filename)
{
	ifstream archivo;
	char character;
	archivo.open("./Levels/" + filename);
	archivo >> fils >> cols;

	for (int i = 0; i < fils; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			archivo >> character;
			character -= 48;
			if ((int)character < 4)
				cells[i][j] = (MapCell)(int)character;
			else
				cells[i][j] = (MapCell)0;
		}
	}
	archivo.close();
}

//pinta el mapa dado a un tamaño dado
void GameMAP::render(string filename, int tamanyo)
{
	leeArchivo(filename);//lee archivo y modifica casillas
	//lee todas las casillas y pinta el sprite correspondiente de cada una
	for (int i = 0; i < fils; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (cells[i][j] == vacio)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 5, 3);
			}
			else if (cells[i][j] == muro)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 5, 0);
			}
			else if (cells[i][j] == comida)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 4, 2);
			}
			else if (cells[i][j] == vitamina)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 4, 3);
			}
			SDL_RenderPresent(renderer);
		}
	}
}

//destructora del tablero
/*GameMAP::~GameMAP()
{
	if (cells != nullptr) {
		for (int i = 0; i < fils; i++)delete[] cells[i];
		delete[] cells;
	}
}*/