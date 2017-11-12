#pragma once
class Game;
#include "Texture.h"
#include <iostream>
using namespace std;

enum MapCell { vacio, muro, comida, vitamina };

class GameMAP {
private:
	int fils;
	int cols;
	Game* game;
	MapCell** cells;
	Texture* textures;
	SDL_Renderer* renderer;
public:
	GameMAP();
	GameMAP(int fils, int cols, Game* game);
	MapCell getCell(int fils, int cols);
	void leeArchivo(string filename);
	void render(string filename);
	~GameMAP();
};