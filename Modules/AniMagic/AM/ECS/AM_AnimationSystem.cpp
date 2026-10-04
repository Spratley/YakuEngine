#include "PCH/AniMagic_PCH.h"
#include "AM_AnimationSystem.h"

#include "YK/Libraries/Zen/System/Zen_System.h"
#include "YK/Time/YK_Time.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

void AM_AnimationSystem::Tick(ComponentView p_components)
{
    float const deltaTime = YK_Time::DeltaTime();

    for (auto [poseComponent, animationComponent] : p_components)
    {
        animationComponent.m_sampleTime += deltaTime;
        for (auto i : YK_CountTo(poseComponent.m_pose.size()))
        {
            poseComponent.m_pose[i] =
              animationComponent.m_poseSampler.Sample(static_cast<YK_U8>(i), animationComponent.m_sampleTime);
        }
    }
}
