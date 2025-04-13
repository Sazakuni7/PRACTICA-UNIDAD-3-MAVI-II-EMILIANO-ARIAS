#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include "Spring.h"

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;
	Spring* ballSpring;
	// Objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	// Tiempo de frame
	float frameTime;
	int fps;

	// Cuerpo de box2d
	b2Body* controlBody;
	b2Body* ball1;
	b2Body* ball2;
	b2MouseJoint* mouseJoint = nullptr;
	b2Body* pickedBody = nullptr;
	b2Body* groundBodyMouse = nullptr;

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};