#pragma once

class CG_Material;
class CG_Mesh;
struct CG_Skeleton;

// TEMP
struct CG_Animation;

struct CG_MeshComponent
{
    CG_Mesh const* m_mesh = nullptr;
    CG_Skeleton const* m_skeleton = nullptr;

    // TEMP
    CG_Animation const* m_animation = nullptr;
};

struct CG_RendererComponent
{
    CG_Material const* m_material;
};