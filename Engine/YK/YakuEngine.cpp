#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

// Temp
YK_Matrix44 g_modelMatrix;

bool YakuEngine::Init()
{
    if (!m_platformCore.Init())
    {
        return false;
    }

// TODO: Make this platform agnostic
#if YK_WINDOWS
    HIDra::PlatformCoreInitData_Windows initData;
    initData.m_mainWindowHandle = m_platformCore.GetMainDisplaySurface().GetNativeHandle();
#elif YK_WEB_ASSEMBLY
    HIDra::PlatformCoreInitData_WASM initData;
#endif

    HIDra::Init(initData);

    // Also make THIS platform agnostic
    m_renderModule =
      new CG_RenderModule(m_platformCore.GetMainDisplaySurface().GetContents().m_glfwWindow); // TODO: Don't do this

    // Temp
    YK_Matrix::Translate(g_modelMatrix, YK_Vector3f(0.0f, 0.0f, -1.0f));

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
    BeginFrame();

    // HIDra::Vec2f input = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L);
    HIDra::Vec2f input;
    if (HIDra::GetKey(HIDra::KEYCODE_S))
    {
        input.m_y = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_W))
    {
        input.m_y = 1;
    }

    // TODO: Converter function
    YK_Vector3f frameDelta(input.m_x, 0.0f, input.m_y);
    frameDelta *= (1.0f / 500.0f);
    YK_Matrix::Translate(g_modelMatrix, frameDelta);

    m_renderModule->Render(g_modelMatrix);

    EndFrame();
}

void YakuEngine::BeginFrame() { m_platformCore.OnFrameStart(); }

void YakuEngine::EndFrame() { HIDra::Flush(); }