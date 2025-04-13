#include "Game.h"

int main(int argc, char* argv[])
{
	Game *Juego;
	Juego = new Game(800,600,"Bola atada a caja - MAVII - Emiliano Arias");
	Juego->Loop();
	
	return 0;
}

