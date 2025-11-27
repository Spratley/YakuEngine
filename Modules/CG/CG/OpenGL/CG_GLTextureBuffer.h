#pragma once

class CG_Texture;

// TODO: Don't do this?? Figure out a generic way to mediate this without an opengl class being directly interfaced

// TODO: Buffer ID pooling
// Again, whatever that means?
struct CG_GLTextureBuffer
{
public:
	CG_GLTextureBuffer() = delete;
	CG_GLTextureBuffer(CG_Texture const& p_texture);
	~CG_GLTextureBuffer();

	void Bind(YK_U8 p_textureSlot) const;
private:
	YK_U32 m_id;
};