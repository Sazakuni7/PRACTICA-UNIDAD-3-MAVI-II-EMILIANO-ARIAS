#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto,std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 2.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();	
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	ballSpring->Update();
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
	ballSpring->Draw(wnd);

class MyQueryCallback : public b2QueryCallback {
public:
	b2Vec2 point;
	b2Fixture* foundFixture = nullptr;

	MyQueryCallback(const b2Vec2& p) : point(p) {}

	bool ReportFixture(b2Fixture* fixture) override {
		if (fixture->TestPoint(point)) {
			foundFixture = fixture;
			return false;
		}
		return true;
	}
};

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		case Event::MouseButtonPressed:
		{
			if (mouseJoint) {
				Vector2f pos = wnd->mapPixelToCoords(Mouse::getPosition(*wnd));
				mouseJoint->SetTarget(b2Vec2(pos.x, pos.y));
			}
			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			b2Vec2 worldPos(pos.x, pos.y);

			// Query para detectar si clickeaste una pelota
			b2AABB aabb;
			aabb.lowerBound = worldPos - b2Vec2(1.5f, 1.5f);
			aabb.upperBound = worldPos + b2Vec2(1.5f, 1.5f);

			MyQueryCallback callback(worldPos);
			phyWorld->QueryAABB(&callback, aabb);

			if (callback.foundFixture)
			{
				b2Body* body = callback.foundFixture->GetBody();

				if (body == ball1 || body == ball2) {
					pickedBody = body;
					b2MouseJointDef mjd;
					mjd.bodyA = groundBodyMouse;
					mjd.bodyB = pickedBody;
					mjd.target = worldPos;
					mjd.maxForce = 10000.0f * pickedBody->GetMass();	
					//Para MouseJoint, tanto stiffness como damping si funcionan y muestra un impacto real, en este caso se simula un resorte desde la zona de la pelota
					//clickeada hacia el punto de arrastre (mi cursor)
					mjd.stiffness = 12.0f;  //Rigidez
					mjd.damping = 0.7f; //Amortiguación
					mouseJoint = (b2MouseJoint*)phyWorld->CreateJoint(&mjd);
					pickedBody->SetAwake(true);
				}
			}
			break;
		}
		case Event::MouseButtonReleased:
			if (mouseJoint)
			{
				phyWorld->DestroyJoint(mouseJoint);
				mouseJoint = nullptr;
				pickedBody = nullptr;
			}
			break;
		case Event::MouseMoved:
			if (mouseJoint)
			{
				Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y));
				mouseJoint->SetTarget(b2Vec2(pos.x, pos.y));
			}
			break;
		}
	}
}


// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); // Asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	//creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Cuerpo de referencia para mouseJoint
	b2BodyDef groundBodyDef;
	groundBodyMouse = phyWorld->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(0.1f, 0.1f);
	groundBodyMouse->CreateFixture(&groundBox, 0.0f);

	// Pelota 1
	ball1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3.5f, 0.3f, 0.1f, 1.0f);
	ball1->SetTransform(b2Vec2(40.0f, 50.0f), 0.0f);
	ball1->SetLinearDamping(0.3f);  // Amortiguación lineal

	// Pelota 2
	ball2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3.5f, 0.3f, 0.1f, 1.0f);
	ball2->SetTransform(b2Vec2(60.0f, 50.0f), 0.0f);
	ball2->SetLinearDamping(0.3f);

	ballSpring = new Spring(ball1, ball2, 18.0f, 50.0f, 2.0f);

	//DISTANCEJOINT COMENTADO: Intenté utilizar este tipo de Joint para mantener unidas las 2 pelotas pero simular un resorte entre ellas
	//Ya que FrequencyHz y DampingRatio no funcionaban (según leí es por la version de Box2D) intenté usar stiffness y damping que son sus derivados
	//pero no hubo resultado alguno, no se simula un resorte

	/*// Resorte (DistanceJoint)
	b2DistanceJointDef jointDef;
	jointDef.Initialize(ball1, ball2, ball1->GetWorldCenter(), ball2->GetWorldCenter());
	jointDef.collideConnected = false;
	jointDef.length = 20.0f;

	jointDef.stiffness = 15.0f;  //rigidez
	jointDef.damping = 2.0f;    //amortiguación

	phyWorld->CreateJoint(&jointDef);*/
}


Game::~Game(void)
{
delete ballSpring;
}
