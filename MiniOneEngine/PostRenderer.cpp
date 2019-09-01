#include "pch.h"
#include "PostRenderer.h"

namespace mini
{
	CPostRenderer::CPostRenderer(CDevice* pDevice)
		:CRenderer(pDevice)
	{
	}


	CPostRenderer::~CPostRenderer()
	{
	}

	void CPostRenderer::addRenderNode(CRenderNode* pRenderNode)
	{
		mRenderNodes.insert(pRenderNode);
	}

	void CPostRenderer::commitToGpu()
	{

	}

	void CPostRenderer::addUnit(CUnit* pUnit)
	{
	}

	void CPostRenderer::clearAllUnit()
	{

	}
}
