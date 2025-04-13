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
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	// Aplicar fuerza adicional para mejor arrastre
	if (mouseJoint && pickedBody)
	{
		b2Vec2 target = mouseJoint->GetTarget();
		b2Vec2 currentPos = pickedBody->GetPosition();
		b2Vec2 force;
		force.x = 50.0f * (target.x - currentPos.x);
		force.y = 50.0f * (target.y - currentPos.y);
		pickedBody->ApplyForceToCenter(force, true);
	}

	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
}

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
			if (mouseJoint) return;

			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			b2Vec2 worldPos(pos.x, pos.y);

			// Crear callback para detecci�n de clic
			MyQueryCallback callback(worldPos);
			b2AABB aabb;
			aabb.lowerBound = worldPos - b2Vec2(0.5f, 0.5f);
			aabb.upperBound = worldPos + b2Vec2(0.5f, 0.5f);
			phyWorld->QueryAABB(&callback, aabb);

			// Verificar si el clic fue en ball2
			if (callback.foundFixture && callback.foundFixture->GetBody() == ball2)
			{
				pickedBody = ball2;

				b2MouseJointDef mjd;
				mjd.bodyA = groundBodyMouse;
				mjd.bodyB = pickedBody;
				mjd.target = worldPos;
				mjd.maxForce = 3000.0f * pickedBody->GetMass();

				mouseJoint = (b2MouseJoint*)phyWorld->CreateJoint(&mjd);
				pickedBody->SetAwake(true);
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

void Game::CheckCollitions()
{
}

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

	// 1. Crear caja est�tica (pelota 1)
	ball1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 4.0f, 4.0f); // Caja de 4x4 unidades
	ball1->SetTransform(b2Vec2(40.0f, 50.0f), 0.0f); // Posicionamos la caja

	// Pelota 2
	ball2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3.5f, 0.3f, 0.1f, 1.0f);
	ball2->SetTransform(b2Vec2(60.0f, 50.0f), 0.0f);
	ball2->SetLinearDamping(0.1f);

	//Conectar ambas con joint el�stico
	b2DistanceJointDef jointDef;
	jointDef.Initialize(ball1, ball2, ball1->GetWorldCenter(), ball2->GetWorldCenter());
	jointDef.collideConnected = true;
	jointDef.length = 20.0f;
	jointDef.stiffness = 0.1f;  // Elasticidad
	phyWorld->CreateJoint(&jointDef);

	// Configuraci�n del cuerpo de referencia para mouse joint
	b2BodyDef groundBodyDef;
	groundBodyMouse = phyWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(0.1f, 0.1f);
	groundBodyMouse->CreateFixture(&groundShape, 0.0f);

}


Game::~Game(void)
{ }