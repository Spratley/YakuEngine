#include "PCH/CG_PCH.h"
#include "CG/CG_RenderModule.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

void CG_RenderModule::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(m_glfwWindow);
}