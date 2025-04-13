#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana de renderizado
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 2.0f / fps;
    SetZoom(); // Configuración del zoom de la cámara
    InitPhysics(); // Inicialización del mundo físico
}

// Método principal que maneja el bucle del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpia la ventana con un color especificado
        DoEvents(); // Procesa los eventos del sistema
        UpdatePhysics(); // Actualiza la simulación física
        DrawGame(); // Dibuja el juego en la ventana
        wnd->display(); // Muestra la ventana renderizada
    }
}

// Actualiza la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Avanza la simulación física
    phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibuja el mundo físico (para depuración)
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    // Dibujamos el techo
    sf::RectangleShape ceilingShape(sf::Vector2f(0, 5));
    ceilingShape.setFillColor(sf::Color::Red);
    ceilingShape.setPosition(100, 95);
    wnd->draw(ceilingShape);
        
    // Dibujamos el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Dibujamos las paredes
    sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    leftWallShape.setFillColor(sf::Color::Green);
    leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
    wnd->draw(leftWallShape);

    sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    rightWallShape.setFillColor(sf::Color::Cyan);
    rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
    wnd->draw(rightWallShape);

    controlBodyAvatar->Actualizar(); // Actualiza la posición del avatar
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
        case Event::MouseButtonPressed:
            // Crea un cuerpo dinámico en forma de triángulo en la posición del clic del ratón
            b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 10.f, 0.4f, 0.0f);
            // Transforma las coordenadas según la vista activa
            Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
            body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);

            Avatar* nuevoAvatar = new Avatar(body, new sf::Sprite(texturaCaja));

            // Aplicar un impulso aleatorio
            b2Vec2 impulsoAleatorio(rand() % 100 - 50, rand() % 100 - 50);
            b2Vec2 origen(pos.x, pos.y);
            nuevoAvatar->AplicarImpulso(impulsoAleatorio, origen);
            break;
        }
    }
    /*
        // Mueve el cuerpo controlado por el teclado
        controlBody->SetAwake(true); // Activa el cuerpo para que responda a fuerzas y colisiones
        if (Keyboard::isKeyPressed(Keyboard::Left))
            controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
        if (Keyboard::isKeyPressed(Keyboard::Right))
            controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
        if (Keyboard::isKeyPressed(Keyboard::Down))
            controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
        if (Keyboard::isKeyPressed(Keyboard::Up))
            controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
    */
}
// Configura el área visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tamaño del área visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo físico y los elementos estáticos del juego
void Game::InitPhysics()
{
    // Inicializa el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 0.0f));

    // Inicializa el renderizador de depuración para el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
    phyWorld->SetDebugDraw(debugRender);

    // Crea los elementos estáticos del juego (suelo y paredes)
    b2Body* ceilingBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    ceilingBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
  
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    // Crea un cuerpo de círculo controlado por el teclado
    controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.f, 0.1f, 1.0f);
    controlBody->SetTransform(b2Vec2(50.0f, 80.0f), 0.0f);
    //controlBody->SetLinearVelocity(b2Vec2(80.f, 80.f));

    // Carga la textura de la pelota para el avatar
    texturaCaja.loadFromFile("Pelota.png");

    // Inicializa el avatar del jugador con el cuerpo físico creado y la textura de la pelota
    controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaCaja));

    // Aplica un impulso inicial
    b2Vec2 impulsoInicial(1980.f,1980.f);
    b2Vec2 origen(50.0f, 80.0f);
    controlBodyAvatar->AplicarImpulso(impulsoInicial, origen);

}

