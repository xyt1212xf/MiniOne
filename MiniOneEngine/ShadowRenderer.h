#pragma once
#include "Renderer.h"
namespace mini
{
	class CShadowRenderer : public CRenderer
	{
		friend class CRendererPlugin;
	public:
		CShadowRenderer(CDevice* pDevice);
		virtual ~CShadowRenderer();
		virtual void clearAllUnit();

	private:
		virtual void addRenderNode(CRenderNode* pRenderNode)override;
		virtual void commitToGpu();
	};
}
