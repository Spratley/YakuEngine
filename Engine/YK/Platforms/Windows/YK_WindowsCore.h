#pragma once
#if YK_WINDOWS

class YK_WindowsWindow;

class YK_WindowsCore
{
public:
	bool Init();
	void ShutDown();

	bool ShouldClose() const;

	void OnFrameStart() const;

	const YK_WindowsWindow* GetMainWindow() const { return m_mainWindow; }

private:
	YK_WindowsWindow* m_mainWindow = nullptr;
};

using PlatformCore = YK_WindowsCore;

#endif // YK_WINDOWS