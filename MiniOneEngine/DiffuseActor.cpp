#include "pch.h"
#include "DiffuseActor.h"
#include "UnitDerive.h"
namespace mini
{
	CDiffuseActor::CDiffuseActor()
	{
	}


	CDiffuseActor::~CDiffuseActor()
	{
	}
	void CDiffuseActor::updateFrame()
	{
		mRotation.y += 0.01f;
		setRotation(mRotation);
	}

	bool CDiffuseActor::initlalise(const std::string& file, const std::string& materilaName)
	{
		CActor::initlalise(file, materilaName);
		CMeshUnit* pMeshUnit = new CMeshUnit;
		pMeshUnit->setPositon(mPosition);
		pMeshUnit->setRotation(mRotation);
		pMeshUnit->setScale(mScale);
		pMeshUnit->setUnitFile("resource/Model/bian_fu_001_ty.fbx");
		pMeshUnit->onInitialise();
		mpUnits.push_back(pMeshUnit);

		Property& meshPropery = pMeshUnit->getProperty("Mesh");
		CMesh* pMesh = static_cast<CMesh*>(meshPropery.pMemory);
		mpMeshUnit = pMeshUnit;

#if 0
#ifdef WIN32
		VertexFormatVTN vertices[3];
		vertices[0].position = CPoint3F(-1.0f, -1.0f, 0.0f);  // Bottom left.
		vertices[0].uv = CPoint2F(0.0f, 1.0f);
		vertices[0].normal = CPoint3F(0.0f, 0.0f, -1.0f);

		vertices[1].position = CPoint3F(0.0f, 1.0f, 0.0f);  // Top middle.
		vertices[1].uv = CPoint2F(0.5f, 0.0f);
		vertices[1].normal = CPoint3F(0.0f, 0.0f, -1.0f);

		vertices[2].position = CPoint3F(1.0f, -1.0f, 0.0f);  // Bottom right.
		vertices[2].uv = CPoint2F(1.0f, 1.0f);
		vertices[2].normal = CPoint3F(0.0f, 0.0f, -1.0f);
#endif

		UINT32 indices[3];

		// Load the index array with data.
		indices[0] = 0;  // Bottom left.
		indices[1] = 1;  // Top middle.
		indices[2] = 2;  // Bottom right

		pMesh->fillVertexBuffer(vertices, sizeof(vertices), sizeof(VertexFormatVTN));
		pMesh->fillIndexBuffer(&indices, sizeof(indices), sizeof(UINT32));
#endif

		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
#if 1
		pMaterialUnit->setUnitFile("resource/Material/monsterMaterial.ma");
#else
		pMaterialUnit->setUnitFile("resource/Material/diffuseMaterial.ma");
#endif
		pMaterialUnit->onInitialise();

		pMeshUnit->buildMaterialUnit(pMaterialUnit);
		SafeRelease(pMaterialUnit);
		return true;
	}
}
