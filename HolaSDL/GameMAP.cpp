#include <fstream>
#include "GameMAP.h"
#include "Game.h"

using namespace std;

GameMAP::GameMAP()
{
	this->fils = 0;
	this->cols = 0;
}

GameMAP::GameMAP(int fils, int cols, Game* game)
{
	this->fils = fils;
	this->cols = cols;
	this->game = game;
	cells = new MapCell*[fils];
	for (int i = 0; i < fils; i++)cells[i] = new MapCell[cols];
	textures = game->getTexture();
	renderer = game->getRenderer();
}

MapCell GameMAP::getCell(int fils, int cols)
{
	return cells[fils][cols];
}

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
			if ((int)character < 4)
				cells[i][j] = (MapCell)character;
		}
	}
	archivo.close();
}

void GameMAP::render(string filename)
{
	leeArchivo(filename);
	for (int i = 0; i < fils; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (cells[i][j] == vacio)
			{
				SDL_Rect destRect;
				destRect.x = cols;
				destRect.y = fils;
				destRect.w = destRect.h = 1;
				textures->renderFrame(renderer, destRect, 3, 12);
			}
			else if (cells[i][j] == muro)
			{
				SDL_Rect destRect;
				destRect.x = cols;
				destRect.y = fils;
				destRect.w = destRect.h = 1;
				textures->renderFrame(renderer, destRect, 3, 13);
			}
			else if (cells[i][j] == comida)
			{
				SDL_Rect destRect;
				destRect.x = cols;
				destRect.y = fils;
				destRect.w = destRect.h = 1;
				textures->renderFrame(renderer, destRect, 3, 13);
			}
			else if (cells[i][j] == vitamina)
			{
				SDL_Rect destRect;
				destRect.x = cols;
				destRect.y = fils;
				destRect.w = destRect.h = 1;
				textures->renderFrame(renderer, destRect, 3, 13);
			}
		}
	}
}

GameMAP::~GameMAP()
{
	if (cells != nullptr) {
		for (int i = 0; i < fils; i++)delete[] cells[i];
		delete[] cells;
	}
}