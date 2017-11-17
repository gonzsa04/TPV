//practicando con SDL
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
class Game;
#include <iostream>
#include <vector>
using namespace std;

struct Punto 
{
	int x, y;
};

//clase Fantasma que rige el comportamiento de los enemigos
class Fantasma {
private:
	int posIniX, posIniY;//posicion inicial
	Texture* texture = nullptr;//puntero a su textura. La carga Game y luego se la pasa 
	SDL_Renderer* renderer = nullptr;//puntero a renderer, Game se lo pasa
	Game* game;//puntero a game para pedirle textura y renderer
	int dirX, dirY;//direccion actual
	vector<Punto>posiblesDir = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0} };
	int fil, col;//informacion de filas y columnas de la textura para ayudar a pintar
	int numTicks, numFrames;//velocidad de la animacion y numero de frames que tiene
	SDL_Rect destRect;//rectangulo destino donde se pintara el fantasma
public:
	Fantasma();//constructora por defecto
   //constructora que inicializa todos los campos de la entidad vistos anteriormente
	Fantasma(Game* game, int width, int height, int f, int c, int numT, int numF);
	void render();//metodo que manda pintar al fantasma
	void animate();//metodo que manda animar al fantasma
	void update();//metodo que actualiza la posicion del fantasma
	void morir();//metodo llamado al morir, devuelve al fantasma a posIni
	void eliminaDir(int x, int y);
	bool hayFantasma(int dX, int dY);
	void siguienteDir(int newDirX, int newDirY);//establece la siguiente direccion del fantasma
	void setPos(int posY, int posX);//establece la posicion del fantasmas
	int getPosX();
	int getPosY();
};

