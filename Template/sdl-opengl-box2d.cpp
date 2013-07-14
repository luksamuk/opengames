// Bibliotecas
#include <cstdlib>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Box2D/Box2D.h>

// Definições de display
#define WIDTH        640
#define HEIGHT       480
#define REFRESH_RATE 60.0

// Definições de input
#define KeyboardKey SDLKey
#define MouseButton int

const float MeterToPixel = 20.125; // Aprox. 1,70M
const float PixelToMeter = 1/MeterToPixel;

bool running;
b2World* world;
void init();
void handleEvents();
void handleKeyboard(KeyboardKey);
void handleMouse(int, int, MouseButton);
void draw();
void drawShape(int, b2Vec2[], b2Vec2, float);

void insertShape(int, int, int, int, float, float, bool);


int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("SDL/OpenGL Box2D Test", NULL);
    SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
    Uint32 start;
    running = true;

    init();

    while(running)
    {
        start = SDL_GetTicks();
        handleEvents();
        draw();

        // Update de 60hz
            // World
        world->Step(1.0/REFRESH_RATE, 8, 3);
            // Display SDL/OpenGL
        SDL_GL_SwapBuffers();
        if(1000.0/REFRESH_RATE > SDL_GetTicks() - start)
            SDL_Delay(1000.0/REFRESH_RATE - (SDL_GetTicks() - start));
    }
    SDL_Quit();
    return 0;
}

void init()
{
    // Muda o canto superior da tela para cima, esquerda
    glMatrixMode(GL_PROJECTION);
        glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 1);

    // Cria um novo World para a Box2D, com a gravidade
    // da terra (g = 9,80665 m/s^2)
    world = new b2World(b2Vec2(0.0, 9.80665));

    // Adiciona uma caixa que funcionará como chão
    insertShape(WIDTH / 2, HEIGHT - 50, WIDTH, 30, 1.0f, 0.0f, false);
}

void handleEvents()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            handleKeyboard(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouse(event.button.x, event.button.y, event.button.button);
            break;
        }
    }
}

void handleKeyboard(KeyboardKey key)
{
    switch(key)
    {
    case SDLK_ESCAPE:
        running = false;
        break;
    case SDLK_q:
        while(world->GetBodyList())
            world->DestroyBody(world->GetBodyList());
            insertShape(WIDTH / 2, HEIGHT - 50, WIDTH, 30, 1.0f, 0.0f, false);
        break;
    }
}

void handleMouse(int X, int Y, MouseButton button)
{
    switch(button)
    {
    case SDL_BUTTON_LEFT:
        break;
    case SDL_BUTTON_RIGHT:
        break;
    case SDL_BUTTON_MIDDLE:
	break;
    }
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    b2Body* aux = world->GetBodyList();
    b2PolygonShape* bodyshape;
    int n_points;

    while(aux)
    {
        bodyshape = (b2PolygonShape*)aux->GetFixtureList()->GetShape();
        n_points = bodyshape->GetVertexCount();
        b2Vec2 points[n_points];

        for(int i = 0; i < n_points; i++)
            points[i] = bodyshape->GetVertex(i);

        drawShape(n_points, points, aux->GetWorldCenter(), aux->GetAngle());

        aux = aux->GetNext();
    }
}

void drawShape(int vertexCount, b2Vec2 vertices[], b2Vec2 CenterOfMass, float angle)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glTranslatef(CenterOfMass.x * MeterToPixel, CenterOfMass.y * MeterToPixel, 0);
        glRotatef(angle * 180.0/M_PI, 0, 0, 1);

        glBegin(GL_QUADS);
            for(int i = 0; i < vertexCount; i++)
                glVertex2f(vertices[i].x * MeterToPixel,
                           vertices[i].y * MeterToPixel);
        glEnd();
    glPopMatrix();
}

void insertShape(int X, int Y, int width, int height, float density, float restitution, bool isDynamic = true)
{
    b2Body* body;
    b2PolygonShape shape;
    b2BodyDef bodydef;
    b2FixtureDef fixturedef;

    // Cria as definições da forma polígono
    shape.SetAsBox(width / 2 * PixelToMeter, height / 2 * PixelToMeter);

    // Cria as definições do corpo
    bodydef.position.Set(X * PixelToMeter, Y * PixelToMeter);
    if(isDynamic) bodydef.type = b2_dynamicBody;

    // Cria as definições da fixture
    fixturedef.shape = &shape;
    fixturedef.density = density;
    fixturedef.restitution = restitution;

    // Cria o corpo e o adiciona ao mundo
    body = world->CreateBody(&bodydef);
    // Adiciona a fixture ao corpo
    body->CreateFixture(&fixturedef);
}
