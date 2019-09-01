#pragma once

#include "SceneNode.h"
namespace mini
{
	class CSceneManagerInterface
	{
	public:
		CSceneManagerInterface();
		virtual ~CSceneManagerInterface();
		virtual void pickNode();
		virtual void addSceneNode(CSceneNode* pSceneNode);
		virtual void removeSceneNode(CSceneNode* pSceneNode);
		virtual void clearAllSceneNode();

	private:
		std::list<CSceneNode* >	mSceneNodes;
	};

}
