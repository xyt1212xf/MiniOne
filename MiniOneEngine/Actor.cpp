#include "pch.h"
#include "Actor.h"
#include "RendererPlugin.h"
#include "Camera.h"
#include "Engine.h"
namespace mini
{
	CActor::CActor()
	{
		mPosition = CVector(0, 0, 0);
		mScale = CVector(1, 1, 1);
		mRotation = CVector(0, 0, 0);
		mType = Actor;
	}


	CActor::~CActor()
	{
		for (auto item : mpUnits)
		{
			SafeDelete(item);
		}
		//SafeRelease(mpMeshUnit);
	}

	bool CActor::initlalise(const std::string& fileName, const std::string& materilaName)
	{
		CResourceBase* pResource = GResourceManager->createResource(fileName, Mesh);
		SafeRelease(mpMeshUnit);
		mpMeshUnit = new CMeshUnit();
		mpMeshUnit->bindMesh(dynamic_cast<CMesh*>(pResource));
		mpUnits.push_back(mpMeshUnit);
		createMaterial(materilaName);
		initialiseFinished();
		return true;
	}


	void CActor::updateUnitProperty(Property* pProperty)
	{
		for (auto item : mpUnits)
		{
			(*item).updateUnitProperty(pProperty);
		}
	}

	void CActor::setPosition(CVector pt)
	{
		mPosition = pt;
		if (mpMeshUnit != nullptr)
		{
			mpMeshUnit->setPositon(mPosition);
		}
		mbUpdate = true;
	}

	void CActor::setScale(CVector scale)
	{
		mScale = scale;
		if (mpMeshUnit != nullptr)
		{
			mpMeshUnit->setScale(mScale);
		}
		mbUpdate = true;
	}

	void CActor::setRotation(CVector rotation)
	{
		mRotation = rotation;
		if (mpMeshUnit != nullptr)
		{
			mpMeshUnit->setRotation(mRotation);
		}
		mbUpdate = true;
	}

	void CActor::bindMeshUnit(CMeshUnit* pMeshUnit)
	{
		if (mpMeshUnit != pMeshUnit)
		{
			SafeRelease(mpMeshUnit);
		}
		mpMeshUnit = pMeshUnit;
		if (nullptr != mpMeshUnit)
		{
			mpMeshUnit->AddRef();
		}
	}

	CVector& CActor::getPosition()
	{
		return mPosition;
	}

	CVector& CActor::getScale()
	{
		return mScale;
	}

	CVector& CActor::getRotation()
	{
		return mRotation;
	}

	std::pair<bool, float> CActor::isPick(const CVector& v, bool isPickBound /*= true*/)
	{
		if (mType != Gui)
		{
			CVector origin;
			CCamera* pCamera = dynamic_cast<CCamera*>(GEngine->getActiveCamera());
			if (isPickBound)
			{
				return std::pair<bool, float>(false, 0.0f);
			}
			else
			{
				INTERSECTION& result = mpMeshUnit->isPick(v, isPickBound);
				if (result.fDist > FLT_MAX - 10.0f)
				{
					return std::pair<bool, float>(false, 0.0f);
				}
				else
				{
					return std::pair<bool, float>(true, result.fDist);
				}
			}
		}
		else
		{
			return std::pair<bool, float>(false, 0.0f);
		}
	}

	CBoundBox& CActor::getBoundBox()
	{
		return mpMeshUnit->getBoundBox();
	}

	void CActor::update()
	{
		updateFrame();
		if (mbUpdate)
		{
			for (auto& item : mpUnits)
			{
				(*item).update();
			}
		}
	}

	void CActor::createMaterial(const std::string& materialName) noexcept
	{
		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
		pMaterialUnit->setUnitFile(materialName);
		pMaterialUnit->onInitialise();
		mpMeshUnit->bindMaterial(pMaterialUnit);
		SafeRelease(pMaterialUnit);
	}

	void CActor::updateFrame()
	{

	}

	void CActor::initialiseFinished()
	{

	}

}
