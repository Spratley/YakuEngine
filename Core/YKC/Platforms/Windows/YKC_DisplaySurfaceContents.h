#pragma once

#if YK_PLATFORM == YK_WINDOWS

struct GLFWwindow;

struct YKC_DisplaySurfaceContents
{
	GLFWwindow* m_glfwWindow = nullptr;
};

#endif // YK_PLATFORM == YK_WINDOWS