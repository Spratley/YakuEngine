#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"

#include <vector>

class CG_Material;
class CG_Mesh;
struct YK_TransformComponent;

class CG_RenderQueue
{
public:
    void Clear() { m_queue.clear(); }
    void Allocate(YK_SizeT p_count) { m_queue.reserve(p_count); }
    void Push(CG_Material const& p_material, CG_Mesh const& p_mesh, YK_TransformComponent const& p_transform);
    void Bake();

    auto begin() const { return m_queue.begin(); }
    auto end() const { return m_queue.end(); }

    struct Entry
    {
        CG_Material const* m_material;
        CG_Mesh const* m_mesh;
        YK_Matrix44 m_transform;
    };

private:
    std::vector<Entry> m_queue;
};