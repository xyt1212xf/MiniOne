#include "pch.h"
#include "CubeActor.h"
#include "UnitDerive.h"
namespace mini
{
	CCubeActor::CCubeActor()
	{
	}


	CCubeActor::~CCubeActor()
	{
	}

	void CCubeActor::updateFrame()
	{
		mRotation.y += 0.01f;
		setRotation(mRotation);
	}

	bool CCubeActor::initlalise(const std::string& file, const std::string& materilaName)
	{
		CActor::initlalise(file, materilaName);
		CMeshUnit* pMeshUnit = new CMeshUnit;
		mPosition.x = -5;
		pMeshUnit->setPositon(mPosition);
		pMeshUnit->setRotation(mRotation);
		pMeshUnit->setScale(mScale);
		pMeshUnit->setUnitFile("resource/Model/Default/cube.m");
		pMeshUnit->onInitialise();
		mpUnits.push_back(pMeshUnit);
		mpMeshUnit = pMeshUnit;

		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
#if 1
		pMaterialUnit->setUnitFile("resource/Material/specularMaterial.ma");
#elif DiffuseMaterial
		pMaterialUnit->setUnitFile("resource/Material/diffuseMaterial.ma");
#else
		pMaterialUnit->setUnitFile("resource/Material/ambientMaterial.ma");
#endif
		pMaterialUnit->onInitialise();
		pMeshUnit->buildMaterialUnit(pMaterialUnit);
		SafeRelease(pMaterialUnit);
		return true;
	}
}
