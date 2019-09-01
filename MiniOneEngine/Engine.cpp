#include "pch.h"
#include "Engine.h"
#include "ScriptPlugin.h"
#include "RendererPlugin.h"
#include "NetworkPlugin.h"
#include "GuiPlugin.h"
#include "Scene.h"
#include "Actor.h"
#include "Device.h"
#include "Camera.h"
#include "Terrain.h"
#include "SkyDome.h"

#ifdef EDITOR
#include "TriangleActor.h"
#include "TextureActor.h"
#include "DiffuseActor.h"
#include "CubeActor.h"
#include "GuiActor.h"
#endif
namespace mini
{
	CEngine *GEngine = nullptr;

	CEngine::CEngine()
		: mpPluginFactory(nullptr)
		, mpPickEntity(nullptr)
	{
		GEngine = this;
		mpActiveScene = nullptr;
		m_status = Editoring;
	}

	bool CEngine::init(HWND hWnd)
	{
		mhWnd = hWnd;
		mResourceManager = CResourceManager::initialize();
		initPlugin();
		mpDevice = CRendererPlugin::getSinglePtr()->getDevice();
		return true;
	}

	bool CEngine::onStart(const char* pStrSceneName)
	{
		bool bResuit = true;
		for (auto& item : mpPluginList)
		{
			bResuit = bResuit && (item.second)->onStart();
		}
		CLuaBind::doFile("script/main.lua", "scriptMain");
		if (nullptr == pStrSceneName)
		{
			createScene("defaultScene");
		}
		else
		{
			createScene(pStrSceneName);
		}
		return bResuit;
	}

