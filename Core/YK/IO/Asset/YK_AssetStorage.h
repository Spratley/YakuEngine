#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Types/Containers/YK_ColonyArray.h"

#include <concepts>
#include <unordered_map>

template <typename Loader, typename AssetType>
concept YK_AssetLoaderType = requires(YK_FilePath const& p_path) {
    { Loader::Load(p_path) } -> std::convertible_to<AssetType>;
};

template <typename... Types>
struct YK_TypeList
{};

// TODO: Add a generation stamp and asset handles,
// so that we don't get overlapping memory causing incorrect assets to be used
template <typename AssetType>
class YK_AssetStorage
{
public:
    YK_AssetStorage() = delete;

    template <YK_AssetLoaderType<AssetType> AssetLoader>
    YK_AssetStorage(YK_TypeList<AssetLoader> p_loaderType);

    // TODO: Replace with concepts for actual load function parameters
    // Also add data path resolution
    AssetType const& GetAsset(YK_FilePath const& p_localDataPath);
    bool LoadAsset(YK_FilePath const& p_localDataPath);
    void UnloadAsset(YK_FilePath const& p_localDataPath);

private:
    bool HasAsset(YK_FilePath const& p_localDataPath) { return m_loadedAssets.contains(p_localDataPath); }

private:
    using ContainerType = YK_ColonyArray<AssetType, 64>;
    std::unordered_map<YK_FilePath, typename ContainerType::Index> m_loadedAssets;
    ContainerType m_dataColony;

    using LoadAssetFunctionSignature = AssetType (*)(YK_FilePath const&);
    LoadAssetFunctionSignature m_loadAssetFunction;
};

template <typename AssetType>
template <YK_AssetLoaderType<AssetType> AssetLoader>
inline YK_AssetStorage<AssetType>::YK_AssetStorage(YK_TypeList<AssetLoader>)
    : m_loadedAssets()
    , m_dataColony()
    , m_loadAssetFunction(AssetLoader::Load)
{}

template <typename AssetType>
AssetType const& YK_AssetStorage<AssetType>::GetAsset(YK_FilePath const& p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        LoadAsset(p_localDataPath);
        YK_ASSERT_PARAM(HasAsset(p_localDataPath), "Asset '{}' not found!", p_localDataPath.CString());
    }
    return *m_dataColony[m_loadedAssets[p_localDataPath]];
}

template <typename AssetType>
bool YK_AssetStorage<AssetType>::LoadAsset(YK_FilePath const& p_localDataPath)
{
    // TODO: Check that file exists
    typename ContainerType::Index index = m_dataColony.Add(std::move(m_loadAssetFunction(p_localDataPath)));
    m_loadedAssets[p_localDataPath] = index;
    return true;
}

template <typename AssetType>
void YK_AssetStorage<AssetType>::UnloadAsset(YK_FilePath const& p_localDataPath)
{
    if (!HasAsset(p_localDataPath))
    {
        return;
    }
    m_dataColony.Remove(m_loadedAssets[p_localDataPath]);
    m_loadedAssets.erase(p_localDataPath);
}