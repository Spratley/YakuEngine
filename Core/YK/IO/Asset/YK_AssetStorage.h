#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Containers/YK_ColonyArray.h"
#include "YK/Utils/YK_MemoryUtils.h"

#include <concepts>
#include <unordered_map>

template <typename Loader, typename AssetType, typename... AssetLoadParameters>
concept YK_AssetLoaderType = requires(AssetType asset, AssetLoadParameters... parameters) {
    { Loader::Load(parameters...) } -> std::convertible_to<AssetType*>;
};

template <typename AssetType, YK_AssetLoaderType<AssetType, char const*> AssetLoader>
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

template <typename AssetType, YK_AssetLoaderType<AssetType, char const*> AssetLoader>
AssetType const& YK_AssetStorage<AssetType, AssetLoader>::GetAsset(char const* p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        bool loaded = LoadAsset(p_localDataPath);
        YK_LOG_ERROR(p_localDataPath);
        YK_ASSERT(loaded, "Asset not found!");
        YK_Unused(loaded); // Since YK_ASSERT gets stripped for retail
    }
    return *m_dataColony[m_loadedAssets[p_localDataPath]];
}

template <typename AssetType, YK_AssetLoaderType<AssetType, char const*> AssetLoader>
bool YK_AssetStorage<AssetType, AssetLoader>::LoadAsset(char const* p_localDataPath)
{
    // TODO: Check that file exists
    AssetType* loadedAsset = AssetLoader::Load(p_localDataPath);
    typename ContainerType::Index index = m_dataColony.Add(std::move(*loadedAsset));
    delete loadedAsset; // TODO: Don't do this!!!!
    m_loadedAssets[p_localDataPath] = index;
    return true;
}

template <typename AssetType, YK_AssetLoaderType<AssetType, char const*> AssetLoader>
void YK_AssetStorage<AssetType, AssetLoader>::UnloadAsset(char const* p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        return;
    }
    m_dataColony.Remove(m_loadedAssets[p_localDataPath]);
    m_loadedAssets.erase(p_localDataPath);
}