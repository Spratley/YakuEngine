#pragma once

// TODO: Decouple OpenGL and shaders
// This is really a GL shader class, not a generic one
// We need to separate the rendering API and the user-facing code so we can swap what renderer is being used per-platform

class CG_Shader
{
public:
	CG_Shader() = default;
	CG_Shader(const char* p_vertexPath, const char* p_fragmentPath);

	CG_Shader& operator=(CG_Shader const& p_otherShader) { m_id = p_otherShader.m_id; return *this; }

	void Use() const;

	// TODO: Cache uniform locations
	void SetBool(const char* p_name, bool p_value) const;
	void SetInt(const char* p_name, YK_Int32 p_value) const;
	void SetFloat(const char* p_name, float p_value) const;
	void SetMatrix44(const char* p_name, float const* p_buffer) const;

	// Alias for SetInt for readability
	inline void SetTextureSamplerSlot(const char* p_textureSamplerName, YK_Int32 p_slot) const { SetInt(p_textureSamplerName, p_slot); }

private:
	void InitShader(const char* p_vertexPath, const char* p_fragmentPath);
	void CompileShader(YK_U32& p_outID, const char* p_shaderCode, YK_U32 p_shaderType) const;
	void LogShaderErrors(YK_U32 p_shaderID, YK_U32 p_errorType) const;

private:
	YK_U32 m_id;
};