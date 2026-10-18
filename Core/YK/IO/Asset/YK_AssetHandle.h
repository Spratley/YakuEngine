#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Asset/YK_AssetConcepts.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <algorithm>
#include <utility>

// Asset garbage collection is done through deferred reference counting
// Long term I'd like to replace this with a tri-color mark and sweep GC
// While YakuEn is small and my needs aren't that deep though, this is good enough

template <YK_AssetType AssetType>
struct YK_AssetHandle
{
    friend class YK_Asset;

public:
    static const YK_AssetHandle<AssetType> EmptyHandle;

private:
    constexpr YK_AssetHandle(AssetType const* p_asset)
        : m_asset(p_asset)
    {}

public:
    constexpr YK_AssetHandle() = default;

    constexpr YK_AssetHandle(YK_AssetHandle const& p_other)
    {
        if (!p_other.IsValid())
        {
            // Leave as empty handle
            return;
        }
        *this = std::move(p_other.m_asset->GetHandle());
    }

    constexpr YK_AssetHandle(YK_AssetHandle&& p_other)
        : m_asset(YK_Exchange(p_other.m_asset, nullptr))
        , m_generation(YK_Exchange(p_other.m_generation, 0))
    {}

    constexpr ~YK_AssetHandle() { ReleaseAsset(); }

    constexpr YK_AssetHandle& operator=(YK_AssetHandle const& p_other)
    {
        ReleaseAsset();
        if (!p_other.IsValid())
        {
            // Leave as empty handle
            return *this;
        }
        *this = std::move(p_other.m_asset->GetHandle());
        return *this;
    }

    constexpr YK_AssetHandle& operator=(YK_AssetHandle&& p_other)
    {
        m_asset = YK_Exchange(p_other.m_asset, nullptr);
        m_generation = YK_Exchange(p_other.m_generation, 0);
        return *this;
    }

    constexpr AssetType const& operator*() const
    {
        YK_ASSERT(IsValid(), "Attempting to dereference an invalid asset!");
        return *m_asset;
    }

    constexpr AssetType const* operator->() const
    {
        YK_ASSERT(IsValid(), "Attempting to pointer-access an invalid asset!");
        return m_asset;
    }

    constexpr bool operator==(YK_AssetHandle const& p_other) const
    {
        // TODO: Not the actual comparison here
        return m_asset != p_other.m_asset;
    }

    constexpr bool operator!=(YK_AssetHandle const& p_other) const { return !(*this == p_other); }

    constexpr bool IsValid() const
    {
        // TODO: This isn't actually what we need to check
        return m_asset != nullptr;
    }

private:
    constexpr void ReleaseAsset()
    {
        if (IsValid())
        {
            m_asset->ReturnHandle(*this);
        }
    }

private:
    AssetType const* m_asset = nullptr;
    YK_U32 m_generation = 0;
};

template <YK_AssetType AssetType>
constexpr YK_AssetHandle<AssetType> YK_AssetHandle<AssetType>::EmptyHandle{};