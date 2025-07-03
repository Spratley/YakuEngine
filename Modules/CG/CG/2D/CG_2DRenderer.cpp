#include "PCH/CG_PCH.h"
#include "CG_2DRenderer.h"

#include <algorithm> // TODO: Replace with custom implementation?

#include "CG/2D/Canvas/CG_Canvas.h"

CG_2DRenderer::CG_2DRenderer()
	: m_canvasesDirty(false)
	, m_canvases()
{}

void CG_2DRenderer::Render()
{
	if (m_canvasesDirty)
	{
		SortCanvases();
	}

	for (CG_Canvas const& canvas : m_canvases)
	{
		canvas.Render();
	}
}

void CG_2DRenderer::SortCanvases()
{
	// Arbitrary render order for canvases that share priorities
	std::sort(m_canvases.begin(), m_canvases.end(), [](CG_Canvas const& p_lhs, CG_Canvas const& p_rhs){
		return p_lhs.GetPriority() < p_rhs.GetPriority();
	});
}