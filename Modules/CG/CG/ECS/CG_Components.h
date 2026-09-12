#pragma once

class CG_Material;
class CG_Mesh;
struct CG_Skeleton;

struct CG_MeshComponent
{
    CG_Mesh const* m_mesh = nullptr;
    CG_Skeleton const* m_skeleton = nullptr;
};

struct CG_RendererComponent
{
    CG_Material const* m_material;
};