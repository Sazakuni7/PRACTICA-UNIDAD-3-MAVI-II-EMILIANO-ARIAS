#include "Game.h"
#include "Box2DHelper.h"
#define velocity 5000.f


// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana de renderizado
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.5f / fps;
    SetZoom(); // Configuraci�n del zoom de la c�mara
    InitPhysics(); // Inicializaci�n del mundo f�sico
}

// M�todo principal que maneja el bucle del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpia la ventana con un color especificado
        DoEvents(); // Procesa los eventos del sistema
        UpdatePhysics(); // Actualiza la simulaci�n f�sica
        DrawGame(); // Dibuja el juego en la ventana
        wnd->display(); // Muestra la ventana renderizada
    }
}

// Actualiza la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Avanza la simulaci�n f�sica
    phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    phyWorld->DebugDraw();
    // Dibujamos el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Dibujamos las paredes
    sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    leftWallShape.setFillColor(sf::Color::Red);
    leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
    wnd->draw(leftWallShape);

    sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    rightWallShape.setFillColor(sf::Color::Cyan);
    rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
    wnd->draw(rightWallShape);

    controlBodyAvatar->Actualizar(); // Actualiza la posici�n del avatar
    controlBodyAvatar->Dibujar(*wnd); // Dibuja el avatar en la ventana
}

// Procesa los eventos del sistema
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cierra la ventana
            break;
        }
    }

    // Mueve el cuerpo controlado por el teclado
   // controlBody->SetAwake(true); // Activa el cuerpo para que responda a fuerzas y colisiones
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBodyAvatar->AplicarFuerza(b2Vec2(-velocity, 0.0f), b2Vec2(controlBody->GetWorldCenter()));
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBodyAvatar->AplicarFuerza(b2Vec2(velocity, 0.0f), b2Vec2(controlBody->GetWorldCenter()));
    if (Keyboard::isKeyPressed(Keyboard::Down))
        controlBodyAvatar->AplicarFuerza(b2Vec2(0.0f, velocity/2), b2Vec2(controlBody->GetWorldCenter()));
    if (Keyboard::isKeyPressed(Keyboard::Up))
        controlBodyAvatar->AplicarFuerza(b2Vec2(0.0f, -velocity/2), b2Vec2(controlBody->GetWorldCenter()));
}

// Configura el �rea visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tama�o del �rea visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo f�sico y los elementos est�ticos del juego
void Game::InitPhysics()
{
    // Inicializa el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Inicializa el renderizador de depuraci�n para el mundo f�sico
   debugRender = new SFMLRenderer(wnd);
   debugRender->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
   phyWorld->SetDebugDraw(debugRender);

    Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10)->SetTransform(b2Vec2(50, 100), 0); // Suelo
    Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100)->SetTransform(b2Vec2(0, 50), 0);   // Pared izq
    Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100)->SetTransform(b2Vec2(100, 50), 0); // Pared der

    // Crea un cuerpo de c�rculo controlado por el teclado
    controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 12, 12, 1.0f, 0.4f, 0.1f);
    controlBody->SetTransform(b2Vec2(50.0f, 80.0f), 0.0f);

    // Carga la textura de la caja para el avatar
    texturaCaja.loadFromFile("Caja.png");

    // Inicializa el avatar del jugador con el cuerpo f�sico creado y la textura de la caja
    controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaCaja));
}

