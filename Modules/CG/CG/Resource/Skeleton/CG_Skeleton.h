#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Matrix.h"

// TODO: Replace with YK_DynamicArray
#include <string>
#include <vector>

struct CG_Skeleton
{
    struct Bone
    {
        Bone const* m_parent = nullptr;
        // I'm not the biggest fan of using the transform component here because techncially it's supposed to be FOR the
        // ECS, but really it's exactly what a bone needs
        YK_TransformComponent m_bindPoseTransform{};
#if !YAKU_RETAIL
        std::string m_name = "?";
#endif // !YAKU_RETAIL
    };

    std::vector<YK_Matrix44> m_inverseBindMatrices;
    std::vector<Bone> m_bones;
};