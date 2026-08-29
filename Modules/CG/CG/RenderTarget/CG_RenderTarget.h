#pragma once

#include "CG/RenderTarget/CG_RenderTargetBase.h"
#include "CG/RenderTarget/OpenGL/CG_RenderTargetData_OpenGL.h"

// TODO: Add preprocessor to choose graphics API when I eventually add Vulcan
class CG_RenderTarget : public CG_RenderTargetBase<CG_RenderTargetData_OpenGL>
{};