#pragma once

#include "YK/Libraries/Zen/Zen_Garden.h"

class YK_Core
{
public:
protected:
    template <typename ComponentTypes, typename SystemTypes>
    constexpr YK_Core(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : m_zenGarden(p_componentTypes, p_systemTypes)
    {
        m_zenGarden.Initialize(1024 * 512);
    }

protected:
    Zen::Garden m_zenGarden;
};