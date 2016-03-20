#include "GameScreen.hpp"
using namespace OficinaFramework;
using namespace SmallGame;


void GameScreen::Initialize()
{
    SetActive(true);
    SetVisible(true);
    m_gameblock = new GameBlock(RenderingSystem::GetViewportSize().toVec2() / 2.0f);
    m_gameblock->Init();
    ScreenSystem::Screen::Initialize();
}

void GameScreen::LoadContent()
{
    m_gameblock->LoadContent();
    ScreenSystem::Screen::LoadContent();
}

void GameScreen::UnloadContent()
{
    m_gameblock->UnloadContent();
    delete m_gameblock;
    m_gameblock = nullptr;
    ScreenSystem::Screen::UnloadContent();
}

void GameScreen::Update()
{
    m_gameblock->Update();
}

void GameScreen::Draw()
{
    glPushMatrix();
        m_gameblock->Draw();
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
    angle = angle >= 360.0f ? 0.0f : angle + 5.0f;
}

void GameBlock::Draw()
{
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glPushMatrix();
        vec2 halfSize = size / 2.0f;
        glTranslatef(-halfSize.x, -halfSize.y, 0.0f);
        m_blocktexture->Draw(vec2::Zero(), size, Color4::MaskToColor4(ColorDef::WHITE));
    glPopMatrix();
}
