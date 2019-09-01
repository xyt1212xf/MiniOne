#include "pch.h"
#include "FrontRenderer.h"

namespace mini
{
	CFrontRenderer::CFrontRenderer(CDevice* pDevice)
		:CRenderer(pDevice)
	{
		mOrder = 235;
		mType = RendererType::FrontRenderer;
	}


	CFrontRenderer::~CFrontRenderer()
	{
	}

	void CFrontRenderer::addUnit(CUnit* pUnit)
	{
		CMeshUnit* pMeshUnit = dynamic_cast<CMeshUnit*>(pUnit);
		if (pMeshUnit)
		{
			CMaterialUnit* pMaterilaUnit = pMeshUnit->getMaterialUnit();
			if (pMaterilaUnit != nullptr)
			{
				CShader* pShader = pMaterilaUnit->getShader();
				if (pShader != nullptr)
				{
					mRenderUnits[pShader].push_back(pMeshUnit);
				}
			}
		}
	}

	void CFrontRenderer::clearAllUnit()
	{
		mRenderUnits.clear();
	}

	void CFrontRenderer::commitToGpu()
	{
		for (auto& item : mRenderUnits)
		{
			(item.first)->commitToGpu();
			for (auto& child : (item.second))
			{
				child->render(mpDevice);
			}
		}
		clearAllUnit();
	}

	void CFrontRenderer::addRenderNode(CRenderNode* pRenderNode)
	{
		if (pRenderNode->getRendererFlag() & RendererType::FrontRenderer)
		{
			mRenderNodes.insert(pRenderNode);
		}
	}
}
