#include "pch.h"
#include "FbxActor.h"
#include "UnitDerive.h"
namespace mini
{
	CFbxActor::CFbxActor()
	{
	}


	CFbxActor::~CFbxActor()
	{
	}

	void CFbxActor::updateFrame()
	{
		mRotation.y += 0.01f;
		setRotation(mRotation);
	}

	bool CFbxActor::initlalise(const std::string& file, const std::string& materilaName)
	{
		CActor::initlalise(file,materilaName);
		CMeshUnit* pMeshUnit = new CMeshUnit;
		pMeshUnit->setPositon(mPosition);
		pMeshUnit->setRotation(mRotation);
		pMeshUnit->setScale(mScale);
		pMeshUnit->setUnitFile("resource/Model/monster.fbx");
		pMeshUnit->onInitialise();
		mpUnits.push_back(pMeshUnit);
		mpMeshUnit = pMeshUnit;

		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
		pMaterialUnit->setUnitFile("resource/Material/monsterMaterial.ma");

		pMaterialUnit->onInitialise();
		pMeshUnit->buildMaterialUnit(pMaterialUnit);
		SafeRelease(pMaterialUnit);
		return true;
	}
}
