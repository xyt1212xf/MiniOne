#include "pch.h"
#include "RenderNode.h"
#include "Actor.h"
namespace mini
{
	CRenderNode::CRenderNode()
	{

	}

	CRenderNode::~CRenderNode()
	{

	}

	void CRenderNode::bindActor(CActor* pActor, UINT32 rendererType)
	{
		if (mpActor != pActor)
		{
			SafeRelease(mpActor);
			mpActor = pActor;
			mpActor->AddRef();
		}
		mRendererFlag = rendererType;
	}


	void CRenderNode::setRenderer(UINT32 rendererType)
	{
		mRendererFlag = rendererType;
	}

	UINT32 CRenderNode::getRendererFlag() const
	{
		return mRendererFlag;
	}

	CActor* CRenderNode::getActor() const
	{
		return mpActor;
	}

}

