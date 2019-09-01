#pragma once
#include "EngineHead.h"
#include "SceneManagerInterface.h"
#include "SceneNode.h"
#include "Camera.h"

namespace mini
{
	class CActor;
	class CScene
	{
	public:
		CScene();
		virtual ~CScene();
		void	load(const std::string& fileName);
		void	save();
		void	saveAs(const std::string& fileName);
		void	reload();
		void	setSceneName(const std::string& strName);

		bool	addActor(CActor* pEntity);
		bool	removeActor(CActor* pEntity);
		bool	addRenderNode(CRenderNode* pRenderNode);
		bool	removeRenderNode(CRenderNode* pRenderNode);
		void	setCamera(CActor* pCamera);
		void	removeCamera(CActor* pCamera);

		CActor* findActor(CActor* pEntity);
		CActor* findActorByUUID(UINT32 uuid);
		CCamera* getCamera();

		void				releaseSceneActors();
		void				updateScene();
		void				setActiveCamear(CCamera* pCamear);

		void				setStartPt(CVector& pt);
		CVector&			getStartPt();
		const std::string&	getSceneName()const;
		CEntity*			calculationPickEntity(float x, float y);
		void initGroundPlane();
		void processMsg(event& msg);

	private:
		std::string							mSceneName;
		std::string							mSaveName;
		std::list<CSceneManagerInterface*>	mSceneInterface;
		std::list<CCamera*>					mCameras;
		std::map<UINT32, CActor*>			mActors;
		std::set<CRenderNode*>				mRenderNodes;
		CCamera*							mActiveCamera;
		CVector								mStartPt;
		CVector								mLookAtPt;
	};
}
