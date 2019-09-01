#pragma once
#include "Renderer.h"
namespace mini
{
	class CDeferredRenderer : public CRenderer
	{
		friend class CRendererPlugin;
	public:
		CDeferredRenderer(CDevice* pDevice);
		virtual ~CDeferredRenderer();
		virtual void clearAllUnit();

	private:
		virtual void commitToGpu();
		virtual void addRenderNode(CRenderNode* pRenderNode) override;
	};
}
