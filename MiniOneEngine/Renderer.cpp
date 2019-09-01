#include "pch.h"
#include "Renderer.h"
#include "Camera.h"
namespace mini
{
	CRenderer::CRenderer(CDevice* pDevice)
	{
		mOrder = 0XFFFFFFFF;
		mType = RendererType::UnknowType;
		mpCamera = nullptr;
		mpDevice = pDevice;
	}

	CRenderer::~CRenderer()
	{
		mpDevice = nullptr;
		SafeRelease(mpCamera);
	}

	void CRenderer::render()
	{
		if (mpCamera != nullptr)
		{
			mpCamera->updateFrame();
			for (auto item : mRenderNodes)
			{
				auto pActor = item->getActor();
				if (nullptr != pActor && mpCamera->isInFrustum(pActor))
				{
					auto& units = pActor->getUnits();
					for (auto child : units)
					{
						addUnit(child);
					}
				}
			}
			commitToGpu();
		}
	}

	void CRenderer::commitToGpu()
	{

	}

	void CRenderer::removeRenderNode(CRenderNode* pRenderNode)
	{
		auto item = std::find(mRenderNodes.begin(), mRenderNodes.end(), pRenderNode);
		if (item != mRenderNodes.end())
		{
			(*item)->Release();
			mRenderNodes.erase(item);
		}
	}

	void CRenderer::removeAllRenderNode()
	{
		for (auto item : mRenderNodes)
		{
			item->Release();
		}
		mRenderNodes.clear();
	}

	void CRenderer::addUnit(CUnit* pUnit)
	{

	}

	void CRenderer::clearAllUnit()
	{

	}

	UINT32 CRenderer::getRendererOrder() const
	{
		return mOrder;
	}

	RendererType CRenderer::getRendererType() const
	{
		return mType;
	}

	void CRenderer::setCamera(CCamera* pCamera)
	{
		if (pCamera->isApplyRenderer(mType) && pCamera != mpCamera)
		{
			mpCamera = pCamera;
			mpCamera->AddRef();
		}
	}

	void CRenderer::setDevice(CDevice* pDevice)
	{
		mpDevice = pDevice;
	}

	void CRenderer::setActive(bool bActive)
	{
		mbActive = bActive;
	}

	bool CRenderer::isActive()
	{
		return mbActive;
	}

	bool CRenderer::isEditor()
	{
		return mbEditor;
	}

	void CRenderer::setEditor(bool bEditor)
	{
		mbEditor = bEditor;
	}

	void CRenderer::addRenderNode(CRenderNode* pActor)
	{

	}
}
