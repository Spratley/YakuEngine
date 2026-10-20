#include "PCH/YakuCore_PCH.h"
#include "YK_AssetManager.h"

void YK_AssetManager::CleanupGarbage()
{
    for (CleanupFunctionSignature cleanupAssetTypeFunction : m_cleanupFunctions)
    {
        cleanupAssetTypeFunction(*this);
    }
}