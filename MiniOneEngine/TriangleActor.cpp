#include "pch.h"
#include "TriangleActor.h"
#include "UnitDerive.h"
namespace mini
{
	CTriangleActor::CTriangleActor()
	{
	}


	CTriangleActor::~CTriangleActor()
	{
	}

	bool CTriangleActor::initlalise(const std::string& file, const std::string& materilaName)
	{
		CActor::initlalise(file,materilaName);
		CMeshUnit* pMeshUnit = new CMeshUnit;
		pMeshUnit->setPositon(mPosition);
		pMeshUnit->setRotation(mRotation);
		pMeshUnit->setScale(mScale);

		pMeshUnit->onInitialise();
		mpUnits.push_back(pMeshUnit);
		mpMeshUnit = pMeshUnit;

		Property& meshPropery = pMeshUnit->getProperty("Mesh");
		CMesh* pMesh = static_cast<CMesh*>(meshPropery.pMemory);
		VertexFormatVTN vertices[3];

#ifdef WIN32
		vertices[0].position = CPoint3F(-1.0f, -1.0f, 0.0f);  // Bottom left.
		vertices[0].normal = CPoint3F(0.0f, 1.0f, 0.0f);

		vertices[1].position = CPoint3F(0.0f, 1.0f, 0.0f);  // Top middle.
		vertices[1].normal = CPoint3F(0.0f, 1.0f, 0.0f);

		vertices[2].position = CPoint3F(1.0f, -1.0f, 0.0f);  // Bottom right.
		vertices[2].normal = CPoint3F(0.0f, 1.0f, 0.0f);
#endif

		UINT32 indices[3];

		// Load the index array with data.
		indices[0] = 0;  // Bottom left.
		indices[1] = 1;  // Top middle.
		indices[2] = 2;  // Bottom right

		pMesh->fillVertexBuffer(vertices, sizeof(vertices), sizeof(VertexFormatVTN));
		pMesh->fillIndexBuffer(&indices, sizeof(indices), sizeof(UINT32));

		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
		pMaterialUnit->setUnitFile("resource/Material/colorMaterial.ma");
		pMaterialUnit->onInitialise();

		pMeshUnit->buildMaterialUnit(pMaterialUnit);
		SafeRelease(pMaterialUnit);

		return true;
	}

}
