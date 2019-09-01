#include "pch.h"
#include "RendererPlugin.h"
#include "Engine.h"
#include "UnitDerive.h"
#include "RendererFactory.h"
#define  WVPMatrix 3 
namespace mini
{
	void renderer(void *lp)
	{
		CRendererPlugin* pRenderer = (CRendererPlugin*)lp;
		while (pRenderer->isRun())
		{
			pRenderer->internalRenderer();
		}
		pRenderer->mbOverRenderer = true;
	}


	CRendererPlugin::CRendererPlugin(CScriptPlugin* pPlugin)
		: mpPlugin(pPlugin)
		, mbOverRenderer(false)
		, mpScene(nullptr)
		, mpWorldMatrix(nullptr)
		, mpViewMatrix(nullptr)
	{
		D3DXMatrixIdentity(&mIdentityMatrix);
	}


	CRendererPlugin::~CRendererPlugin()
	{
		for (auto item : mRenderNodes)
		{
			(item)->Release();
		}
		mRenderNodes.clear();
	}

	bool CRendererPlugin::onStart()
	{
		resume();
		mbRender = true;
		unsigned threadID = 0;
		CRendererFactory::initialize();
		addRenderer(RendererType::FrontRenderer);
		/*
			这个渲染器移动到了具体的Plugin里面去了，由这些Plugin来产生，这样才是合理的方式，其他的渲染器也需要修改
			addRenderer(RendererType::GuiRenderer);
		*/
		addRenderer(RendererType::ShadowRenderer);
		addRenderer(RendererType::PostRenderer);
		mCurrentSceneRenderType = RendererType::FrontRenderer;
		std::thread t(renderer, this);
		t.detach();
		return true;
	}

	void CRendererPlugin::onRun()
	{

	}

	void CRendererPlugin::onShutDown()
	{
		stopRender();
		stop();
		mbRender = false;
		recoverRender();
		while (!mbOverRenderer)
		{
			Sleep(10);
		}
		for (auto item : mRenderers)
		{
			SafeDelete((item.second));
		}
		CRendererFactory::destory();
		SafeDelete(mpDevice);
	}

	CRenderer* CRendererPlugin::addRenderer(RendererType type)
	{
		if (isExistRenderer(type))
		{
			return 	mRenderers[type];
		}

		CRenderer* pRenderer = CRendererFactory::getSinglePtr()->createRenderer(type);
		if (pRenderer != nullptr)
		{
			UINT32 order = pRenderer->getRendererType();
			mRenderers[order] = pRenderer;
		}
		return pRenderer;
	}

