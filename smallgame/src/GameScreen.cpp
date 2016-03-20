#include "GameScreen.hpp"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace OficinaFramework;
using namespace SmallGame;


void GameScreen::Initialize()
{
    SetActive(true);
    SetVisible(true);
    m_drawables = new EntitySystem::DrawableEntityCollection;
    m_drawables->Init();
    srand(time(NULL));
    ScreenSystem::Screen::Initialize();
}

void GameScreen::LoadContent()
{
    for(int i = 0; i < 5; i++)
        m_drawables->Add("RotatingBlock", new GameBlock(vec2(100.0f + (150.0f * i), 300.0f)));

    m_drawables->LoadContent();
    ScreenSystem::Screen::LoadContent();
}

void GameScreen::UnloadContent()
{
    m_drawables->UnloadContent();
    ScreenSystem::Screen::UnloadContent();
}

void GameScreen::Update()
{
    m_drawables->Update();
}

void GameScreen::Draw()
{
    glPushMatrix();
        m_drawables->Draw();
    glPopMatrix();
}


GameBlock::GameBlock(vec2 Position)
{
    this->position = Position;
}

GameBlock::~GameBlock() {}

void GameBlock::Init()
{
    angle = 0.0f;
}

void GameBlock::LoadContent()
{
    m_blocktexture = RenderingSystem::TexturePool::LoadTexture("textures/ground");
    size = m_blocktexture->GetSize().toVec2() / 4.0f;
}

void GameBlock::UnloadContent()
{
    RenderingSystem::TexturePool::DisposeTexture(m_blocktexture);
}

void GameBlock::Update()
{
    angle = angle >= 360.0f ? 0.0f : angle + (rand() % 5) + 1.0f;
    std::stringstream ss;
    ss.clear();
    ss << "Obj RotatingBlock pos: " << position << " ang: " << angle;

    ScreenSystem::Debug_AddLine(ss.str());
}

void GameBlock::Draw()
{
    glPushMatrix();
        glTranslatef(position.x, position.y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
            vec2 halfSize = size / 2.0f;
            glTranslatef(-halfSize.x, -halfSize.y, 0.0f);
            m_blocktexture->Draw(vec2::Zero(), size, Color4::MaskToColor4(ColorDef::WHITE));
        glPopMatrix();
    glPopMatrix();
}
