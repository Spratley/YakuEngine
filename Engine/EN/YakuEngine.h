#pragma once

#include "EN/Modules/EN_ModuleRegistry.h"

#include "YK/Core/YK_Core.h"

// Temp
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "YK/IO/Asset/YK_AssetStorage.h"

class YakuEngine : public YK_Core
{
public:
    template <typename ComponentTypes, typename SystemTypes>
    YakuEngine(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : YK_Core(p_componentTypes, p_systemTypes)
    {}

    ~YakuEngine() = default;

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

    YK_AssetStorage<CG_Mesh, CG_MeshLoader>& GetMfUhhAssets() { return m_meshStorage; }

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

    EN_ModuleRegistry m_modules;

    // TEMP
    YK_AssetStorage<CG_Mesh, CG_MeshLoader> m_meshStorage;
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
    if (!GetGame<Game>().Init(*this))
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
    LaunchCoreLoop([](void* p_context) { static_cast<YakuEngine*>(p_context)->EngineLoop(); }, this);

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