	void CRendererPlugin::removeRenderer(RendererType type)
	{
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() == type)
			{
				SafeDelete((item.second));
			}
		}
	}

	bool CRendererPlugin::isExistRenderer(RendererType type)
	{
		auto item = mRenderers.find(type);
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() == type)
			{
				return true;
			}
		}
		return false;
	}

	void CRendererPlugin::flushToGpu()
	{
		if (GEngine->getStatus() == Running)
		{
			mpScene->updateScene();
		}
		for (auto item : mRenderers)
		{
			if ((item.second)->isActive())
			{
				(item.second)->render();
			}
		}
	}

	CDevice* CRendererPlugin::getDevice()
	{
		return mpDevice;
	}

	CRenderNode * CRendererPlugin::createRender()
	{
		return new CRenderNode();
	}

	void CRendererPlugin::setScene(CScene* pScene)
	{
		mpScene = pScene;
	}


	void CRendererPlugin::setWorldMatrix(CMatrix* pMatrix)
	{
		mpWorldMatrix = pMatrix;
		//mpWorldMatrix->_11 = 0.1;
		//mpWorldMatrix->_22 = 0.1;
		//mpWorldMatrix->_33 = 0.1;
		mbuildGpuMatrxis[0] = pMatrix;
		mbUpdateTranspose = true;
	}


	void CRendererPlugin::setViewMatrix(CMatrix* pMatrix)
	{
		mpViewMatrix = pMatrix;
		mbuildGpuMatrxis[1] = pMatrix;
		mbUpdateTranspose = true;
	}

	void CRendererPlugin::setProjectMatrix(CMatrix* pMatrix)
	{
		mpProjectMatrix = pMatrix;
		mbuildGpuMatrxis[2] = pMatrix;
		mbUpdateTranspose = true;
	}



	void CRendererPlugin::setCamera(CCamera* pCamera)
	{
		for (auto item : mRenderers)
		{
			(item.second)->setCamera(pCamera);
		}
	}

	void CRendererPlugin::addRenderNode(CRenderNode* pRenderNode)
	{
		if (nullptr != pRenderNode)
		{
			mRenderNodes.insert(pRenderNode);
			pRenderNode->AddRef();
			for (auto item : mRenderers)
			{
				(item.second)->addRenderNode(pRenderNode);
			}
		}
	}

	void CRendererPlugin::removeRenderNode(CRenderNode* pRenderNode)
	{
		if (nullptr != pRenderNode)
		{
			auto item = std::find(mRenderNodes.begin(), mRenderNodes.end(), pRenderNode);
			if (item != mRenderNodes.end())
			{
				(*item)->Release();
				mRenderNodes.erase(item);
			}
			for (auto item : mRenderers)
			{
				(item.second)->removeRenderNode(pRenderNode);
			}
		}
	}

	void CRendererPlugin::activeRenderer(UINT32 uflag)
	{
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() &uflag)
			{
				(item.second)->setActive(true);
			}
		}
	}

	void CRendererPlugin::disableRenderer(UINT32 uflag)
	{
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() &uflag)
			{
				(item.second)->setActive(false);
			}
		}
	}

	void CRendererPlugin::activeRendererEditor(UINT32 uflag)
	{
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() &uflag)
			{
				(item.second)->setEditor(true);
			}
		}
	}

	void CRendererPlugin::disableRendererEditor(UINT32 uflag)
	{
		for (auto item : mRenderers)
		{
			if ((item.second)->getRendererType() &uflag)
			{
				(item.second)->setEditor(false);
			}
		}
	}

	const CMatrix* const CRendererPlugin::getWorldMatrix()
	{
		return mpWorldMatrix;
	}

	const CMatrix* const CRendererPlugin::getViewMatrix()
	{
		return mpViewMatrix;
	}

	void CRendererPlugin::stopRender()
	{
		if (0 == mLockCount)
		{
			++mLockCount;
			mMutex.lock();
		}
		else
		{
			++mLockCount;
		}
	}

	void CRendererPlugin::recoverRender()
	{
		if (--mLockCount == 0)
		{
			mMutex.unlock();
		}
	}

	std::vector<CMatrix>& CRendererPlugin::getTransposeMatrices()
	{
		if (mbUpdateTranspose)
		{
			mbUpdateTranspose = false;
			for (size_t i = 0; i < mbuildGpuMatrxis.size(); ++i)
			{
				D3DXMatrixTranspose(&mbindMatrixsTranspose[i], mbuildGpuMatrxis[i]);
			}
		}
		return mbindMatrixsTranspose;
	}

	bool CRendererPlugin::initialize()
	{
		mpDevice = new CDevice();

		auto& configInfo = mpPlugin->getGraphicDeviceConfig();

		DeviceConfig& config = mpDevice->getConfig();
		config.vsync = configInfo["vsync"] == 1 ? true : false;
		config.fullScreen = configInfo["fullScreen"] == 1 ? true : false;
		config.screenWidth = configInfo["deviceWidth"];
		config.screenHeight = configInfo["deviceHeight"];
		config.cameraNear = configInfo["cameraNear"];
		config.cameraFar = configInfo["cameraFar"];
		config.fps = configInfo["fps"];
		config.hWnd = GEngine->getHWND();
		mpDevice->initDevice(config);
		// Setup the projection matrix.
		float fieldOfView = (float)D3DX_PI / 4.0f;
		float screenAspect = (float)config.screenWidth / (float)config.screenHeight;

		mbuildGpuMatrxis.resize(WVPMatrix);
		mbindMatrixsTranspose.resize(WVPMatrix);
		mbuildGpuMatrxis[0] = &mIdentityMatrix;
		mbuildGpuMatrxis[1] = &mIdentityMatrix;
		mbuildGpuMatrxis[2] = &mIdentityMatrix;
		mbUpdateTranspose = true;
		CResourceManager::getSinglePtr()->createResource("resource/font/styleFont", Font);
		return true;
	}

	void CRendererPlugin::internalRenderer()
	{
		mMutex.lock();
		if (mbRender && mpScene != nullptr)
		{
			mpDevice->beginScene();
			if (mActive)
			{
				flushToGpu();
			}
			mpDevice->endinScene();
		}
		mMutex.unlock();
		Sleep(5);
	}
}
