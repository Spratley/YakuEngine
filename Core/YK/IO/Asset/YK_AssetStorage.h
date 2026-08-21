#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Types/Containers/YK_ColonyArray.h"
// #include "YK/Utils/YK_MemoryUtils.h"

#include <concepts>
#include <unordered_map>

template <typename Loader, typename AssetType>
concept YK_AssetLoaderType = requires(char const* p_path) {
    { Loader::Load(p_path) } -> std::convertible_to<AssetType>;
};

// TODO: Add a generation stamp and asset handles,
// so that we don't get overlapping memory causing incorrect assets to be used
template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
class YK_AssetStorage
{
public:
    // TODO: Replace with concepts for actual load function parameters
    // Also add data path resolution
    AssetType const& GetAsset(char const* p_localDataPath);
    bool LoadAsset(char const* p_localDataPath);
    void UnloadAsset(char const* p_localDataPath);

private:
    bool HasAsset(char const* p_localDataPath) { return m_loadedAssets.contains(p_localDataPath); }

private:
    using ContainerType = YK_ColonyArray<AssetType, 64>;
    std::unordered_map<char const*, typename ContainerType::Index> m_loadedAssets;
    ContainerType m_dataColony;
};

template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
AssetType const& YK_AssetStorage<AssetType, AssetLoader>::GetAsset(char const* p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        LoadAsset(p_localDataPath);
        YK_ASSERT_PARAM(HasAsset(p_localDataPath), "Asset '{}' not found!", p_localDataPath);
    }
    return *m_dataColony[m_loadedAssets[p_localDataPath]];
}

template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
bool YK_AssetStorage<AssetType, AssetLoader>::LoadAsset(char const* p_localDataPath)
{
    // TODO: Check that file exists
    typename ContainerType::Index index = m_dataColony.Add(std::move(AssetLoader::Load(p_localDataPath)));
    m_loadedAssets[p_localDataPath] = index;
    return true;
}

template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
void YK_AssetStorage<AssetType, AssetLoader>::UnloadAsset(char const* p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        return;
    }
    m_dataColony.Remove(m_loadedAssets[p_localDataPath]);
    m_loadedAssets.erase(p_localDataPath);
}