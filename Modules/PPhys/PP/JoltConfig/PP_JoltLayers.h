#pragma once

#include <Jolt/Jolt.h>
#include "Jolt/Core/IssueReporting.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include "Jolt/Physics/Collision/ObjectLayer.h"

struct PP_JoltLayers
{
    enum Layers : JPH::ObjectLayer
    {
        STATIC,
        DYNAMIC,
        COUNT
    };

    // 1:1 Mapping, good enough for now
    static constexpr JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer p_layer)
    {
        return JPH::BroadPhaseLayer{ static_cast<JPH::BroadPhaseLayer::Type>(p_layer) };
    }
};

class PP_JoltBroadPhaseLayerInterface : public JPH::BroadPhaseLayerInterface
{
public:
    YK_U32 GetNumBroadPhaseLayers() const override { return PP_JoltLayers::COUNT; }

    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer p_layer) const override
    {
        JPH_ASSERT(p_layer < PP_JoltLayers::COUNT);
        return PP_JoltLayers::GetBroadPhaseLayer(p_layer);
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
    {
        switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer))
        {
            case static_cast<JPH::BroadPhaseLayer::Type>(PP_JoltLayers::STATIC):  return "Static";
            case static_cast<JPH::BroadPhaseLayer::Type>(PP_JoltLayers::DYNAMIC): return "Dynamic";
            default:                                                              JPH_ASSERT(false); return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
};

class PP_JoltObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
public:
    bool ShouldCollide(JPH::ObjectLayer p_a, JPH::ObjectLayer p_b) const override
    {
        switch (p_a)
        {
            case PP_JoltLayers::STATIC:  return p_b == PP_JoltLayers::DYNAMIC;
            case PP_JoltLayers::DYNAMIC: return true;
            default:                     JPH_ASSERT(false); return false;
        }
    }
};

class PP_JoltObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer p_objectLayer, JPH::BroadPhaseLayer p_broadPhaseLayer) const override
    {
        switch (p_objectLayer)
        {
            case PP_JoltLayers::STATIC:  return p_broadPhaseLayer.GetValue() == PP_JoltLayers::DYNAMIC;
            case PP_JoltLayers::DYNAMIC: return true;
            default:                     JPH_ASSERT(false); return false;
        }
    }
};