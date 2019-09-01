#include "pch.h"
#include "DeferredRenderer.h"

namespace mini
{
	CDeferredRenderer::CDeferredRenderer(CDevice* pDevice)
		:CRenderer(pDevice)
	{
		mOrder = 235;
		mType = RendererType::DeferredRenderer;
	}


	CDeferredRenderer::~CDeferredRenderer()
	{
	}

	void CDeferredRenderer::clearAllUnit()
	{

	}

	void CDeferredRenderer::commitToGpu()
	{

	}

	void CDeferredRenderer::addRenderNode(CRenderNode* pRenderNode)
	{
		mRenderNodes.insert(pRenderNode);
	}
}
