#include <OficinaFramework/OficinaFramework.h>
#include <string>
#include <list>

#include "AppScreens.hpp"
using namespace OficinaFramework;

// Screens
SmallGame::GameScreen* m_gamescreen;


int main(int argc, char** argv)
{
    std::list<string>* GameArgs = new list<string>;
    GameArgs->push_front("gamename=The Simple Small Game");
    GameArgs->push_front("iconpath=icon");
    GameArgs->push_front("windowsize=720p");
    GameArgs->push_front("resolution=720p");
    GameArgs->push_front("framerate=60hz");

    EngineCore::Initialize(argc, argv, GameArgs);

    // GohuFont
    InputSystem::BindDefaultKeys();
    ScreenSystem::Debug_SetFont(RenderingSystem::TexturePool::LoadDefaultFont());
    ScreenSystem::SetDebug(true);


    m_gamescreen = new SmallGame::GameScreen;

    ScreenSystem::AddScreen(m_gamescreen);


    return EngineCore::DoGameLoop();
}
