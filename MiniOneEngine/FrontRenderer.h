#pragma once
#include "Renderer.h"
namespace mini
{
	class CFrontRenderer : public CRenderer
	{
		friend class CRendererPlugin;
	public:
		CFrontRenderer(CDevice* pDevice);
		virtual ~CFrontRenderer();
		virtual void addUnit(CUnit* pUnit);
		virtual void clearAllUnit();

	private:
		virtual void commitToGpu();
		virtual void addRenderNode(CRenderNode* pRenderNode) override;
		std::map<CShader*, std::list<CMeshUnit*> >	mRenderUnits;
	};
}
