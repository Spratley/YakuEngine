#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Asset/YK_AssetConcepts.h"
#include "YK/IO/Asset/YK_AssetHandle.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Types/Containers/YK_ColonyArray.h"
#include "YK/Types/Identifier/YK_UID.h"

#include <algorithm>
#include <concepts>
#include <unordered_map>
#include <utility>

template <typename Loader, typename AssetType>
concept YK_AssetLoaderType = YK_AssetType<AssetType> && requires(YK_FilePath const& p_path) {
    { Loader::Load(p_path) } -> std::convertible_to<AssetType>;
};

// TODO: What is this? Why is this here?? Move somewhere else
template <typename... Types>
struct YK_TypeList
{};

template <YK_AssetType AssetType>
class YK_AssetStorage
{
public:
    YK_AssetStorage() = delete;

    template <YK_AssetLoaderType<AssetType> AssetLoader>
    YK_AssetStorage(YK_TypeList<AssetLoader> p_loaderType);

    YK_AssetHandle<AssetType> GetAsset(YK_FilePath const& p_localDataPath);
    YK_AssetHandle<AssetType> GetAsset(YK_UID p_uid) const;

    // TODO: Replace with concepts for actual load function parameters
    bool LoadAsset(YK_FilePath const& p_localDataPath);
    bool InsertAsset(AssetType&& p_asset, YK_UID p_uid);

    bool UnloadAsset(YK_FilePath const& p_localDataPath) { return UnloadAsset(YK_UID{ p_localDataPath.CString() }); }
    bool UnloadAsset(YK_UID p_uid);

    void CleanupGarbage();

private:
    bool HasAsset(YK_UID p_uid) { return m_loadedAssets.contains(p_uid); }

private:
    using ContainerType = YK_ColonyArray<AssetType, 64>;
    std::unordered_map<YK_UID, typename ContainerType::Index> m_loadedAssets;
    ContainerType m_dataColony;

    using LoadAssetFunctionSignature = AssetType (*)(YK_FilePath const&);
    LoadAssetFunctionSignature m_loadAssetFunction;
};

template <YK_AssetType AssetType>
template <YK_AssetLoaderType<AssetType> AssetLoader>
inline YK_AssetStorage<AssetType>::YK_AssetStorage(YK_TypeList<AssetLoader>)
    : m_loadedAssets()
    , m_dataColony()
    , m_loadAssetFunction(AssetLoader::Load)
{}

template <YK_AssetType AssetType>
YK_AssetHandle<AssetType> YK_AssetStorage<AssetType>::GetAsset(YK_FilePath const& p_localDataPath)
{
    YK_UID assetUID{ p_localDataPath.CString() };
    if (!HasAsset(assetUID))
    {
        LoadAsset(p_localDataPath);
        YK_ASSERT_PARAM(HasAsset(assetUID), "Asset '{}' failed to load!", p_localDataPath.CString());
    }
    return m_dataColony[m_loadedAssets[assetUID]]->GetHandle();
}

template <YK_AssetType AssetType>
YK_AssetHandle<AssetType> YK_AssetStorage<AssetType>::GetAsset(YK_UID p_uid) const
{
    if (!HasAsset(p_uid))
    {
        return YK_AssetHandle<AssetType>::EmptyHandle;
    }
    return m_dataColony[m_loadedAssets[p_uid]]->GetHandle();
}

template <YK_AssetType AssetType>
bool YK_AssetStorage<AssetType>::LoadAsset(YK_FilePath const& p_localDataPath)
{
    YK_UID assetUID{ p_localDataPath.CString() };
    if (HasAsset(assetUID))
    {
        YK_ASSERT(true, "Loading asset over existing data!");
        return false;
    }
    AssetType asset = m_loadAssetFunction(p_localDataPath);
    asset.m_uid = assetUID;
    typename ContainerType::Index index = m_dataColony.Add(std::move(asset));
    m_loadedAssets[assetUID] = index;
    YK_ASSERT(m_dataColony[index]->m_uid != YK_UID{}, "Asset did not keep UID after loading!");
    return true;
}

template <YK_AssetType AssetType>
bool YK_AssetStorage<AssetType>::InsertAsset(AssetType&& p_asset, YK_UID p_uid)
{
    if (HasAsset(p_uid))
    {
        YK_ASSERT(true, "Inserting asset over existing data!");
        return false;
    }
    p_asset.m_uid = p_uid;
    typename ContainerType::Index index = m_dataColony.Add(std::move(p_asset));
    m_loadedAssets[p_uid] = index;
    YK_ASSERT(m_dataColony[index]->m_uid != YK_UID{}, "Asset did not keep UID after loading!");
    return true;
}

template <YK_AssetType AssetType>
bool YK_AssetStorage<AssetType>::UnloadAsset(YK_UID p_uid)
{
    if (!HasAsset(p_uid))
    {
        return false;
    }
    m_dataColony.Remove(m_loadedAssets[p_uid]);
    m_loadedAssets.erase(p_uid);
    return true;
}

template <YK_AssetType AssetType>
void YK_AssetStorage<AssetType>::CleanupGarbage()
{
    // Should this be pre-alloc? On the stack? Capped per cycle?
    std::vector<YK_UID> assetsToRemove;

    YK_ColonyArrayIterator begin = m_dataColony.begin();
    YK_ColonyArrayIterator const end = m_dataColony.end();
    for (; begin != end; ++begin)
    {
        AssetType const& asset = *begin;
        if (asset.ReferenceCount() == 0)
        {
            assetsToRemove.push_back(asset.GetUID());
        }
    }

    for (YK_UID const& uidToRemove : assetsToRemove)
    {
        YK_ENSURE(UnloadAsset(uidToRemove), "Failed to unload asset for some reason!");
    }
}