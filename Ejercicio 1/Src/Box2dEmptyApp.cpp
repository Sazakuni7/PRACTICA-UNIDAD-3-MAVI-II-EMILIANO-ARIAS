#include "Game.h"

// Funci�n principal del programa
int main(int argc, char* argv[])
{
    // Creamos una instancia del juego
    Game* Juego;
    // Inicializamos el juego con una ventana de 800x600 p�xeles con el t�tulo "Avatar"
    Juego = new Game(800, 600, "Pelota bounce - MAVI II - Emiliano Arias");
    // Iniciamos el bucle principal del juego
    Juego->Loop();

    return 0;
}
