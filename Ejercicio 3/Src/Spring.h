#pragma once
#include <Box2D/Box2D.h>

class Spring {
private:
    b2Body* bodyA;
    b2Body* bodyB;
    float restLength;
    float stiffness;
    float damping;

public:
    Spring(b2Body* a, b2Body* b, float length, float k, float d)
        : bodyA(a), bodyB(b), restLength(length), stiffness(k), damping(d) {}

    void Update() {
        b2Vec2 posA = bodyA->GetPosition();
        b2Vec2 posB = bodyB->GetPosition();
        b2Vec2 delta = posB - posA;

        float currentLength = delta.Length();
        if (currentLength > 0) {
            // Fuerza del resorte (Ley de Hooke)
            float displacement = currentLength - restLength;
            b2Vec2 springForce = (stiffness * displacement / currentLength) * delta;

            // Fuerza de amortiguación (relativa a la velocidad)
            b2Vec2 velA = bodyA->GetLinearVelocity();
            b2Vec2 velB = bodyB->GetLinearVelocity();
            b2Vec2 dampingForce = damping * (velB - velA);

            //Aplicar fuerzas
            bodyA->ApplyForce(springForce + dampingForce, posA, true);
            bodyB->ApplyForce(-springForce - dampingForce, posB, true);
        }
    }

    void Draw(sf::RenderWindow* window) {
        b2Vec2 posA = bodyA->GetPosition();
        b2Vec2 posB = bodyB->GetPosition();

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(posA.x, posA.y), sf::Color::Green),
            sf::Vertex(sf::Vector2f(posB.x, posB.y), sf::Color::Red)
        };
        window->draw(line, 2, sf::Lines);
    }
};