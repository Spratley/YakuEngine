#pragma once

#include "YKC/Platforms/YKC_PlatformCore.h"

class CG_RenderModule;

class YakuEngine
{
public:
    YakuEngine() = default;
    ~YakuEngine() {}

    template <class Game>
    void Run();

    template <class Game>
    Game& GetGame()
    {
        return *static_cast<Game*>(m_game);
    }
    template <class Game>
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

private:
    void* m_game = nullptr;
    YKC_PlatformCore m_platformCore;

    // Modules
    // TODO: Move to separate implementation struct so we can have linear packing and no header exposure
    CG_RenderModule* m_renderModule;
};

template <class Game>
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
        ShutDown();
        return;
    }

    EngineLoop();

    // Shut Down
    GetGame<Game>().ShutDown();
    ShutDown();
}