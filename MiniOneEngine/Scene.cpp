#include "pch.h"
#include "Scene.h"
#include "RendererPlugin.h"
#include "Actor.h"
namespace mini
{
	CScene::CScene()
	{
		mSceneName = "";
		mSaveName = "";
		CRendererPlugin::getSinglePtr()->setScene(this);

		mStartPt = CVector(0, 10, -7.0f);
		mLookAtPt = CVector(0, 0, 0);
	
	}

	CScene::~CScene()
	{
		releaseSceneActors();
		SafeRelease(mActiveCamera);
	}

	void CScene::load(const std::string& fileName)
	{
		if (mSceneName.length() == 0)
		{
			mSceneName = fileName;
		}
		mSaveName = fileName;
	}

	void CScene::save()
	{
		if (mSaveName == "")
		{

		}
	}

	void CScene::saveAs(const std::string& fileName)
	{
		std::string tempName = mSceneName;
		save();
		mSaveName = tempName;
	}

	void CScene::reload()
	{

	}

	void CScene::setSceneName(const std::string& strName)
	{
		mSceneName = strName;
	}

	bool CScene::addActor(CActor* pActor)
	{
		if (pActor != nullptr)
		{
			UINT32 uuid = pActor->getUUID();
			auto item = mActors.find(pActor->getUUID());
			if (item == mActors.end())
			{
				mActors[uuid] = pActor;
				pActor->AddRef();
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool CScene::removeActor(CActor* pEntity)
	{
		if (pEntity != nullptr)
		{
			UINT32 uuid = pEntity->getUUID();
			auto item = mActors.find(uuid);
			if (item != mActors.end())
			{
				mActors.erase(item);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CScene::addRenderNode(CRenderNode* pRenderNode)
	{
		if (pRenderNode != nullptr)
		{
			auto item = std::find(mRenderNodes.begin(), mRenderNodes.end(), pRenderNode);
			if (item == mRenderNodes.end())
			{
				mRenderNodes.insert(pRenderNode);
				pRenderNode->AddRef();
				CRendererPlugin::getSinglePtr()->addRenderNode(pRenderNode);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool CScene::removeRenderNode(CRenderNode* pRenderNode)
	{
		if (pRenderNode != nullptr)
		{
			auto item = std::find(mRenderNodes.begin(), mRenderNodes.end(), pRenderNode);
			if (item != mRenderNodes.end())
			{
				mRenderNodes.erase(item);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void CScene::setCamera(CActor* pCamera)
	{
		if (pCamera->getEntityType() == EntityType::Camera)
		{
			mActiveCamera = dynamic_cast<CCamera*>(pCamera);
			mActiveCamera->AddRef();
			mActiveCamera->setEye(mStartPt);
			mActiveCamera->setLookAt(mLookAtPt);
			//addActor(pCamera);
		}
	}

	void CScene::removeCamera(CActor* pCamera)
	{
		if (pCamera->getEntityType() == EntityType::Camera)
		{
			removeActor(pCamera);
		}
	}

	CActor* CScene::findActor(CActor* pActor)
	{
		if (pActor != nullptr)
		{
			UINT32 uuid = pActor->getUUID();
			return findActorByUUID(uuid);
		}
		else
		{
			return nullptr;
		}
	}

	CActor* CScene::findActorByUUID(UINT32 uuid)
	{
		auto item = mActors.find(uuid);
		if (item != mActors.end())
		{
			return (item->second);
		}
		else
		{
			return nullptr;
		}
	}

	CCamera* CScene::getCamera()
	{
		return mActiveCamera;
	}

	void CScene::releaseSceneActors()
	{
		for (auto item : mActors)
		{
			SafeRelease((item.second));
		}
		mActors.clear();
	}


	void CScene::updateScene()
	{
		for (auto& node : mActors)
		{
			(node.second)->update();
		}
	}

	void CScene::setActiveCamear(CCamera* pCamear)
	{
		if (mActiveCamera != pCamear && nullptr != pCamear)
		{
			SafeRelease(mActiveCamera);
			mActiveCamera = pCamear;
			mActiveCamera->AddRef();
		}
	}

	void CScene::setStartPt(CVector& pt)
	{
		mStartPt = pt;
	}

	CVector& CScene::getStartPt()
	{
		return mStartPt;
	}

	const std::string& CScene::getSceneName() const
	{
		return mSceneName;
	}

	CEntity* CScene::calculationPickEntity(float x, float y)
	{
		static DeviceConfig& conflg = GEngine->getDevice()->getConfig();

		CMatrix inverseViewMatrix;
		CMatrix& projectionMatrix = mActiveCamera->getProjectMatrix();

		CVector v;
		// Move the mouse cursor coordinates into the -1 to +1 range.
		v.x = ((2.0f * x / (float)conflg.screenWidth) - 1.0f) / projectionMatrix._11;
		v.y = -((2.0f * y / (float)conflg.screenHeight) - 1.0f) / projectionMatrix._22;
		v.z = 1.0f;

		float distance = FLT_MAX;
		CEntity* pPickEntity = nullptr;
		for (auto item : mActors)
		{
			if ((item.second)->isVisual())
			{
				auto result = (item.second)->isPick(v, false);
				if (result.first && result.second < distance)
				{
					distance = result.second;
					pPickEntity = item.second;
				}
			}
		}
		return pPickEntity;
	}

	void CScene::initGroundPlane()
	{
		GEngine->stopRender();

		mini::CRenderNode* pRenderNode = mini::GEngine->createRenderNode();
		mini::CActor* pSkyDome = mini::GEngine->createActor("Resource/Model/skydome.txt", "Resource/Material/skyDomeMaterial.ma", mini::EntityType::SkyDome);
		pRenderNode->bindActor(pSkyDome, mini::RendererType::FrontRenderer);
		mini::GEngine->addRenderNodeToScene(pRenderNode);
		pSkyDome->setScale(CVector(4, 4, 4));
		SafeRelease(pSkyDome);
		SafeRelease(pRenderNode);
	
		{
			CRenderNode* pRenderNode = GEngine->createRenderNode();
			CActor* pGroundPlane = GEngine->createActor("Resource/Model/plane.txt", "Resource/Material/planeMaterial.ma", EntityType::Actor);

			pRenderNode->bindActor(pGroundPlane, mini::RendererType::FrontRenderer);
			GEngine->addRenderNodeToScene(pRenderNode);
			GEngine->recoverRender();
			pGroundPlane->setScale(CVector(10, 1, 10));
			SafeRelease(pGroundPlane);
			SafeRelease(pRenderNode);
		}
	}

	void CScene::processMsg(event& msg)
	{
		mActiveCamera->processMove(msg);
		calculationPickEntity((float)msg.point.x, (float)msg.point.y);
	}

}
