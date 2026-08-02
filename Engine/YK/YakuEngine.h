#pragma once

#include "YKC/Core/YKC_Core.h"

// TEMP
#include "YKC/ECS/YKC_TEMP_TransformComponent.h"
#include "YKC/Libraries/Zen/Zen_Garden.h"
#include "YKC/Platforms/YKC_PlatformCore.h"

// Temp
struct VelocityComponent
{
    YK_Vector3f m_velocity = YK_Vector3f::Zero();
};

struct TestSystem : public Zen::SystemBase<TestSystem, TransformComponent, VelocityComponent>
{
    static void Tick(ComponentView const& p_components)
    {
        for (auto [transform, velocity] : p_components)
        {
            YK_Matrix::Translate(transform.m_transform, velocity.m_velocity);
        }
    }
};

class CG_RenderModule;

class YakuEngine : YKC_Core
{
public:
    template <typename ComponentTypes, typename SystemTypes>
    YakuEngine(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : YKC_Core(p_componentTypes, p_systemTypes)
    {}

    ~YakuEngine() {}

    template <typename Game>
    void Run();

    template <typename Game>
    Game& GetGame()
    {
        return *static_cast<Game*>(m_game);
    }
    template <typename Game>
    Game const& GetGame() const
    {
        return *static_cast<Game*>(m_game);
    }

private:
    bool Init();
    void ShutDown();
    void EngineLoop();

    void BeginFrame();
    void EndFrame();

    // TODO: This doesn't seem good
    // I'm doing this because Game's concrete type is from a higher level project and cannot be known by YakuEngine
    // Since we're storing the game as a void*, we can't safely delete it without access to it's destructor
    // Yeah technically it doesn't matter that much since
    // Not the biggest fan of the void* m_game anyway, I should probably find a better way to do all of this
    // Composition with an abstract templated type? Make Game derive YakuEngine? Who knows
    template <typename Game>
    void DeleteGame();

private:
    void* m_game = nullptr;
    YKC_PlatformCore m_platformCore;

    // Modules
    // TODO: Move to separate implementation struct so we can have linear packing and no header exposure
    CG_RenderModule* m_renderModule = nullptr;
};

template <typename Game>
void YakuEngine::Run()
{
    // Init Engine
    if (!Init())
    {
        ShutDown();
        return;
    }

    // Init Game
    m_game = new Game();
    if (!GetGame<Game>().Init())
    {
        GetGame<Game>().ShutDown();
        DeleteGame<Game>();
        ShutDown();
        return;
    }

    // TODO: Come back to this. WebAssembly requires that we don't manually loop,
    // and so this ugly back and forth needs to be here to support it.
    // However, it takes up a callstack frame that wouldn't need to be if we were directly looping ourselves
    // Since most platforms DON'T have an asynchronous loop,
    // we can probably figure out a more elegant way to get that stack memory back
    m_platformCore.LaunchCoreLoop([](void* p_context) { static_cast<YakuEngine*>(p_context)->EngineLoop(); }, this);

    // Shut Down
    GetGame<Game>().ShutDown();
    ShutDown();
}

template <typename Game>
void YakuEngine::DeleteGame()
{
    delete static_cast<Game*>(m_game);
    m_game = nullptr;
}