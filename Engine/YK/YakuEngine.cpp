#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

// Temp
YK_Matrix44 g_modelMatrix;

// Temp 2x
HIDra::Core g_hidraCore;

bool YakuEngine::Init()
{
    if (!m_platformCore.Init())
    {
        return false;
    }

    // TODO: Make this platform agnostic
    HIDra::Core_PlatformInitData initData;
#if YK_WINDOWS
    initData.m_mainWindowHandle = m_platformCore.GetMainDisplaySurface().GetNativeHandle();
#endif

    g_hidraCore.Init(initData);

    // Also make THIS platform agnostic
    m_renderModule =
      new CG_RenderModule(m_platformCore.GetMainDisplaySurface().GetContents().m_glfwWindow); // TODO: Don't do this

    // Temp
    YK_Matrix::Translate(g_modelMatrix, YK_Vector3f(0.0f, -0.5f, -1.0f));

    return true;
}

void YakuEngine::ShutDown()
{
    YK_SAFE_DELETE(m_renderModule);

    m_platformCore.ShutDown();
}

void YakuEngine::EngineLoop()
{
    // Run Game Loop
    BeginFrame();

    HIDra::Vec2f input = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L);
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

    if (HIDra::GetButtonDown(HIDra::BID_SOUTH))
    {
        YK_LOG("Hit south!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_NORTH))
    {
        YK_LOG("Hit north!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_EAST))
    {
        YK_LOG("Hit east!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_WEST))
    {
        YK_LOG("Hit west!");
    }

    if (HIDra::GetButtonDown(HIDra::BID_DPAD_SOUTH))
    {
        YK_LOG("Hit dpad south!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_DPAD_NORTH))
    {
        YK_LOG("Hit dpad north!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_DPAD_EAST))
    {
        YK_LOG("Hit dpad east!");
    }
    if (HIDra::GetButtonDown(HIDra::BID_DPAD_WEST))
    {
        YK_LOG("Hit dpad west!");
    }

    EndFrame();
}

void YakuEngine::BeginFrame() { m_platformCore.OnFrameStart(); }

void YakuEngine::EndFrame() { HIDra::Flush(); }