#pragma once

class CG_Canvas;

class CG_2DRenderer
{
public:
	CG_2DRenderer();

	void Render();

private:
	void SortCanvases();

private:
	bool m_canvasesDirty;
	std::vector<CG_Canvas> m_canvases;
};