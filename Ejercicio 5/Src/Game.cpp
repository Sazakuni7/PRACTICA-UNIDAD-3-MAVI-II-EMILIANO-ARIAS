#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo) :
    clearColor(sf::Color::Black)
{
    wnd = new sf::RenderWindow(sf::VideoMode(ancho, alto), titulo);
    wnd->setVerticalSyncEnabled(true);
    fps = 60.f;
    frameTime = 2.0f / fps;
    wnd->setFramerateLimit(fps);
    renderer = new SFMLRenderer(wnd);

    // Inicializar variables de arrastre
    draggedBody = nullptr;
    mouseJoint = nullptr;
    isDragging = false;

    InitPhysics();
    SetZoom();
}

// Método principal que maneja el bucle del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        DoEvents();
        UpdatePhysics();
        DrawGame();
    }
}

// Actualiza la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    wnd->clear(clearColor);

    phyWorld->DebugDraw();// Dibujado de debug (mostrará todas las formas y joints)

    wnd->display();
}

void Game::DoEvents()
{
    sf::Event evt;
    while (wnd->pollEvent(evt))
    {
        if (evt.type == sf::Event::Closed)
            wnd->close();

        // Conversión de coordenadas
        sf::Vector2f mousePos = wnd->mapPixelToCoords(sf::Vector2i(evt.mouseButton.x, evt.mouseButton.y));
        b2Vec2 physicsMousePos(mousePos.x, mousePos.y);

        // Presionar click izquierdo
        if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left)
        {
            StartDrag(physicsMousePos);
        }

        // Mover mouse mientras arrastramos
        if (isDragging && evt.type == sf::Event::MouseMoved)
        {
            sf::Vector2f newMousePos = wnd->mapPixelToCoords(sf::Vector2i(evt.mouseMove.x, evt.mouseMove.y));
            UpdateDrag(b2Vec2(newMousePos.x, newMousePos.y));
        }

        // Soltar click
        if (evt.type == sf::Event::MouseButtonReleased && evt.mouseButton.button == sf::Mouse::Left)
        {
            EndDrag();
        }
    }
}

void Game::StartDrag(b2Vec2 mousePos)
{
    //Resetear selección previa
    draggedBody = nullptr;

    // Consultar cuerpo bajo el mouse
    b2AABB aabb;
    b2Vec2 d(0.001f, 0.001f);
    aabb.lowerBound = mousePos - d;
    aabb.upperBound = mousePos + d;

    class QueryCallback : public b2QueryCallback {
    public:
        b2Vec2 point;
        b2Body* foundBody = nullptr;

        bool ReportFixture(b2Fixture* fixture) override {
            if (fixture->TestPoint(point)) {
                foundBody = fixture->GetBody();
                return false;
            }
            return true;
        }
    } callback;

    callback.point = mousePos;
    phyWorld->QueryAABB(&callback, aabb);

    if (callback.foundBody && callback.foundBody != groundBodyMouse)
    {

        //Metodo con MouseJoint
        draggedBody = callback.foundBody;
        dragPoint = draggedBody->GetLocalPoint(mousePos);

        b2MouseJointDef md;
        md.bodyA = groundBodyMouse;
        md.bodyB = draggedBody;
        md.target = mousePos;
        md.maxForce = 5000.0f * draggedBody->GetMass();
        md.collideConnected = true;

        md.stiffness = 150.0f;  //Rigidez
        md.damping = 0.9f;  //amortiguación


        mouseJoint = (b2MouseJoint*)phyWorld->CreateJoint(&md);
        isDragging = true;
    }
}

void Game::UpdateDrag(b2Vec2 mousePos)
{
    if (isDragging && mouseJoint)
    {
        if (mouseJoint->GetType() == e_mouseJoint)
        {
            ((b2MouseJoint*)mouseJoint)->SetTarget(mousePos);
        }
    }
}
void Game::EndDrag()
{
    if (mouseJoint)
    {
        phyWorld->DestroyJoint(mouseJoint);
        mouseJoint = nullptr;
    }
    draggedBody = nullptr;
    isDragging = false;
}

