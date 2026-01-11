#pragma once

// TODO: Evaluate thread safety! This probably needs some RW locks when loading data

template <typename ResourceType>
struct YKC_ResourceTraits;

template <typename ResourceType>
struct YKC_ResourceContainer
{
    using ResourceLoader = typename YKC_ResourceTraits<ResourceType>::Loader;

public:
    template <typename... Parameters>
    YKC_HandleBase<ResourceType, YK_HANDLE_PAGE_SIZE> Load(Parameters&&... p_parameters)
    {
        return m_loadedResources.EmplaceHandled(ResourceLoader::Load(std::forward<Parameters>(p_parameters)...));
    }

private:
    YKC_HandledStorage<ResourceType, YK_HANDLE_PAGE_SIZE> m_loadedResources;
};

template <typename... ResourceContainers>
class YKC_ResourceManager
{
public:
    template <typename ResourceType>
    YKC_ResourceContainer<ResourceType>& GetResourceContainer();

    template <typename ResourceType, typename... Parameters>
    YKC_HandleBase<ResourceType, YK_HANDLE_PAGE_SIZE> Load(Parameters&&... p_parameters)
    {
        return GetResourceContainer<ResourceType>().Load(std::forward<Parameters>(p_parameters)...);
    }

private:
    std::tuple<ResourceContainers...> m_resourceContainers;
};

template <typename... ResourceContainers>
template <typename ResourceType>
inline YKC_ResourceContainer<ResourceType>& YKC_ResourceManager<ResourceContainers...>::GetResourceContainer()
{
    return std::get<YKC_ResourceContainer<ResourceType>>(m_resourceContainers);
}
