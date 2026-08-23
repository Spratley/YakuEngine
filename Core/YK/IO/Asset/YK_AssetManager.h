#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Asset/YK_AssetStorage.h"
#include "YK/IO/File/YK_FilePath.h"

#include <typeindex>
#include <unordered_map>

class YK_AssetManager
{
public:
    template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
    void RegisterType();

    template <typename AssetType>
    YK_AssetStorage<AssetType>& GetAssetStorage();

    template <typename AssetType>
    AssetType const& GetAsset(YK_FilePath const& p_localDataPath);

private:
    // TODO: Come back to this, storing a void* is too C
    std::unordered_map<std::type_index, void*> m_assetStorages;
};

template <typename AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
void YK_AssetManager::RegisterType()
{
    YK_ASSERT(!m_assetStorages.contains(std::type_index(typeid(AssetType))), "Attempting to re-register an asset type!");
    m_assetStorages[typeid(AssetType)] = static_cast<void*>(new YK_AssetStorage<AssetType>(YK_TypeList<AssetLoader>{}));
}

template <typename AssetType>
YK_AssetStorage<AssetType>& YK_AssetManager::GetAssetStorage()
{
    YK_ASSERT(m_assetStorages.contains(std::type_index(typeid(AssetType))), "Attempting to get an asset type that's not registered!");
    return *static_cast<YK_AssetStorage<AssetType>*>(m_assetStorages[std::type_index(typeid(AssetType))]);
}

template <typename AssetType>
AssetType const& YK_AssetManager::GetAsset(YK_FilePath const& p_localDataPath)
{
    return GetAssetStorage<AssetType>().GetAsset(p_localDataPath);
}
