#pragma once

#include "YK/IO/Asset/YK_Asset.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Other/YK_BitField.h"

struct AM_BoneMask
    : public YK_BitField<YK_SizeT>
    , public YK_Asset
{};