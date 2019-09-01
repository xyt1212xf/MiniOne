#pragma once
#include "EngineHead.h"
#include "LodSceneManager.h"
#include "BspSceneManager.h"
namespace mini
{
	enum ScnenManagerType
	{
		Lod,
		Bsp,
	};

	class CSceneManagerFactory : TSingle<CSceneManagerFactory>
	{
	public:
		CSceneManagerFactory();
		~CSceneManagerFactory();
		CSceneManagerInterface*	createSceneManager(ScnenManagerType type);

	};
}
