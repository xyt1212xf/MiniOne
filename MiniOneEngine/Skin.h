#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "GPUBuffer.h"
#include "FoundationStruct.h"
#include "Mesh.h"
namespace mini
{
	class CSkin : public CResourceBase
	{
	public:
		CSkin();
		virtual ~CSkin();
		virtual bool initialise();

	private:
		std::list<CMesh*>	mMeshes;
	};
}