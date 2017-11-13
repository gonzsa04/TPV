#include "Game.h"
#include <iostream>
//inicializa la ventana del juego y todas las entidades
Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);//iniciamos SDL
	//damos a la ventana un nombre, con la posicion y el tamaño establecidos en Game
	window = SDL_CreateWindow("PACMAN", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	//le damos valor al renderer a partir de la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//avisamos si hay error
	if (window == nullptr || renderer == nullptr)cout << "Error initializing SDL\n";
	//si no hay error
	else
	{
		textures = new Texture;//creamos una nueva textura para cargar la imagen con todos los sprites
		textures->load(renderer, "..//images/pacman-spritesheet.png", 8, 4);

		gameMap = GameMAP(29, 28, this);//creamos el tablero
		gameMap.leeArchivo("level01.dat");//leemos un mapa de archivo y lo cargamos en gameMap
		pacman = PacMan(this, 20, 20, 14, 22, 0, 0, 6, 2, 0, 0);//creamos a pacman
	}
}

//devuelve el tipo de la casilla contigua en la direccion dada
MapCell Game::nextCell(int posX, int posY, int dirX, int dirY)
{
	return(gameMap.getCell((posY + dirY)/20, (posX + dirX)/20));
}

//bucle principal del juego
void Game::run()
{
	//mientras no se haya pulsado salir
	while (!exit && !win && !gameOver)
	{
		handleEvents();//miramos los eventos que ocurran en pantalla
		update();//mandamos a las entidades que actualicen su posicion
		render();//mandamos a las entidades que se pinten
		if (numComida == 0)win = true;//si nos comemos la comida ganamos
		SDL_Delay(100);
	}
}

//mira los eventos que ocurran en el juego
void Game::handleEvents()
{
	SDL_PollEvent(&event);//si se ha pulsado 
	//salir ponemos el bool a true para salir del bucle ppal.
	if (event.type == SDL_QUIT)exit = true;
	else if (event.type == SDL_KEYDOWN) 
	{//dependiendo de la tecla pulsada establecemos la siguiente direccion de pacman
		if (event.key.keysym.sym == SDLK_LEFT)pacman.siguienteDir(-20, 0);
		else if (event.key.keysym.sym == SDLK_RIGHT)pacman.siguienteDir(20, 0);
		else if (event.key.keysym.sym == SDLK_UP)pacman.siguienteDir(0, -20);
		else if (event.key.keysym.sym == SDLK_DOWN)pacman.siguienteDir(0, 20);
	}
}

//devuelve la textura
Texture* Game::getTexture()
{
	return textures;
}

//devuelve el renderer
SDL_Renderer* Game::getRenderer()
{
	return renderer;
}

//establece una casilla con un valor dado
void Game::setCell(int fils, int cols, MapCell tipoCasilla) 
{
	gameMap.setCell(fils, cols, tipoCasilla);
}

//suma o resta uno a la comida (1 o -1 como parametros)
void Game::setComida(int i) 
{
	numComida += i;
}

//manda a cada una de las entidades del juego que actualicen su posicion
void Game::update()
{
	pacman.update();
}

//manda a cada una de las entidades que se pinten
void Game::render()
{
	SDL_RenderClear(renderer);//borra
	gameMap.render();//le mandamos al tablero que se pinte
	pacman.render();//pinta entidades
	SDL_RenderPresent(renderer);//representa (pinta todo)
}

//finaliza el juego
Game::~Game()
{
	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}