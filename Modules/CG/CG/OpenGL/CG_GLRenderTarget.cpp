#include "PCH/CG_PCH.h"
#include "CG_GLRenderTarget.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "CG_GLViewportHelper.h"

CG_GLRenderTarget::CG_GLRenderTarget(YK_Vector2i p_size)
    : m_size(p_size)
    , m_frameBufferID(0)
    , m_colorBufferID(0)
    , m_depthBufferID(0)
{
    glGenFramebuffers(1, &m_frameBufferID);
    Binding binding = Bind();

    // Generate color buffer
    // TODO: Move image generation to texture buffer class
    glGenTextures(1, &m_colorBufferID);
    glBindTexture(GL_TEXTURE_2D, m_colorBufferID);
    // Don't assume alpha is wanted!
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_size.x, p_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferID, 0);

    // Generate depth buffer
    // TODO: Move render buffer generation to render buffer class
    glGenRenderbuffers(1, &m_depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, p_size.x, p_size.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

    // Validate status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        YK_LOG_ERROR("Framebuffer not complete!");
    }
}

CG_GLRenderTarget::~CG_GLRenderTarget()
{
    glDeleteRenderbuffers(1, &m_depthBufferID);
    glDeleteTextures(1, &m_colorBufferID);
    glDeleteFramebuffers(1, &m_frameBufferID);
}

CG_GLRenderTarget::Binding::~Binding()
{
    // Should this be popping to the last frame buffer and not the default?
    // Is there ever a world where we stop one render pass to start another?
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CG_GLViewportHelper::SetViewportSize(m_restoreSize);
}

CG_GLRenderTarget::Binding CG_GLRenderTarget::Bind() const
{
    YK_Vector2i const restoreSize = CG_GLViewportHelper::GetViewportSize();
    CG_GLViewportHelper::SetViewportSize(m_size);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
    return Binding(restoreSize);
}