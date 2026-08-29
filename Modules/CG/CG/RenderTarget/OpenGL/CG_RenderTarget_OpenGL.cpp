#include "PCH/CG_PCH.h"
#include "CG/RenderTarget/CG_RenderTargetBase.h"
#include "CG/RenderTarget/OpenGL/CG_RenderTargetData_OpenGL.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Vector.h"

template <>
CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::CG_RenderTargetBase(YK_Vector2i p_size)
    : m_size(p_size)
    , m_extraData()
{}

template <>
CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::~CG_RenderTargetBase<CG_RenderTargetData_OpenGL>()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_extraData.m_depthBufferID);
        glDeleteTextures(1, &m_extraData.m_colorBufferID);
        glDeleteFramebuffers(1, &m_extraData.m_frameBufferID);
    }
}

template <>
bool CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::Initialize()
{
    YK_ASSERT(!IsInitialized(), "Attempting to re-initialize a render target!");

    glGenFramebuffers(1, &m_extraData.m_frameBufferID);
    Bind();

    // Generate color buffer
    // TODO: Move image generation to texture buffer class
    glGenTextures(1, &m_extraData.m_colorBufferID);
    glBindTexture(GL_TEXTURE_2D, m_extraData.m_colorBufferID);
    // Don't assume alpha is wanted!
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_extraData.m_colorBufferID, 0);

    // Generate depth buffer
    // TODO: Move render buffer generation to render buffer class
    glGenRenderbuffers(1, &m_extraData.m_depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_extraData.m_depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_size.x, m_size.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_extraData.m_depthBufferID);

    // Validate status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        YK_LOG_ERROR("Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

template <>
bool CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::IsInitialized() const
{
    return m_extraData.m_frameBufferID != 0;
}

template <>
void CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::Bind() const
{
    // If left uninitialized, m_frameBufferID will be 0, effectively binding the screen as the target
    glBindFramebuffer(GL_FRAMEBUFFER, m_extraData.m_frameBufferID);
    glViewport(0, 0, m_size.x, m_size.y);
}

template <>
void CG_RenderTargetBase<CG_RenderTargetData_OpenGL>::BindAsInputTexture(YK_U32 p_slotIndex) const
{
    glActiveTexture(GL_TEXTURE0 + p_slotIndex);
    glBindTexture(GL_TEXTURE_2D, m_extraData.m_colorBufferID);
}