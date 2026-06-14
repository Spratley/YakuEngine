#include "PCH/CG_PCH.h"
#include "CG_GLTextureBuffer.h"

#if YK_WEB_ASSEMBLY
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YKC/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

#include "CG/Resource/Texture/CG_Texture.h"

CG_GLTextureBuffer::CG_GLTextureBuffer(CG_Texture const& p_texture)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // TODO: Expose as texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // TODO: Determine from texture members
    // Don't assume RGB or RGBA, there are other options
    YK_U32 internalFormat = p_texture.GetNrChannels() == 3 ? GL_RGB8 : GL_RGBA8;
    YK_U32 format = p_texture.GetNrChannels() == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internalFormat,
                 p_texture.GetWidth(),
                 p_texture.GetHeight(),
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 p_texture.GetBuffer());

    // TODO: Mipmap as paramter
    // glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

CG_GLTextureBuffer::~CG_GLTextureBuffer() { glDeleteTextures(1, &m_id); }

void CG_GLTextureBuffer::Bind(YK_U8 p_textureSlot) const
{
    glActiveTexture(GL_TEXTURE0 + p_textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}