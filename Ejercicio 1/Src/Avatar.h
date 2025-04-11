#pragma once

#include <Box2D/Box2D.h>

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

// Representa un objeto en la escena
// Basicamente es una union entre la simulacion de Box2D y una representacion usando SFML
// Este objeto sera el responsable de:
//	-Mantener sincronizada la simulacion de 1 cuerpo rigido y SFML
class Avatar
{
private:
	//Sprite de SFML
	sf::Sprite*  m_pSprite = nullptr;

	//El cuerpo rigido del avatar
	b2Body*    m_pCuerpo = nullptr;

	//El mundo de la simulacion de Box2D (se usa para limpiar el avatar de la escena)
	b2World*   m_pWorld = nullptr;

	//Variables auxiliares, para no alocar memoria en cada frame
	b2Vec2     m_posAux = b2Vec2(0.0f, 0.0f);

	float      m_angAux = 0.0f;

public:

	// Parametros:
	//	-Un puntero al cuerpo rigido a mantener
	//	-Un puntero al sprite de SFML asociado con este cuerpo rigido
	// Nota: Se mantienen rerferencias a pCuerpo y pSprite, no liberarlos!
	Avatar(b2Body* pCuerpo, sf::Sprite* pSprite);

	~Avatar();

	 operator b2Body*();

	// Actualiza la posicion del avatar
	void Actualizar();

	// Dibuja el avatar
	// Parametros:
	//	-Una referencia al RenderWindow donde dibujar el avatar
	void Dibujar(sf::RenderWindow &RW);

	// Devuelve la posicion del cuerpo
	b2Vec2 GetPosition();

	// Devuelve el cuerpo de Box2D
	b2Body& GetCuerpo();

	// Devuelve el sprite de SFML
	sf::Sprite& GetSprite();

	// Establece la posicion del cuerpo
	void SetPosition(b2Vec2 position);

	// Aplica una fuerza sobre el cuerpo
	// Parametros:
	//	-El vector de fuerza a aplicar
	//	-El origen de aplicacion de la fuerza en coordenadas del mundo
	void AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen = b2Vec2(0.0f,0.0f));

	// Aplica un impulso sobre el cuerpo
	// Parametros:
	//	-El vector de impulso a aplicar
	//	-El origen de aplicacion el impulso en coordenadas del mundo
	void AplicarImpulso(b2Vec2& impulso, b2Vec2& origen = b2Vec2(0.0f, 0.0f));
};