	void CEngine::onRun()
	{
		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				for (auto& item : mpPluginList)
				{
					(item.second)->onRun();
				}
			}
		}
		onShutDown();
	}

	void CEngine::onShutDown()
	{
		for (auto& item : mpPluginList)
		{
			(item.second)->onShutDown();
			SafeDelete(item.second);
		}
		CResourceManager::destory();
		CPluginFactory::destory();
		mpPluginList.clear();
		mpPluginFactory = nullptr;
		mResourceManager = nullptr;
		SafeDelete(mpActiveScene);

	}

	void CEngine::addPlugin(PluginType type)
	{
		if (nullptr == getPlugin<CPlugin>(type))
		{
			CPlugin* pPlugIn = mpPluginFactory->createPlugin(type);
			mpPluginList[type] = pPlugIn;
		}
	}

	void CEngine::removePlugin(PluginType type)
	{
		auto item = mpPluginList.find(type);
		if (item != mpPluginList.end())
		{
			mpPluginList.erase(item);
		}
	}

	HWND CEngine::getHWND()
	{
		return mhWnd;
	}


	CScene* CEngine::createScene(const std::string& strSceneName)
	{
		if (mpActiveScene == nullptr)
		{
			mpActiveScene = new CScene();
		}
		else
		{
			SafeDelete(mpActiveScene);
			mpActiveScene = new CScene();
		}
		mpActiveScene->initGroundPlane();
		mpActiveScene->setSceneName(strSceneName);

		CCamera* pCamera = dynamic_cast<CCamera*>(GEngine->createActor(EntityType::Camera));
		pCamera->createTo3DProjectMatrix();
		pCamera->applyToRenderer(RenderType1);
		CRendererPlugin::getSinglePtr()->setCamera(pCamera);
		mpActiveScene->setCamera(pCamera);
		SafeRelease(pCamera);
		return mpActiveScene;
	}


	CActor* CEngine::createActor(const std::string& fileName, EntityType type)
	{
		return createActor(fileName, "", type);
	}

	mini::CActor* CEngine::createActor(const std::string& fileName, const std::string& materialName, EntityType type)
	{
		CActor* pActor = createActor(type);
		if (nullptr != pActor && pActor->initlalise(fileName, materialName))
		{
			return pActor;
		}
		else
		{
			return nullptr;
		}
	}


	CActor* CEngine::createActor(EntityType type)
	{
		CActor* pEntity = nullptr;
		if (type == Actor)
		{
			pEntity = new CActor();
		}
		else if (type == Camera)
		{
			pEntity = new CCamera;
		}
		else if (type == Gui)
		{
			pEntity = new CGuiActor;
		}
		else if (type == Terrain)
		{
			pEntity = new CTerrain();
		}
		else if (type == SkyDome)
		{
			pEntity = new CSkyDome();
		}
#ifdef EDITOR
		else if (type == Triangle)
		{
			pEntity = new CTriangleActor;
		}
		else if (type == TriangleTexture)
		{
			pEntity = new CTextureActor;
		}
		else if (type == Diffuse)
		{
			pEntity = new CDiffuseActor;
		}
		else if (type == CubeMesh)
		{
			pEntity = new CCubeActor;
		}
#endif
		/*
		if (pEntity != nullptr)
		{
			pEntity->initlalise("");
		}
		*/
		return pEntity;
	}



	CScene* CEngine::getCurrentScene()
	{
		return mpActiveScene;
	}

	UINT32 CEngine::getDeviceWidth()
	{
		return mpDevice->getConfig().screenWidth;
	}

	UINT32 CEngine::getDeviceHeight()
	{
		return mpDevice->getConfig().screenHeight;
	}

	void CEngine::setStatus(EngineStatus status)
	{
		m_status = status;
	}

	EngineStatus CEngine::getStatus()
	{
		return m_status;
	}

	bool CEngine::ProcessMsg(event& msg)
	{
		switch (msg.Messtype)
		{
		case WM_LBUTTONDOWN:
			msg.Messtype = LEFTDOWN;
			break;
		case WM_LBUTTONUP:
			msg.Messtype = LEFTUP;
			break;
		case WM_RBUTTONDOWN:
			msg.Messtype = RIGHTDOWN;
			break;
		case WM_RBUTTONUP:
			msg.Messtype = RIGHTUP;
			break;
		case WM_MOUSEMOVE:
			msg.Messtype = MOUSEMOVE;
			break;
		case WM_MOUSEWHEEL:
			msg.Messtype = MOUSEWHEEL;
			break;
		case WM_LBUTTONDBLCLK:
			msg.Messtype = LEFTDBLCLK;
			break;
		case WM_RBUTTONDBLCLK:
			msg.Messtype = RIGHTDBLCLK;
			break;
		case WM_CHAR:
			msg.Messtype = WMCHAR;
			break;
		case WM_KEYDOWN:
			msg.Messtype = WMKEYDOWN;
			break;
		case WM_KEYUP:
			msg.Messtype = WMKEYUP;
			break;
		case WM_MBUTTONDOWN:
			msg.Messtype = MBUTTONDOWN;
			break;
		case WM_MBUTTONUP:
			msg.Messtype = MBUTTONUP;
			break;
		case WM_SYSKEYDOWN:
			msg.Messtype = WMKEYDOWN;
			break;
		case WM_SYSKEYUP:
			msg.Messtype = WMKEYDOWN + 1;
			break;
		}
		for (auto i : mpPluginList)
		{
			(i.second)->Process(msg);
		}
		if (!msg.bProcess)
		{
			mpActiveScene->processMsg(msg);	
		}
		return  true;
	}

	CEntity* CEngine::getPickEntity()
	{
		return mpPickEntity;
	}

	bool CEngine::createTerrain(std::string strHeightMap)
	{
		SafeRelease(mpTerrain);
		mpTerrain = createActor(Terrain);
		if (mpTerrain != nullptr)
		{
			CTerrain* pTerrain = dynamic_cast<CTerrain*>(mpTerrain);;
			return pTerrain->init(strHeightMap);
		}
		return true;
	}

	bool CEngine::createTerrain(INT32 width, INT32 height)
	{
		SafeRelease(mpTerrain);
		mpTerrain = createActor(Terrain);
		if (mpTerrain != nullptr)
		{
			CTerrain* pTerrain = dynamic_cast<CTerrain*>(mpTerrain);
			return pTerrain->init(width, height);
		}
		return false;
	}

	CEntity* CEngine::getTerrainActor()
	{
		return mpTerrain;
	}

	bool CEngine::IntersectTriangle(const float* orig, const float* dir, float* v0, float* v1, float* v2, float* t, float* u, float* v)
	{
		// Find vectors for two edges sharing vert0
		D3DXVECTOR3 edge1 = D3DXVECTOR3(v1) - D3DXVECTOR3(v0);
		D3DXVECTOR3 edge2 = D3DXVECTOR3(v2) - D3DXVECTOR3(v0);

		// Begin calculating determinant - also used to calculate U parameter
		D3DXVECTOR3 pvec;
		D3DXVec3Cross(&pvec, &D3DXVECTOR3(dir), &D3DXVECTOR3(edge2));

		// If determinant is near zero, ray lies in plane of triangle
		FLOAT det = D3DXVec3Dot(&edge1, &pvec);

		D3DXVECTOR3 tvec;
		if (det > 0)
		{
			tvec = D3DXVECTOR3(orig) - D3DXVECTOR3(v0);
		}
		else
		{
			tvec = D3DXVECTOR3(v0) - D3DXVECTOR3(orig);
			det = -det;
		}

		if (det < 0.0001f)
		{
			return false;
		}
		// Calculate U parameter and test bounds
		*u = D3DXVec3Dot(&tvec, &pvec);
		if (*u < 0.0f || *u > det)
		{
			return false;
		}

		// Prepare to test V parameter
		D3DXVECTOR3 qvec;
		D3DXVec3Cross(&qvec, &tvec, &edge1);

		// Calculate V parameter and test bounds
		*v = D3DXVec3Dot(&D3DXVECTOR3(dir), &D3DXVECTOR3(qvec));
		if (*v < 0.0f || *u + *v > det)
		{
			return false;
		}
		// Calculate t, scale parameters, ray intersects triangle
		*t = D3DXVec3Dot(&edge2, &qvec);
		float fInvDet = 1.0f / det;
		*t *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return true;
	}

	void CEngine::addActorToScene(CEntity* pEntity)
	{
		if (mpActiveScene != nullptr)
		{
			mpActiveScene->addActor(dynamic_cast<CActor*>(pEntity));
		}
	}

	void CEngine::disableRendererPlugin()
	{
		CRendererPlugin::getSinglePtr()->active(false);
	}

	void CEngine::enableRendererPlugin()
	{
		CRendererPlugin::getSinglePtr()->active(true);
	}

	void CEngine::stopRender()
	{
		CRendererPlugin::getSinglePtr()->stopRender();
	}

	void CEngine::recoverRender()
	{
		CRendererPlugin::getSinglePtr()->recoverRender();
	}

	CDevice* CEngine::getDevice()
	{
		return mpDevice;
	}

	bool CEngine::createRenderWindow(HWND hWnd)
	{
		return mpDevice->createSwapChain(hWnd);
	}

	void CEngine::changeRenderWindow(HWND hWnd)
	{
		mpDevice->changeSwapChain(hWnd);
	}

	void CEngine::activeRenderer(UINT32 uflag)
	{
		CRendererPlugin* pRendererPlugin = getPlugin<CRendererPlugin>(RENDERER);
		if (pRendererPlugin != nullptr)
		{
			pRendererPlugin->activeRenderer(uflag);
		}
	}

	void CEngine::disableRenderer(UINT32 uflag)
	{
		CRendererPlugin* pRendererPlugin = getPlugin<CRendererPlugin>(RENDERER);
		if (pRendererPlugin != nullptr)
		{
			pRendererPlugin->disableRenderer(uflag);
		}
	}

	void CEngine::activeRendererEditor(UINT32 uflag)
	{
		CRendererPlugin* pRendererPlugin = getPlugin<CRendererPlugin>(RENDERER);
		if (pRendererPlugin != nullptr)
		{
			pRendererPlugin->activeRendererEditor(uflag);
		}
	}

	void CEngine::disableRendererEditor(UINT32 uflag)
	{
		CRendererPlugin* pRendererPlugin = getPlugin<CRendererPlugin>(RENDERER);
		if (pRendererPlugin != nullptr)
		{
			pRendererPlugin->disableRendererEditor(uflag);
		}
	}

	CEntity* CEngine::getActiveCamera()
	{
		return mpActiveScene->getCamera();
	}

	std::string CEngine::getSceneName()
	{
		if (mpActiveScene != nullptr)
		{
			return mpActiveScene->getSceneName();
		}
		else
		{
			return "Error";
		}
	}


	void CEngine::initPlugin()
	{
		mpPluginFactory = CPluginFactory::initialize();
		if (nullptr != mpPluginFactory)
		{
			addPlugin(PluginType::SCRIPT);
		}
	}

	LRESULT CALLBACK CEngine::engineMsgProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		event msg;
		if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) ||
			(message >= WM_KEYFIRST && message <= WM_KEYLAST))
		{

			msg.bProcess = false;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.Messtype = message;
			::GetCursorPos(&msg.point);
			::ScreenToClient(hWnd, &msg.point);
			GEngine->ProcessMsg(msg);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	void CEngine::addRenderNodeToScene(CRenderNode* pRenderNode)
	{
		if (mpActiveScene != nullptr)
		{
			mpActiveScene->addRenderNode(pRenderNode);
		}
	}

	void CEngine::removeRenderNodeToScene(CRenderNode* pRenderNode)
	{
		if (mpActiveScene != nullptr)
		{
			mpActiveScene->removeRenderNode(pRenderNode);
		}
	}

	CRenderNode* CEngine::createRenderNode()
	{
		auto plugin = CRendererPlugin::getSinglePtr();
		if (nullptr != plugin)
		{
			return plugin->createRender();
		}
		else
		{
			return nullptr;
		}
	}

}
