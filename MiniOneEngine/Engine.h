#pragma once
#include <thread>
#include "EngineHead.h"
#include "PlugInFactory.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "RenderNode.h"
#include "FoundationStruct.h"

#ifdef  WIN32
#include <windows.h>
#endif

namespace mini
{
	class CScene;
	class CActor;
	enum EngineStatus
	{
		Running,
		Editoring,
	};

	class CEngine : public TSingle<CEngine>
	{
	public:
		CEngine();
		bool init(HWND hWnd);
		bool onStart(const char* pStrSceneName = nullptr);
		void onRun();
		void onShutDown();
		template<typename T>
		T* getPlugin(PluginType type) const;

		void addPlugin(PluginType type);
		void removePlugin(PluginType type);
		HWND	getHWND();
		CScene*	createScene(const std::string& strSceneName);
		CActor* createActor(const std::string& fileName, EntityType type);
		CActor* createActor(const std::string& fileName,const std::string& materialName, EntityType type);
		std::string getSceneName();
		void	disableRendererPlugin();
		void	enableRendererPlugin();
		void	stopRender();
		void	recoverRender();
		class CDevice* getDevice();
		bool createRenderWindow(HWND hWnd);
		void changeRenderWindow(HWND hWnd);
		void activeRenderer(UINT32 uflag);
		void disableRenderer(UINT32 uflag);
		void activeRendererEditor(UINT32 uflag);
		void disableRendererEditor(UINT32 uflag);

		CEntity*	getActiveCamera();
		CActor* createActor(EntityType type);
		CScene*	getCurrentScene();
		UINT32	getDeviceWidth();
		UINT32	getDeviceHeight();
		void	setStatus(EngineStatus status);
		EngineStatus getStatus();
		bool ProcessMsg(event& msg);
		CEntity* getPickEntity();
		bool createTerrain(std::string strHeightMap);
		bool createTerrain(INT32 width, INT32 height);
		CEntity*	getTerrainActor();
		bool IntersectTriangle(const float* orig, const float* dir, float* v0, float* v1, float* v2, float* t, float* u, float* v);
		static LRESULT CALLBACK   engineMsgProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void addRenderNodeToScene(CRenderNode* pRenderNode);
		void removeRenderNodeToScene(CRenderNode* pRenderNode);
		CRenderNode* createRenderNode();

#ifdef EDITOR
		void addActorToScene(CEntity* pEntity);
#endif // EDITOR

	private:
		void initPlugin();

	private:
		HWND				mhWnd;
		CResourceManager*	mResourceManager;
		CPluginFactory*		mpPluginFactory;
		CScene*				mpActiveScene;
		mutable std::map<PluginType, CPlugin*>	mpPluginList;
		EngineStatus		m_status;
		class CDevice*		mpDevice;
		CEntity*			mpPickEntity;
		CEntity*			mpTerrain = nullptr;
		std::thread*		mpRenderThread = nullptr;
		std::thread*		mpLogicThread = nullptr;
	};



	template<typename T>
	T* mini::CEngine::getPlugin(PluginType type) const
	{
		auto item = mpPluginList.find(type);
		if (item != mpPluginList.end())
		{
			return dynamic_cast<T*>(item->second);
		}
		return nullptr;
	}
	extern	CEngine* GEngine;

	#define  GETPLUGIN(Plugin, PluginType) \
		GEngine->getPlugin<Plugin>(PluginType);
}
