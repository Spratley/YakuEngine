#pragma once

#include "YK/Platforms/YK_PlatformCore.h"

class CG_RenderModule;

class YakuEngine
{
public:
	YakuEngine() = default;
	~YakuEngine() {}

	template <class Game>
	void Run();

	template <class Game>
	Game& GetGame() { return *static_cast<Game*>(m_game); }
	template <class Game>
	Game const& GetGame() const { return *static_cast<Game*>(m_game); }

private:
	bool Init();
	void ShutDown();
	void EngineLoop();

	void BeginFrame();
	void EndFrame();

private:
	void* m_game = nullptr;
	PlatformCore m_platformCore;

	// Modules
	// TODO: Replace with smart pointers
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