// Configura el área visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tamaño del área visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

void Game::CreateRagdoll(const b2Vec2& position)
{
    const float scale = 5.5f;
    const float density = 0.5f;
    const float friction = 0.3f;
    const float restitution = 0.1f;

    // Torso
    torso = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5f * scale, 3.0f * scale, density, friction, restitution);
    torso->SetTransform(position, 0.0f);
    ragdollParts.push_back(torso);

    // Cabeza
    b2Body* head = Box2DHelper::CreateCircularDynamicBody(phyWorld, 0.8f * scale, density, friction, restitution);
    head->SetTransform(position + b2Vec2(0.0f, -2.2f * scale), 0.0f);
    ragdollParts.push_back(head);

    // Brazos
    b2Body* upperArmL = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5f * scale, 0.6f * scale, density, friction, restitution);
    upperArmL->SetTransform(position + b2Vec2(-1.8f * scale, -1.0f * scale), -0.3f);
    ragdollParts.push_back(upperArmL);

    b2Body* upperArmR = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5f * scale, 0.6f * scale, density, friction, restitution);
    upperArmR->SetTransform(position + b2Vec2(1.8f * scale, -1.0f * scale), 0.3f);
    ragdollParts.push_back(upperArmR);

    // Piernas
    b2Body* upperLegL = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.8f * scale, 2.0f * scale, density, friction, restitution);
    upperLegL->SetTransform(position + b2Vec2(-0.5f * scale, 2.5f * scale), -0.1f);
    ragdollParts.push_back(upperLegL);

    b2Body* upperLegR = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.8f * scale, 2.0f * scale, density, friction, restitution);
    upperLegR->SetTransform(position + b2Vec2(0.5f * scale, 2.5f * scale), 0.1f);
    ragdollParts.push_back(upperLegR);

    // Creación de joints
    b2RevoluteJointDef jointDef;
    jointDef.lowerAngle = -0.5f * b2_pi; //Límites de rotación
    jointDef.upperAngle = 0.5f * b2_pi;
    jointDef.enableLimit = true;

    // Cabeza
    jointDef.Initialize(torso, head, torso->GetWorldPoint(b2Vec2(0, -1.5f * scale)));
    phyWorld->CreateJoint(&jointDef);

    // Brazos
    jointDef.Initialize(torso, upperArmL, torso->GetWorldPoint(b2Vec2(-0.9f * scale, -0.8f * scale)));
    phyWorld->CreateJoint(&jointDef);

    jointDef.Initialize(torso, upperArmR, torso->GetWorldPoint(b2Vec2(0.9f * scale, -0.8f * scale)));
    phyWorld->CreateJoint(&jointDef);

    // Piernas
    jointDef.Initialize(torso, upperLegL, torso->GetWorldPoint(b2Vec2(-0.3f * scale, 1.5f * scale)));
    phyWorld->CreateJoint(&jointDef);

    jointDef.Initialize(torso, upperLegR, torso->GetWorldPoint(b2Vec2(0.3f * scale, 1.5f * scale)));
    phyWorld->CreateJoint(&jointDef);

    for (auto part : ragdollParts) {
        part->SetLinearDamping(0.3f);
        part->SetAngularDamping(0.8f);
    }
}

// Inicializa el mundo físico y los elementos estáticos del juego
void Game::InitPhysics()
{
    phyWorld = new b2World(b2Vec2(0, 9.8f));
    phyWorld->SetDebugDraw(renderer);
    renderer->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);

    // Paredes y suelo
    Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10)->SetTransform(b2Vec2(50, 100), 0); // Suelo
    Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100)->SetTransform(b2Vec2(0, 50), 0);   // Pared izq
    Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100)->SetTransform(b2Vec2(100, 50), 0); // Pared der

    CreateRagdoll(b2Vec2(50, 30)); // Posición inicial

    // Crear cuerpo fantasma para el mouse joint
    b2BodyDef groundBodyDef;
    groundBodyMouse = phyWorld->CreateBody(&groundBodyDef);

    // Inicializar estado de arrastre
    mouseJoint = nullptr;
    isDragging = false;

    dragOffset.SetZero();
}

