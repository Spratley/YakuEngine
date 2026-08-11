#include "PCH/CG_PCH.h"
#include "CG_Shader.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "YK/IO/File/YK_IOFile.h"
#include "YK/Types/Math/YK_Integer.h"

#include <string>
#include <sstream>

namespace CG_Shader_Private
{
#if YK_PLATFORM == YK_WASM
    constexpr char const* shaderVersion = "#version 300 es\n";
#else
    constexpr char const* shaderVersion = "#version 330 core\n";
#endif
    constexpr YK_SizeT shaderVersionLength = std::char_traits<char>::length(shaderVersion);
} // namespace CG_Shader_Private

CG_Shader::CG_Shader(char const* p_vertexPath, char const* p_fragmentPath)
    : m_id(0)
{
    InitShader(p_vertexPath, p_fragmentPath);
}

void CG_Shader::Use() const { glUseProgram(m_id); }

void CG_Shader::SetBool(char const* p_name, bool p_value) const
{
    glUniform1i(glGetUniformLocation(m_id, p_name), static_cast<YK_Int32>(p_value));
}

void CG_Shader::SetInt(char const* p_name, YK_Int32 p_value) const
{
    glUniform1i(glGetUniformLocation(m_id, p_name), p_value);
}

void CG_Shader::SetFloat(char const* p_name, float p_value) const
{
    glUniform1f(glGetUniformLocation(m_id, p_name), p_value);
}

void CG_Shader::SetMatrix44(char const* p_name, float const* p_buffer) const
{
    const YK_U32 matrixLocation = glGetUniformLocation(m_id, p_name);
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, p_buffer);
}

void CG_Shader::InitShader(char const* p_vertexPath, char const* p_fragmentPath)
{
    // TODO: Replace with custom string implementation
    std::string vertexCode;
    std::string fragmentCode;

    std::stringstream vertexShaderStream;
    std::stringstream fragmentShaderStream;

    YK_IFile::GetFileContents(p_vertexPath, vertexShaderStream);
    YK_IFile::GetFileContents(p_fragmentPath, fragmentShaderStream);

    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();

    YK_U32 vertexShaderID;
    YK_U32 fragmentShaderID;

    CompileShader(vertexShaderID, vertexCode, GL_VERTEX_SHADER);
    CompileShader(fragmentShaderID, fragmentCode, GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShaderID);
    glAttachShader(m_id, fragmentShaderID);
    glLinkProgram(m_id);
    LogShaderErrors(m_id, GL_LINK_STATUS);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void CG_Shader::CompileShader(YK_U32& p_outID, std::string const& p_shaderCode, YK_U32 p_shaderType) const
{
    p_outID = glCreateShader(p_shaderType);

    char const* sources[]{ CG_Shader_Private::shaderVersion, p_shaderCode.c_str() };
    YK_Int32 const sourceLengths[]{ CG_Shader_Private::shaderVersionLength, static_cast<YK_Int32>(p_shaderCode.length()) };
    glShaderSource(p_outID, 2, sources, sourceLengths);
    glCompileShader(p_outID);

    LogShaderErrors(p_outID, GL_COMPILE_STATUS);
}

void CG_Shader::LogShaderErrors(YK_U32 p_shaderID, YK_U32 p_errorType) const
{
#if !YAKU_RETAIL
    YK_Int32 success;

    switch (p_errorType)
    {
        case GL_LINK_STATUS: glGetProgramiv(p_shaderID, p_errorType, &success); break;
        default:             glGetShaderiv(p_shaderID, p_errorType, &success); break;
    }

    if (!success)
    {
        char infoLog[512];

        switch (p_errorType)
        {
            case GL_LINK_STATUS: glGetProgramInfoLog(p_shaderID, 512, NULL, infoLog); break;
            default:             glGetShaderInfoLog(p_shaderID, 512, NULL, infoLog); break;
        }

        YK_LOG_ERROR("Shader compilation error!");
        YK_LOG_ERROR(infoLog);
    }
#else
    YK_Unused(p_shaderID);
    YK_Unused(p_errorType);
#endif // !YAKU_RETAIL
}