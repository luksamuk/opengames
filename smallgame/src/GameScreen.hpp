#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include <OficinaFramework/OficinaFramework.h>
using namespace OficinaFramework;

namespace SmallGame
{
    class GameBlock : public EntitySystem::DrawableEntity
    {
    private:
        vec2 position;
        vec2 size;
        float angle;
        RenderingSystem::Texture* m_blocktexture;
    public:
        GameBlock(vec2);
        virtual ~GameBlock();
        void Init()          override;
        void LoadContent()   override;
        void UnloadContent() override;
        void Update()        override;
        void Draw()          override;
    };


    class GameScreen : public ScreenSystem::Screen
    {
    private:
        EntitySystem::DrawableEntityCollection* m_drawables = nullptr;
    public:
        void Initialize()    override;
        void LoadContent()   override;
        void UnloadContent() override;
        void Update()        override;
        void Draw()          override;
    };
}

#endif // GAMESCREEN_HPP
