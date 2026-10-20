#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Asset/YK_AssetConcepts.h"
#include "YK/IO/Asset/YK_AssetHandle.h"
#include "YK/IO/Asset/YK_AssetStorage.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Types/Identifier/YK_UID.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

#include <typeindex>
#include <unordered_map>
#include <vector>

class YK_AssetManager
{
public:
    template <YK_AssetType AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
    void RegisterType();

    template <YK_AssetType AssetType, typename Self>
    typename YK_DeducedReturn<YK_AssetStorage<AssetType>, Self&&>::Type& GetAssetStorage(this Self&& p_self);

    template <YK_AssetType AssetType>
    YK_AssetHandle<AssetType> GetAsset(YK_FilePath const& p_localDataPath);

    template <YK_AssetType AssetType>
    YK_AssetHandle<AssetType> GetAsset(YK_UID p_uid) const;

    void CleanupGarbage();

private:
    template <YK_AssetType AssetType>
    static void CleanupAssetTypeGarbage(YK_AssetManager& p_self);

private:
    // TODO: Come back to this, storing a void* is too C
    std::unordered_map<std::type_index, void*> m_assetStorages;

    using CleanupFunctionSignature = void (*)(YK_AssetManager&);
    std::vector<CleanupFunctionSignature> m_cleanupFunctions;
};

template <YK_AssetType AssetType, YK_AssetLoaderType<AssetType> AssetLoader>
void YK_AssetManager::RegisterType()
{
    YK_ASSERT(!m_assetStorages.contains(std::type_index(typeid(AssetType))),
              "Attempting to re-register an asset type!");
    m_assetStorages[typeid(AssetType)] = static_cast<void*>(new YK_AssetStorage<AssetType>(YK_TypeList<AssetLoader>{}));
    m_cleanupFunctions.push_back(&CleanupAssetTypeGarbage<AssetType>);
}

template <YK_AssetType AssetType, typename Self>
typename YK_DeducedReturn<YK_AssetStorage<AssetType>, Self&&>::Type& YK_AssetManager::GetAssetStorage(
  this Self&& p_self)
{
    YK_ASSERT(p_self.m_assetStorages.contains(std::type_index(typeid(AssetType))),
              "Attempting to get an asset type that's not registered!");
    return *static_cast<YK_AssetStorage<AssetType>*>(p_self.m_assetStorages[std::type_index(typeid(AssetType))]);
}

template <YK_AssetType AssetType>
YK_AssetHandle<AssetType> YK_AssetManager::GetAsset(YK_FilePath const& p_localDataPath)
{ return GetAssetStorage<AssetType>().GetAsset(p_localDataPath); }

template <YK_AssetType AssetType>
YK_AssetHandle<AssetType> YK_AssetManager::GetAsset(YK_UID p_uid) const
{ return GetAssetStorage<AssetType>().GetAsset(p_uid); }

template <YK_AssetType AssetType>
void YK_AssetManager::CleanupAssetTypeGarbage(YK_AssetManager& p_self)
{
    YK_AssetStorage<AssetType>& assetStorage = p_self.GetAssetStorage<AssetType>();
    assetStorage.CleanupGarbage();
}