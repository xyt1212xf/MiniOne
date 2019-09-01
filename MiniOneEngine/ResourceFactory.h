#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"

namespace mini
{
	class CResourceFactory : public TSingle<CResourceFactory>
	{
	public:
		CResourceFactory();
		virtual ~CResourceFactory();
		CResourceBase* createResource(ResType type);
	};
}
