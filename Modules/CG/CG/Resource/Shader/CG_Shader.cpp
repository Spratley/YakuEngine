#include "PCH/CG_PCH.h"
#include "CG_Shader.h"

#if YK_WEB_ASSEMBLY
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "YKC/IO/File/YKC_IOFile.h"

CG_Shader::CG_Shader(const char* p_vertexPath, const char* p_fragmentPath)
    : m_id(0)
{
    InitShader(p_vertexPath, p_fragmentPath);
}

void CG_Shader::Use() const { glUseProgram(m_id); }

void CG_Shader::SetBool(const char* p_name, bool p_value) const
{
    glUniform1i(glGetUniformLocation(m_id, p_name), static_cast<YK_Int32>(p_value));
}

void CG_Shader::SetInt(const char* p_name, YK_Int32 p_value) const
{
    glUniform1i(glGetUniformLocation(m_id, p_name), p_value);
}

void CG_Shader::SetFloat(const char* p_name, float p_value) const
{
    glUniform1f(glGetUniformLocation(m_id, p_name), p_value);
}

void CG_Shader::SetMatrix44(const char* p_name, float const* p_buffer) const
{
    const YK_U32 matrixLocation = glGetUniformLocation(m_id, p_name);
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, p_buffer);
}

void CG_Shader::InitShader(const char* p_vertexPath, const char* p_fragmentPath)
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

    const char* rawVertexCode = vertexCode.c_str();
    const char* rawFragmentCode = fragmentCode.c_str();

    YK_U32 vertexShaderID;
    YK_U32 fragmentShaderID;

    CompileShader(vertexShaderID, rawVertexCode, GL_VERTEX_SHADER);
    CompileShader(fragmentShaderID, rawFragmentCode, GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShaderID);
    glAttachShader(m_id, fragmentShaderID);
    glLinkProgram(m_id);
    LogShaderErrors(m_id, GL_LINK_STATUS);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void CG_Shader::CompileShader(YK_U32& p_outID, const char* p_shaderCode, YK_U32 p_shaderType) const
{
    p_outID = glCreateShader(p_shaderType);
    glShaderSource(p_outID, 1, &p_shaderCode, NULL);
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
    YK_UNUSED(p_shaderID);
    YK_UNUSED(p_errorType);
#endif // !YAKU_RETAIL
}