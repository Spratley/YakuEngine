#pragma once
#if YK_WINDOWS

struct GLFWwindow;

class YK_WindowsWindow
{
public:
	YK_WindowsWindow(YK_U32 p_width, YK_U32 p_height);
	~YK_WindowsWindow();

	bool IsValid() const { return m_glfwWindow != nullptr; }
	bool ShouldClose() const;

	GLFWwindow* GetGLFWWindow() const { return m_glfwWindow; }
	void* GetHWND() const;

private:
	GLFWwindow* m_glfwWindow = nullptr;
};
#endif // YK_WINDOWS