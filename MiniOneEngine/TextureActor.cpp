#include "pch.h"
#include "TextureActor.h"
#include "UnitDerive.h"
namespace mini
{
	CTextureActor::CTextureActor()
	{

	}


	CTextureActor::~CTextureActor()
	{
	}

	bool CTextureActor::initlalise(const std::string& file, const std::string& materilaName)
	{
		CActor::initlalise(file, materilaName);
		CMeshUnit* pMeshUnit = new CMeshUnit;
		pMeshUnit->setPositon(mPosition);
		pMeshUnit->setRotation(mRotation);
		pMeshUnit->setScale(mScale);
		pMeshUnit->setUnitFile("resource/Model/Default/cube.m2");
		pMeshUnit->onInitialise();
		mpUnits.push_back(pMeshUnit);
		mpMeshUnit = pMeshUnit;

		Property& meshPropery = pMeshUnit->getProperty("Mesh");
		CMesh* pMesh = static_cast<CMesh*>(meshPropery.pMemory);

		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
#if 1
		pMaterialUnit->setUnitFile("resource/Material/normalMapMaterial.ma");
#elif
		pMaterialUnit->setUnitFile("resource/Material/alphaMaterial.ma");
#elif lightmapmaterial
		pMaterialUnit->setUnitFile("resource/Material/lightMapMaterial.ma");
#elif multexture
		pMaterialUnit->setUnitFile("resource/Material/textureMaterial.ma");
#endif
		pMaterialUnit->onInitialise();

		Property& materialPropery = pMaterialUnit->getProperty("Material");
		CMaterial* pMaterial = static_cast<CMaterial*>(materialPropery.pMemory);

		pMeshUnit->buildMaterialUnit(pMaterialUnit);
		SafeRelease(pMaterialUnit);

		return true;
	}

}
