#include <fstream>
#include <string>
#include "Game.h"
#include <time.h>
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
		srand(time(nullptr));
		for (int i = 0; i < 7; i++) { textures[i] = new Texture; }//creamos una nueva textura para cargar la imagen con todos los sprites
		textures[0]->load(renderer, "..//images/pacman-spritesheet.png", 8, 4);
		textures[1]->load(renderer, "..//images/PacmanAnimation.png", 1, 4);
		textures[2]->load(renderer, "..//images/FAsustadosAnimation.png", 1, 2);
		textures[3]->load(renderer, "..//images/YouWin.png", 1, 1);
		textures[4]->load(renderer, "..//images/Game-Over.png", 1, 1);
		textures[5]->load(renderer, "..//images/Menu1.png", 1, 1);
		textures[6]->load(renderer, "..//images/Menu2.png", 1, 1);
		leeArchivo("level0" + std::to_string(nivel) + ".dat");
	}
}

//devuelve el tipo de la casilla contigua en la direccion dada
MapCell Game::nextCell(int posX, int posY, int dirX, int dirY)
{
	return(gameMap->getCell((posY + dirY) / TAM, (posX + dirX) / TAM));
}

//bucle principal del juego
void Game::run()
{
	int startTime, frameTime, menu = 1;
	
	//mientras no se haya pulsado salir
	while (!exit)
	{
		if (event.key.keysym.sym == SDLK_UP) menu = 1;
		else if (event.key.keysym.sym == SDLK_DOWN) menu = 2;
		if (menu == 1) 
		{ 
			textures[5]->render(renderer); 
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				menu = 3;
			}
		}
		else
		{
			textures[6]->render(renderer);
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				exit = true;
			}
		}
		SDL_RenderPresent(renderer);//representa (pinta todo)
		SDL_PollEvent(&event);//si se ha pulsado 
		while (!exit && !win && !gameOver && menu == 3)
		{
			startTime = SDL_GetTicks();

			handleEvents();//miramos los eventos que ocurran en pantalla
			update();//mandamos a las entidades que actualicen su posicion
			render();//mandamos a las entidades que se pinten
			colisiones();//controlamos colisiones entre pacman y fantasmas

			if (numComida == 0)win = true;//si nos comemos la comida ganamos

			//temporiza el tiempo que puedes comerte a los fantasmas
			if (temporizador)Temp++;
			if (Temp >= 50) { Temp = 0; fantasmasComibles(false); }

			//framerate
			frameTime = SDL_GetTicks() - startTime;
			if (frameTime < 120)
				SDL_Delay(180 - frameTime);

			if (win && nivel < 5)
			{
				nivel++;
				leeArchivo("level0" + std::to_string(nivel) + ".dat");
				win = false;
			}
			if (event.type == SDL_QUIT)exit = true;
		}
		
		SDL_RenderClear(renderer);
		if (menu == 3 && gameOver)
		{ 
			menu = 1;
			textures[4]->render(renderer);
			SDL_RenderPresent(renderer);//representa (pinta todo)
			SDL_Delay(2000);
		}
		else if (menu == 3 && win)
		{
			menu = 1;
			textures[3]->render(renderer); 
			SDL_RenderPresent(renderer);//representa (pinta todo)
			SDL_Delay(2000);
		}
		//salir ponemos el bool a true para salir del bucle ppal.
		if (event.type == SDL_QUIT)exit = true;
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
		if (event.key.keysym.sym == SDLK_LEFT) pacman.siguienteDir(-TAM, 0);
		else if (event.key.keysym.sym == SDLK_RIGHT) pacman.siguienteDir(TAM, 0);
		else if (event.key.keysym.sym == SDLK_UP) pacman.siguienteDir(0, -TAM);
		else if (event.key.keysym.sym == SDLK_DOWN) pacman.siguienteDir(0, TAM);
		else if (event.key.keysym.sym == SDLK_g) guardarPartida();
		else if (event.key.keysym.sym == SDLK_c) cargarPartida();
	}
}

//manda a cada una de las entidades del juego que actualicen su posicion
void Game::update()
{
	pacman.update();
	for (int i = 0; i < 4; i++)fantasmas[i].update();
}

void Game::colisiones()
{	
	int i = 0;
	//colisiona con el si sus posiciones son iguales o si su siguiente posicion es igual que la del fantasma y la siguiente posicion del fantasma es la misma que la suya
	while (i < 4 && ((pacman.getPosX() != getFantasmas(i).getPosX() || pacman.getPosY() != getFantasmas(i).getPosY()) &&
		((pacman.getPosX() + pacman.getDirX() != getFantasmas(i).getPosX() || pacman.getPosY() + pacman.getDirY() != getFantasmas(i).getPosY()) ||
		(pacman.getPosX() != getFantasmas(i).getPosX() + getFantasmas(i).getDirX() || pacman.getPosY() != getFantasmas(i).getPosY() + getFantasmas(i).getDirY()))))
		i++;
	//si hay colision 
	if (i < 4)
	{
		//si no son comibles se pierde una vida y tanto pacman como los fantasmas vuelven a su posicion inicial
		if (!getFantasmas(i).getComible())
		{
			pacman.morir();
			for (int i = 0; i < 4; i++)muereFantasma(i);
		}
		//si son comibles te comes a ese fantasma
		else muereFantasma(i);
	}
}

