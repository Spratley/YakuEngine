#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"

#include "CG/Resource/CG_ResourceHandles.h"

#include <vector>

// TODO: Split these into separate files
struct CG_RendererComponent
{
    CG_MaterialHandle m_material = CG_MaterialHandle::EmptyHandle;
};

struct CG_MeshComponent
{
    CG_MeshHandle m_mesh = CG_MeshHandle::EmptyHandle;
};

struct CG_SkeletalMeshComponent : public CG_MeshComponent
{
    CG_SkeletonHandle m_skeleton = CG_SkeletonHandle::EmptyHandle;
};

struct CG_PoseComponent
{
    std::vector<YK_Transform> m_pose;
};