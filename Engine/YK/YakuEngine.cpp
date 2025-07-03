#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

// TODO: Figure out how to make this platform agnostic at this level
#include "YK/Platforms/YK_PlatformDefines.h"
#include "YK/Platforms/Windows/YK_WindowsWindow.h"

// Temp
YK_Matrix44 g_modelMatrix;

bool YakuEngine::Init()
{
	if (!m_platformCore.Init())
	{
		return false;
	}

	HIDra::PlatformCoreInitData_Windows initData;
	initData.m_mainWindowHandle = m_platformCore.GetMainWindow()->GetHWND();
	HIDra::Init(initData);

	m_renderModule = new CG_RenderModule(m_platformCore.GetMainWindow()->GetGLFWWindow()); // TODO: Don't do this

	return true;
}

void YakuEngine::ShutDown()
{
	YK_SAFE_DELETE(m_game);

	YK_SAFE_DELETE(m_renderModule);

	m_platformCore.ShutDown();
}

void YakuEngine::EngineLoop()
{
	// Run Game Loop
	while (!m_platformCore.ShouldClose())
	{
		BeginFrame();

		HIDra::Vec2f input = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L);
		// TODO: Converter function
		YK_Vector3f frameDelta(input.m_x, 0.0f, input.m_y);
		frameDelta *= (1.0f / 500.0f);
		YK_Matrix::Translate(g_modelMatrix, frameDelta);

		m_renderModule->Render(g_modelMatrix);

		EndFrame();
	}
}

void YakuEngine::BeginFrame()
{
	m_platformCore.OnFrameStart();
}

void YakuEngine::EndFrame()
{
	HIDra::Flush();
}