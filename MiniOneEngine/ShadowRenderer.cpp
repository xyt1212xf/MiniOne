#include "pch.h"
#include "ShadowRenderer.h"

namespace mini
{
	CShadowRenderer::CShadowRenderer(CDevice* pDevice)
		: CRenderer(pDevice)
	{
		mOrder = 245;
		mType = RendererType::ShadowRenderer;
	}


	CShadowRenderer::~CShadowRenderer()
	{
	}

	void CShadowRenderer::clearAllUnit()
	{

	}

	void CShadowRenderer::addRenderNode(CRenderNode* pRenderNode)
	{
		mRenderNodes.insert(pRenderNode);
	}

	void CShadowRenderer::commitToGpu()
	{

	}

}
