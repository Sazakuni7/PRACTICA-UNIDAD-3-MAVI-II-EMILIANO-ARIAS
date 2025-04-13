#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include <vector>
#include "Avatar.h"

using namespace sf;
class Game
{
private:
	// Sistemas principales
	sf::RenderWindow* wnd;
	b2World* phyWorld;
	SFMLRenderer* renderer;

	// Configuración
	float fps;
	float frameTime;
	sf::Color clearColor;

	// Ragdoll
	b2Body* torso;
	std::vector<b2Body*> ragdollParts;

	// Para el arrastre con mouse
	b2MouseJoint* mouseJoint;
	b2Body* draggedBody;
	bool isDragging;
	b2Body* groundBodyMouse;
	b2Vec2 dragPoint;
	b2Vec2 dragOffset;

	void StartDrag(b2Vec2 mousePos);
	void UpdateDrag(b2Vec2 mousePos);
	void EndDrag();

public:

	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void Loop();
	void InitPhysics();
	void CreateRagdoll(const b2Vec2& position);
	void SetZoom();
	void DoEvents();
	void UpdatePhysics();
	void DrawGame();

};

