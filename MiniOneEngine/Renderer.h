#pragma once
#include "EngineHead.h"
#include "UnitDerive.h"
#include "Device.h"
#include "RenderNode.h"

namespace mini
{
#define RenderType1 (RendererType::DeferredRenderer | RendererType::FrontRenderer | RendererType::ShadowRenderer | RendererType::PostRenderer)

	class CCamera;
	class CRenderer
	{
		friend class CRendererPlugin;
	public:
		explicit CRenderer(CDevice* pDevice);
		virtual ~CRenderer();
		UINT32 getRendererOrder() const;
		RendererType getRendererType() const;
		void setCamera(CCamera* pCamera);
		void setDevice(CDevice* pDevice);
		void setActive(bool bActive);
		void setEditor(bool bEditor);
		bool isActive();
		bool isEditor();


	protected:
		virtual void addRenderNode(CRenderNode* pActor);
		virtual void addUnit(CUnit* pUnit);
		virtual void clearAllUnit();
		virtual void render();
		virtual void commitToGpu();
		void removeRenderNode(CRenderNode* pActor);
		void removeAllRenderNode();

	protected:
		UINT32				mOrder;
		RendererType		mType;
		CCamera*			mpCamera;
		CDevice*			mpDevice;
		std::set<CRenderNode*>	mRenderNodes;
		bool				mbActive = true;
		bool				mbEditor = false;
	};
}
