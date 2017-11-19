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
	Texture* texture2 = nullptr;//puntero a su textura. La carga Game y luego se la pasa 
	SDL_Renderer* renderer = nullptr;//puntero a renderer, Game se lo pasa
	Game* game;//puntero a game para pedirle textura y renderer
	int dirX, dirY;//direccion actual
	vector<Punto>posiblesDir = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0} };//vector de posibles direcciones, inicialmente todas
	int fil, col;//informacion de filas y columnas de la textura para ayudar a pintar
	int numTicks, numFrames;//velocidad de la animacion y numero de frames que tiene
	bool comible = false;//inicialmente el fantasma no es comible
	SDL_Rect destRect;//rectangulo destino donde se pintara el fantasma
public:
	Fantasma();//constructora por defecto
   //constructora que inicializa todos los campos de la entidad vistos anteriormente
	Fantasma(Game* game, int width, int height, int f, int c, int numT, int numF);
	void render();//metodo que manda pintar al fantasma
	void animate();//metodo que manda animar al fantasma
	void update();//metodo que actualiza la posicion del fantasma
	void morir();//metodo llamado al morir, devuelve al fantasma a posIni
	void eliminaDir(int x, int y);//elimina la direccion contraria a la que lleva el fantasma
	bool hayFantasma(int dX, int dY);//indica si hay fantasma o no en la siguiente casilla donde va a avanzar el fantasma
	void siguienteDir(int newDirX, int newDirY);//establece la siguiente direccion del fantasma
	void posiblesDirecciones();//rellena un vector con todas las posibles direcciones que puede tomar el fantasma
	void toroide();//controla cuando los fantasmas se salen por los extremos del mapa
	void setPos(int posY, int posX);//establece la posicion del fantasmas
	int getPosX();//devuelven posicion y direccion del fantasma
	int getPosY();
	int getDirX();
	int getDirY();
	void modifyComible(bool esComible);//cambia el fantasma a comible o no comible
	bool getComible();//devuelve si el fantasma es comible
};

