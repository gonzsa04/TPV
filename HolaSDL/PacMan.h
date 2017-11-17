//practicando con SDL
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
class Game;
#include <iostream>
using namespace std;

//clase PacMan que rige el comportamiento del jugador
class PacMan {
private:
	int posIniX, posIniY;//posicion inicial
	Texture* texture = nullptr;//puntero a su textura. La carga Game y luego se la pasa 
	SDL_Renderer* renderer = nullptr;//puntero a renderer, Game se lo pasa
	Game* game;//puntero a game para pedirle textura y renderer
	int dirX, dirY;//direccion actual
	int dirXSig, dirYSig;//siguiente direccion a tomar cuando sea posible
	int vidas;//numero de vidas
	int fil, col;//informacion de filas y columnas de la textura para ayudar a pintar
	int numTicks, numFrames;//velocidad de la animacion y numero de frames que tiene
	SDL_Rect destRect;//rectangulo destino donde se pintara pacman
public:
	PacMan();//constructora por defecto
			 //constructora que inicializa todos los campos de la entidad vistos anteriormente
	PacMan(Game* game, int width, int height, int f, int c, int numT, int numF);
	void render();//metodo que manda pintar a pacman
	void animate();//metodo que manda animar a pacman
	void update();//metodo que actualiza la posicion de pacman
	void morir();//metodo llamado al morir, resta una vida y devuelve a pacman a posIni
	void siguienteDir(int newDirX, int newDirY);//establece la siguiente direccion de pacman
	int getVidas();//devuelve el numero de vidas que le quedan a pacman
	void setPos(int posY, int posX);//establece la posicion de pacman
	int getPosX();
	int getPosY();
};
