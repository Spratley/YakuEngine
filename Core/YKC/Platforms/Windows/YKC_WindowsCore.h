#pragma once
#if YK_WINDOWS

#include "YKC/Platforms/YKC_PlatformCoreBase.inl"

class YKC_WindowsWindow;

class YKC_WindowsCore : public YKC_PlatformCoreBase<YKC_WindowsCore>
{
public:
	bool Init();
	void ShutDown();

	bool ShouldClose() const;

	void OnFrameStart() const;

	YKC_WindowsWindow* GetMainWindow() { return m_mainWindow; }
	YKC_WindowsWindow const* GetMainWindow() const { return m_mainWindow; }

private:
	YKC_WindowsWindow* m_mainWindow = nullptr;
};

using PlatformCore = YKC_WindowsCore;

#endif // YK_WINDOWS