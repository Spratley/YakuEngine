#include "PCH/AniMagic_PCH.h"
#include "AM_AnimationSystem.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Libraries/Zen/System/Zen_System.h"
#include "YK/Time/YK_Time.h"

#include "CG/Resource/Animation/CG_Animation.h"

void AM_AnimationSystem::Tick(ComponentView p_components)
{
    float const deltaTime = YK_Time::DeltaTime();

    for (auto [poseComponent, animationComponent] : p_components)
    {
        if (!animationComponent.m_animation)
        {
            continue;
        }

        animationComponent.m_sampleTime += deltaTime;
        if (animationComponent.m_sampleTime > 4.0f)
        {
            animationComponent.m_sampleTime = 0.0f;
        }

        CG_Animation const* animation = animationComponent.m_animation;
        for (CG_Animation::PositionChannel const& positionChannel : animation->m_positionChannels)
        {
            YK_TransformComponent& boneTransform = poseComponent.m_pose[positionChannel.m_boneIndex];
            boneTransform.m_position = positionChannel.Sample(animationComponent.m_sampleTime);
        }

        for (CG_Animation::OrientationChannel const& orientationChannel : animation->m_orientationChannels)
        {
            YK_TransformComponent& boneTransform = poseComponent.m_pose[orientationChannel.m_boneIndex];
            boneTransform.m_orientation = orientationChannel.Sample(animationComponent.m_sampleTime);
        }

        for (CG_Animation::ScaleChannel const& scaleChannel : animation->m_scaleChannels)
        {
            YK_TransformComponent& boneTransform = poseComponent.m_pose[scaleChannel.m_boneIndex];
            boneTransform.m_scale = scaleChannel.Sample(animationComponent.m_sampleTime);
        }
    }
}
