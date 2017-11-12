#include "Game.h"
#include <iostream>
//inicializa la ventana del juego y todas las entidades
Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);//iniciamos SDL
	//damos a la ventana un nombre, con la posicion y el tamaño establecidos en Game
	window = SDL_CreateWindow("My first test wih SDL", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	//le damos valor al renderer a partir de la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//avisamos si hay error
	if (window == nullptr || renderer == nullptr)cout << "Error initializing SDL\n";
	//si no hay error
	else
	{
		textures = new Texture;//creamos una nueva textura para cada entidad
		textures->load(renderer, ".\characters1.png", 3, 13);//cargamos cada textura de la lista

		gameMap = GameMAP(5, 5, this);
		gameMap.render("level01.dat");
	}
}

//bucle principal del juego
void Game::run()
{
	//mientras no se haya pulsado salir
	while (!exit)
	{
		handleEvents();//miramos los eventos que ocurran en pantalla
		update();//mandamos a las entidades que actualicen su posicion
		render();//mandamos a las entidades que se pinten
		SDL_Delay(5);
	}
}

//mira los eventos que ocurran en el juego
void Game::handleEvents()
{
	SDL_PollEvent(&event);//si se ha pulsado 
	//salir ponemos el bool a true para salir del bucle ppal.
	if (event.type == SDL_QUIT)exit = true;
}

Texture* Game::getTexture()
{
	return textures;
}

SDL_Renderer* Game::getRenderer()
{
	return renderer;
}

//manda a cada una de las entidades del juego que actualicen su posicion
/*void Game::update()
{
dog.update();
helicopter.update();
}*/

//manda a cada una de las entidades que se pinten
void Game::render()
{
SDL_RenderClear(renderer);//borra
render();//pinta entidades
SDL_RenderPresent(renderer);//representa (pinta todo)
}

//finaliza el juego
Game::~Game()
{
	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}