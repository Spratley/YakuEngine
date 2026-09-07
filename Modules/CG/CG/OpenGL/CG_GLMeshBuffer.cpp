#include "PCH/CG_PCH.h"
#include "CG_GLMeshBuffer.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "CG/Resource/Mesh/CG_Mesh.h"

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

CG_GLMeshBuffer::CG_GLMeshBuffer(CG_Mesh const& p_sourceMesh)
{
    // TODO: Come back and remember what GL_STATIC_DRAW is and if that needs to be parametrized
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * p_sourceMesh.GetVertexBufferSize(),
                 p_sourceMesh.GetVertexBuffer(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(YK_U32) * p_sourceMesh.GetIndexBufferSize(),
                 p_sourceMesh.GetIndexBuffer(),
                 GL_STATIC_DRAW);

    CG_MeshLayout const& meshLayout = p_sourceMesh.GetMeshLayout();
    YK_SizeT const stride = meshLayout.CalculateStride();
    YK_Byte* pointerOffset = nullptr;
    for (auto i : YK_CountTo(CG_MeshAttribute::COUNT))
    {
        CG_MeshAttribute const attribute = static_cast<CG_MeshAttribute>(i);
        if (meshLayout.IsEnabled(attribute))
        {
            YK_SizeT attributeSize = CG_MeshLayout::AttributeSize(attribute);
            glVertexAttribPointer(static_cast<GLuint>(i),
                                  static_cast<GLint>(attributeSize),
                                  GL_FLOAT,
                                  GL_FALSE,
                                  static_cast<GLsizei>(stride),
                                  pointerOffset);
            glEnableVertexAttribArray(static_cast<GLint>(i));
            pointerOffset += attributeSize * sizeof(float);
        }
        else
        {
            glDisableVertexAttribArray(static_cast<GLint>(i));
        }
    }
}

CG_GLMeshBuffer::~CG_GLMeshBuffer()
{
    glDeleteBuffers(1, &m_eboID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

// TODO: Figure out how to manage glBindVertexArray and unbind the data
void CG_GLMeshBuffer::Bind() const { glBindVertexArray(m_vaoID); }