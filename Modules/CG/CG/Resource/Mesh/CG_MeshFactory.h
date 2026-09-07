#pragma once

#include "YK/Types/Math/YK_Integer.h"

#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG_MeshLayout.h"

class CG_MeshFactory
{
public:
    static CG_Mesh FromData(float const* p_vertexData,
                            YK_U32 p_vertexDataCount,
                            YK_U32 const* p_indexData,
                            YK_U32 p_indexCount,
                            CG_MeshLayout p_meshLayout);
    static CG_Mesh Quad();
};