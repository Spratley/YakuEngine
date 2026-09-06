#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Other/YK_DeferredConstructible.h"

#include "PP/JoltConfig/PP_JoltLayers.h"

#include <Jolt/Jolt.h>
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Physics/PhysicsSystem.h"

namespace Zen
{
    class Garden;
}

class PP_PhysicsModule
{
public:
    static PP_PhysicsModule& GetPhysicsInstance() { return *s_instance; }

private:
    static inline PP_PhysicsModule* s_instance = nullptr;

public:
    PP_PhysicsModule(YK_SizeT p_allocatedMemory);
    ~PP_PhysicsModule();

    PP_PhysicsModule(PP_PhysicsModule const&) = delete;
    PP_PhysicsModule(PP_PhysicsModule&&) = delete;

    PP_PhysicsModule& operator=(PP_PhysicsModule const&) = delete;
    PP_PhysicsModule& operator=(PP_PhysicsModule&&) = delete;

    void Update(Zen::Garden& p_entityGarden);

    JPH::PhysicsSystem& GetJoltInstance() { return m_joltInstance; }

private:
    JPH::PhysicsSystem m_joltInstance;
    YK_DeferredConstructible<JPH::TempAllocatorImpl> m_tempAllocator;
    YK_DeferredConstructible<JPH::JobSystemThreadPool> m_jobSystem;

    PP_JoltBroadPhaseLayerInterface m_broadPhaseLayerInterface;
    PP_JoltObjectVsBroadPhaseLayerFilter m_objectVsBroadPhaseLayerFilter;
    PP_JoltObjectLayerPairFilter m_objectLayerPairFilter;
};