/// Sonic CD's D.A. Garden
/// Powered by Oficina Framework, SDL2 and OpenGL
///
/// Written by Lucas Vieira <lucas.samuel2002@gmail.com>
/// under the MIT License
///
/// NOTE: Sonic The Hedgehog and all graphics/music are Copyright
/// SEGA and SONICTEAM.
/// I do not own any of this, and this software was written
/// with the sole purpose of STUDY.
///
/// THIS SHOULD NOT BE SOLD.

// Controls:
// WASD  -> Move Little Planet
// Q/E   -> Change rotation
// Esc   -> Quit
// Start -> TO-DO
// Bcksp -> TO-DO

#include <cstdio>
#include <OficinaFramework/OficinaFramework.hpp>
#include <list>
using namespace std;
using namespace OficinaFramework;


class LittlePlanet
    : public EntitySystem::DrawableEntity
{
private:
    RenderingSystem::Texture* t_lilplanet;
    float m_rotation = -20.0f;
public:
    void Initialize() override
    {
        this->m_position = RenderingSystem::GetResolution().toVec2() / 2.0f;
    }

    void LoadContent() override
    {
        t_lilplanet =
            RenderingSystem::TexturePool::LoadTexture(
                            "sprites/littleplanet");
    }

    void UnloadContent() override
    {
        RenderingSystem::TexturePool::DisposeTexture(t_lilplanet);
    }

    void Update() override
    {
        // Rotate planet accordingly
        this->m_angle += this->m_rotation * TimingSystem::GetDeltaTime();

        // Clamp angle
        if(this->m_angle < 0.0)
            this->m_angle += 360.0;
        else if(this->m_angle > 360.0)
            this->m_angle -= 360.0;


        // Move planet
        m_position += InputSystem::GetLeftStick() * (300.0f * TimingSystem::GetDeltaTime());
        // Change rotation speed
        if(InputSystem::PressedKey(SDL_SCANCODE_E))
            this->m_rotation += 20.0f;
        else if(InputSystem::PressedKey(SDL_SCANCODE_Q))
            this->m_rotation -= 20.0f;
    }

    void Draw() override
    {
        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0.0f);
        glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
        glTranslatef(t_lilplanet->GetSize().x / -2.0f,
                    t_lilplanet->GetSize().y / -2.0f,
                    0.0f);
        t_lilplanet->Draw(vec2::Zero(), Color4::MaskToColor4(WHITE));
        glPopMatrix();
        glPopMatrix();
    }
};

class DAGarden
    : public ScreenSystem::Screen
{
private:
    LittlePlanet lp;
    RenderingSystem::Texture* t_background;
    AudioSystem::Audio* m_dagarden;
    AudioSystem::AudioSource* m_src;
public:
    DAGarden() {
        SetVisible(true);
        SetActive(true);
    }
    ~DAGarden() {}

    void Initialize() override
    {
        lp.Initialize();
        ScreenSystem::Screen::Initialize();
    }

    void LoadContent() override
    {
        lp.LoadContent();
        t_background = RenderingSystem::TexturePool::LoadTexture("sprites/background");
        m_dagarden = AudioSystem::AudioPool::LoadAudio("music/DAGarden",
                AudioSystem::OF_AUDIO_TYPE_OGG, true);
        m_src = new AudioSystem::AudioSource;
        m_src->Play(m_dagarden);
        ScreenSystem::Screen::LoadContent();
    }
    
    void UnloadContent() override
    {
        m_src->Stop();
        AudioSystem::AudioPool::UnloadAudio(m_dagarden);
        delete m_src;
        lp.UnloadContent();
        RenderingSystem::TexturePool::DisposeTexture(t_background);
        ScreenSystem::Screen::UnloadContent();
    }

    void Update() override
    {
        lp.Update();
        if(InputSystem::PressedKey(SDL_SCANCODE_ESCAPE))
            InputSystem::CallExitCommand();
    }

    void Draw() override
    {
        RenderingSystem::SetCameraPosition(RenderingSystem::GetResolution().toVec2() / 2.0f);
        glPushMatrix();
        glTranslatef(RenderingSystem::GetResolution().x / 2.0f,
                RenderingSystem::GetResolution().y / 2.0f,
                0.0f);
        glPushMatrix();
        glTranslatef(t_background->GetSize().x / -2.0f,
                t_background->GetSize().y / -2.0f,
                0.0f);
        t_background->Draw(vec2::Zero(), Color4::MaskToColor4(WHITE));
        glPopMatrix();
        glPopMatrix();
        lp.Draw();
    }
};

int main(int argc, char** argv)
{
    std::list<std::string>* confv = new std::list<std::string>;
    confv->push_back("resolution=960x540");
    confv->push_back("gamename=D.A. Garden (OficinaFramework C++/SDL2/OpenGL)");
    EngineCore::Initialize(argc, argv, confv);
    InputSystem::BindDefaultKeys();
    RenderingSystem::SetLinearFiltering(false);
    ScreenSystem::SetDebug(true);
    ScreenSystem::Debug_SetFont(RenderingSystem::TexturePool::LoadDefaultFont());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ScreenSystem::AddScreen(new DAGarden);
    return EngineCore::DoGameLoop();
}
