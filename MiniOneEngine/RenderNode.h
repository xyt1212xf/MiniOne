#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"


namespace mini
{
	class CActor;
	class CRenderNode : public CRefcount 
	{
	public:
		CRenderNode();
		virtual ~CRenderNode();
		void bindActor(CActor* pActor, UINT32 rendererType);
		void setRenderer(UINT32 rendererType);
		UINT32 getRendererFlag() const;
		CActor* getActor() const;

	private:
		CActor*	mpActor = nullptr;
		UINT32	mRendererFlag;
	};
}