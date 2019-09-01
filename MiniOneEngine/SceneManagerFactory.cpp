#include "pch.h"
#include "SceneManagerFactory.h"

namespace mini
{
	CSceneManagerFactory::CSceneManagerFactory()
	{
	}


	CSceneManagerFactory::~CSceneManagerFactory()
	{
	}

	CSceneManagerInterface* CSceneManagerFactory::createSceneManager(ScnenManagerType type)
	{
		CSceneManagerInterface* pSceneManager = nullptr;
		switch (type)
		{
		case Lod:
			pSceneManager = new CLodSceneManager();
			break;
		case Bsp:
			pSceneManager = new CBspSceneManager();
			break;
		default:
			break;
		}
		return pSceneManager;
	}
}
