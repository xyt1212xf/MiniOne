#pragma once
#include "Renderer.h"
namespace mini
{
	class CPostRenderer : public CRenderer
	{
		friend class CRendererPlugin;
	public:
		CPostRenderer(CDevice* pDevice);
		virtual ~CPostRenderer();

	private:
		virtual void addRenderNode(CRenderNode* pRenderNode) override;
		virtual void commitToGpu();
		virtual void addUnit(CUnit* pUnit);
		virtual void clearAllUnit();
	};
}
