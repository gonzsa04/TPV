#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "GameMAP.h"
#include "PacMan.h"
#include <iostream>
using namespace std;

//clase Game que controla todo el juego
class Game 
{
private:
	SDL_Window*	window = nullptr;//puntero a ventana que creara al inicio del juego
	SDL_Renderer* renderer = nullptr;//lo mismo con un puntero a un renderer
	SDL_Event event;//aqui se guardan los eventos que ocurren en el juego
	int winWidth = 800;//tama�o de la pantalla
	int winHeight = 600;
	int winX = SDL_WINDOWPOS_CENTERED;//posicion de la pantalla
	int winY = SDL_WINDOWPOS_CENTERED;
	bool exit = false;//booleano que indica cuando se sale del juego (bucle ppal.)
	bool error = false;//informa de posibles errores
	bool gameOver = false; bool win = false;//ganar o perder
	int casillasComida;//numero de comida del mapa
	GameMAP gameMap;//tablero
	PacMan pacman;//jugador
	Texture* textures;//punteros a textura que contiene todos los sprites del juego
public:
	Game();//carga la ventana y el tablero del juego
	void run();//bucle ppal., dentro de el se ejecutan los siguientes metodos
	void render();//manda a cada una de las entidades que se pinten
	void update();//manda a cada una de las entidades que actualicen su posicion
	void handleEvents();//mira los eventos que ocurren en pantalla
	Texture* getTexture();//proporciona la textura
	SDL_Renderer* getRenderer();//proporciona el renderer
	bool nextCell(int posX, int posY, int dirX, int dirY);//determina si puedes moverte a la siguiente casilla en una direccion dada
	~Game();//cierra la ventana etc. (acaba el juego)
};