//manda a cada una de las entidades que se pinten
void Game::render()
{
	SDL_RenderClear(renderer);//borra
	gameMap->render(TAM);//le mandamos al tablero que se pinte a un tamaño
	pacman.animate();//pinta entidades
	for (int i = 0; i < 4; i++) fantasmas[i].render();//pintamos los fantasmas
	renderHud();
	SDL_RenderPresent(renderer);//representa (pinta todo)
}

void Game::renderHud()
{
	SDL_Rect destRect;
	destRect.w = destRect.h = TAM;
	destRect.x = cols*TAM;
	destRect.y = 1;
	for (int i = 0; i < pacman.getVidas(); i++) {
		destRect.x += TAM;
		textures[0]->renderFrame(renderer, destRect, 6, 2);
	}
}

//suma o resta uno a la comida (1 o -1 como parametros)
void Game::setComida(int i)
{
	numComida += i;
}

//devuelve la textura
Texture* Game::getTexture(int i) { return textures[i]; }

//devuelve el renderer
SDL_Renderer* Game::getRenderer() { return renderer; }

//establece una casilla con un valor dado
void Game::setCell(int fils, int cols, MapCell tipoCasilla)
{
	gameMap->setCell(fils, cols, tipoCasilla);
}

//lee de archivo un mapa y modifica el array de casillas para que sea igual
//llamado desde render
void Game::leeArchivo(string filename)
{
	ifstream archivo;
	char character;
	archivo.open("./Levels/" + filename);
	if (archivo.is_open()) 
	{
		archivo >> fils >> cols;

		pacman = PacMan(this, TAM, TAM, 6, 2, 12, 4);//creamos a pacman
		gameMap = new GameMAP(fils, cols, this);//creamos el tablero
		fantasmas[0] = Fantasma(this, TAM, TAM, 0, 0, 1, 2);//creamos los fantasmas
		fantasmas[1] = Fantasma(this, TAM, TAM, 1, 0, 1, 2);
		fantasmas[2] = Fantasma(this, TAM, TAM, 2, 0, 1, 2);
		fantasmas[3] = Fantasma(this, TAM, TAM, 3, 0, 1, 2);

		for (int i = 0; i < fils; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				archivo >> character;
				character -= 48;
				if ((int)character < 4)
				{
					gameMap->setCell(j, i, (MapCell)(int)character);
					if (gameMap->getCell(i, j) == comida || gameMap->getCell(i, j) == vitamina) setComida(1);
				}
				else
				{
					if ((int)character == 5) { fantasmas[0].setPos(i*TAM, j*TAM); }
					else if ((int)character == 6) { fantasmas[1].setPos(i*TAM, j*TAM); }
					else if ((int)character == 7) { fantasmas[2].setPos(i*TAM, j*TAM); }
					else if ((int)character == 8) { fantasmas[3].setPos(i*TAM, j*TAM); }
					else if ((int)character == 9) { pacman.setPos(i*TAM, j*TAM); }
					gameMap->setCell(j, i, (MapCell)0);
				}
			}
		}
		archivo.close();
	}
}

int Game::getTabFils() { return fils; }

int Game::getTabCols() { return cols; }

Fantasma Game::getFantasmas(int i) { return fantasmas[i]; }//devuelve el fantasma i

void Game::muereFantasma(int i) //mata al fantasma i
{
	fantasmas[i].morir();
}

//establece todos los fantasmas a comibles o no comibles
void Game::fantasmasComibles(bool sonComibles)
{
	for (int i = 0; i < 4; i++)fantasmas[i].modifyComible(sonComibles);
	if (sonComibles) { temporizador = true; Temp = 0; }//si son comibles inicia el temporizador
	else temporizador = false;//si dejan de serlo finaliza el temporizador
}

PacMan Game::getPacman() { return pacman; }

int Game::getTam() { return TAM; }

void Game::GameOver()
{
	gameOver = true;
}

void Game::guardarPartida()
{
	ofstream archivo;
	archivo.open("./Levels/NivelGuardado");
	archivo << fils << " " << cols << endl;

	for (int i = 0; i < fils; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			if (pacman.getPosX() == j*TAM && pacman.getPosY() == i*TAM)archivo << 9;
			else if (fantasmas[0].getPosX() == j*TAM && fantasmas[0].getPosY() == i*TAM)archivo << 5;
			else if (fantasmas[1].getPosX() == j*TAM && fantasmas[0].getPosY() == i*TAM)archivo << 6;
			else if (fantasmas[2].getPosX() == j*TAM && fantasmas[0].getPosY() == i*TAM)archivo << 7;
			else if (fantasmas[3].getPosX() == j*TAM && fantasmas[0].getPosY() == i*TAM)archivo << 8;
			else archivo << (int)gameMap->getCell(i, j);
			if (j < cols - 1)archivo << " ";
		}
		archivo << endl;
	}
	archivo.close();
}

void Game::cargarPartida() 
{
	leeArchivo("NivelGuardado");
}

//finaliza el juego
Game::~Game()
{
	delete(gameMap);
	if (textures != nullptr) {
		for (int i = 0; i < 7; i++)delete[] textures[i];
	}

	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}