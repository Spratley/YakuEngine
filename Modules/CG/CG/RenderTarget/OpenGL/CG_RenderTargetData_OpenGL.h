#pragma once

#include "YK/Types/Math/YK_Integer.h"

struct CG_RenderTargetData_OpenGL
{
    YK_U32 m_frameBufferID = 0;

    // Should these also be replaced with GL texture objects?
    // Also this should be more generic, either template parameters or dynamic with AddColorTarget etc
    YK_U32 m_colorBufferID = 0;
    YK_U32 m_depthBufferID = 0;
};