#include "Game.h"

int main(int argc, char* argv[])
{
	Game *Juego;
	Juego = new Game(800,600,"Resorte - MAVII - Emiliano Arias");
	Juego->Loop();
	
	return 0;
}

