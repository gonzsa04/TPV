#pragma once
class Game;
#include "Texture.h"
#include <iostream>
using namespace std;

//enum con los tipos de casilla que puede tener el tablero
enum MapCell { vacio, muro, comida, vitamina };

//clase que representa el mapa o tablero del juego
class GameMAP {
private:
	int fils;//filas del tablero
	int cols;//columnas del tablero
	Game* game;//puntero a game para pedirle renderer y textura
	MapCell** cells;//array bidimensional de casillas
	Texture* textures;//puntero a textura que obtendra de game
	SDL_Renderer* renderer;//puntero a renderer que obtendra de game
public:
	GameMAP();//constructora por defecto que iguala todo a 0 (para cuando se llame al crear game)
	GameMAP(int fils, int cols, Game* game);//constructora "de verdad" que inicializa los parametros del tablero
	MapCell getCell(int fils, int cols);//devuelve una casilla en la posicion dada
	void setCell(int fils, int cols, MapCell tipoCasilla);//cambia la casilla dada por el valor correspondiente
	void leeArchivo(string filename);//lee de un archivo un mapa y actualiza las casillas del tablero
	void render(int tamanyo = 20);//pinta el tablero a un tamaño determinado
	//~GameMAP();//destructora
};