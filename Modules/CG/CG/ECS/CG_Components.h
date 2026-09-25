#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"

#include <vector>

class CG_Material;
class CG_Mesh;
struct CG_Skeleton;

// TODO: Split these into separate files

struct CG_RendererComponent
{
    CG_Material const* m_material;
};

struct CG_MeshComponent
{
    CG_Mesh const* m_mesh = nullptr;
};

struct CG_SkeletalMeshComponent : public CG_MeshComponent
{
    CG_Skeleton const* m_skeleton = nullptr;
};

struct CG_PoseComponent
{
    std::vector<YK_TransformComponent> m_pose;
};