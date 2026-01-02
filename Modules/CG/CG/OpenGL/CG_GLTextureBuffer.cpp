#include "PCH/CG_PCH.h"
#include "CG_GLTextureBuffer.h"

#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YKC/Libraries/OpenGL/GLFW/include/glfw3.h>

#include "CG/Texture/CG_Texture.h"

CG_GLTextureBuffer::CG_GLTextureBuffer(CG_Texture const& p_texture)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// TODO: Expose as texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO: Determine from texture members
	YK_U32 format = p_texture.GetNrChannels() == 3 ? GL_RGB : GL_RGBA; // Don't assume RGB or RGBA, there are other options
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_texture.GetWidth(), p_texture.GetHeight(), 0, format, GL_UNSIGNED_BYTE, p_texture.GetBuffer());

	// TODO: Mipmap as paramter
	// glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

CG_GLTextureBuffer::~CG_GLTextureBuffer()
{
	glDeleteTextures(1, &m_id);
}

void CG_GLTextureBuffer::Bind(YK_U8 p_textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + p_textureSlot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}