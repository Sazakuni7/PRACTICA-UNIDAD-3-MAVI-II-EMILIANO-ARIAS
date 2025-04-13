// Box2dEmptyApp.cpp : Defines the entry point for the console application.
//
#include "Game.h"

using namespace sf;



class Pelota{

private:
	//sprite y su imagen la para representación gráfica
	Sprite *_sprite;
	Texture  *_image;
	RenderWindow *wnd;
	//body para representación física
	b2Body* _body;
	//...
public:
	Pelota(b2World* _world, RenderWindow *_wnd){
		//guardamos puntero a ventana para dibujar luego
		wnd = _wnd;
		_image = new Texture();
		_image->loadFromFile("Pelota.png");
		//cargamos el sprite
		_sprite = new Sprite(*_image);

		//definimos el body y lo creamos
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(100.0f, 0.0f);
		_body = _world->CreateBody(&bodyDef);

		//creamos su figura de colisión
		//en este caso suponemos que la figura de
		//colision es una caja cuadrada		
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20.0f, 20.0f);

		//creamos el fixture, le seteamos 
		//la figura de colision
		//y agregamos el fixture al body
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 1.0f;
		_body->CreateFixture(&fixtureDef);

	}

	//metodo que posiciona el sprites
	//segun la posicion del body
	void ActualizarPosiciones(){
		b2Vec2 pos = _body->GetPosition();
		_sprite->setPosition(pos.x, pos.y);
	}

	//metodo que dibuja el sprite
	void Dibujar(){
		wnd->draw(*_sprite);
	}
	//otros metodos 
	//relacionados al personaje
};



int main(int argc, char* argv[])
{
	Game *Juego;
	Juego = new Game(800,600,"Cuerpo blando - MAVII");
	Juego->Loop();
	
	return 0;
}

