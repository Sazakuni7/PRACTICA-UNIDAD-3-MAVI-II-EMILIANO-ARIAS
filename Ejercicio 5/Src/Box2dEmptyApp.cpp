#include "Game.h"

int main(int argc, char* argv[])
{
    Game* Juego;
    Juego = new Game(800, 600, "Ragdoll test - MAVI II - Emiliano Arias");
    Juego->Loop();

    return 0;
}
